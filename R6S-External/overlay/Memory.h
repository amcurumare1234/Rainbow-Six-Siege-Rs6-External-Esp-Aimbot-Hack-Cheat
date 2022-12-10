#pragma once

#include <Windows.h>
#include <string>
#include "Utility.h"

//Struct to hold player info
struct PlayerInfo {
	DWORD		Health;
	Vector3		Position;
	Vector3		w2s;
	Vector3		w2sHead;
	BYTE		TeamId;
	std::string	Name;
};

class Memoryyy {

private:
	//Game
	DWORD_PTR pGameManager;
	DWORD_PTR pEntityList;

	//Camera
	DWORD_PTR pRender;
	DWORD_PTR pGameRender;
	DWORD_PTR pEngineLink;
	DWORD_PTR pEngine;
	DWORD_PTR pCamera;

public:
	//Hard coded display size cause lazy
	int displayWidth = GetSystemMetrics(SM_CXSCREEN);
	int displayHeight = GetSystemMetrics(SM_CYSCREEN);

	Memoryyy();

	//Method to set the base address of the game to our global variable
	//Mostly copy pasted from stack overflow tbh
	//Returns true if we got it
	//False if we didn't
	BOOL SetBaseAddress();

	//Just casts base address to a DWORD_PTR
	DWORD_PTR GetBaseAddress();

	//Method we use to ReadProcessMemory
	//Templated so we can read any type with the same function
	//Only param is the address to read
	//Returns whatever is read from that address
	template<typename T> T RPM(SIZE_T address);

	//Method we use to WriteProcessMemory
	//Templated so we can write any type with the same function
	//2 params, the address to write to and a buffer to write from
	template<typename T> void WPM(SIZE_T address, T buffer);

	//Method to read char arrays from memory
	//I split this into a seperate method cause my code was ran from a windows process which requires more checks
	//Pretty much just used for reading player names so far
	std::string RPMString(SIZE_T address);

	//Simple method to update all top-level addresses
	//Just in case something changes
	void UpdateAddresses();

	//Method to get i entity from the entity list
	//Just doest EntityList] + i * 0x0008
	DWORD_PTR GetEntity(int i);

	//method to return a pointer to the local entity
	DWORD_PTR GetLocalEntity();
	DWORD_PTR GetLocalEntityID();
	std::string teamN(int i, DWORD_PTR base);

	//Method to get an entity's health
	//Takes pointer to an entity as param
	//Returns health value as a DWORD (120 max health for pvp cause downed state)
	DWORD GetEntityHealth(DWORD_PTR entity);

	//Method to get entity's feet position
	//Takes pointer to an entity as param
	//Returns a Vector3 of the entity's 3D coordinates at their feet
	Vector3 GetEntityFeetPosition(DWORD_PTR entity);

	//Method to get an entity's head position
	//Takes pointer to an entity as param
	//Retunrs Vector3 of the entity's 3D coordiantes at their head
	Vector3 GetEntityHeadPosition(DWORD_PTR entity);

	//Method to get an entity's name
	std::string GetEntityPlayerName(DWORD_PTR entity);

	//Method to get an entities team id
	BYTE GetEntityTeamId(DWORD_PTR entity);

	//Method that gathers all the possible info about a player
	//Returns a PlayerInfo struct
	//Makes life easier
	//If you think this is a performance issues then fix it, but it really isn't
	PlayerInfo GetAllEntityInfo(DWORD_PTR entity);

	//Method to remove recoil and spread from the current weapon
	void ZeroSpread(DWORD_PTR entity);
	void ZeroRecoil(DWORD_PTR entity);

	//Method to get your view translation
	//Returns Vector3 of the view translation
	//Used for World2Screen stuff
	Vector3 GetViewTranslation();

	//Method to get your view right
	//Returns Vector3 of your view right
	//Used for World2Screen stuff
	Vector3 GetViewRight();

	//Method to get your view up
	//Returns Vector3 of your view up
	//USed for Wolrd2Screen stuff
	Vector3 GetViewUp();

	//Method to get your view forward
	//Returns Vector3 of your view forward
	//Used for World2Screen stuff
	Vector3 GetViewForward();

	//Method to get your current FOV X
	//Returns float of your FOV on the X axis
	//Used for Wolrd2Screen stuff
	float GetFOVX();

	//Method to get your current FOV Y
	//Returns float of your FOV on the Y axis
	//Used for World2Screen stuff
	float GetFOVY();

	//World 2 screen function
	//Translates 3d coordinates to 2d screen positions
	//Use that screen position to draw stuff
	Vector3 WorldToScreen(Vector3 position);
};
