#pragma once
#include <Windows.h>
#include <string>
#include "Utility.h"
struct PlayerInfo {
	DWORD		Health;
	Vector3		Position;
	Vector3		w2s; 
	Vector3		w2sHead;
	BYTE		TeamId;
	std::string	Name;
};
class Mem{
private:
	DWORD_PTR pGameManager;
	DWORD_PTR pEntityList;
	DWORD_PTR pRender;
	DWORD_PTR pGameRender;
	DWORD_PTR pEngineLink;
	DWORD_PTR pEngine;
	DWORD_PTR pCamera;

public:
	int displayWidth = 1920;
	int displayHeight = 1080;
	Mem();
	BOOL SetBaseAddress();
	DWORD_PTR GetBaseAddress();
	template<typename T> T RPM(SIZE_T address);
	template<typename T> void WPM(SIZE_T address, T buffer);
	std::string RPMString(SIZE_T address);
	void UpdateAddresses();
	DWORD_PTR GetEntity(int i);
	DWORD_PTR GetLocalEntity();
	DWORD GetEntityHealth(DWORD_PTR entity);
	Vector3 GetEntityFeetPosition(DWORD_PTR entity);
	Vector3 GetEntityHeadPosition(DWORD_PTR entity);
	std::string GetEntityPlayerName(DWORD_PTR entity);
	BYTE GetEntityTeamId(DWORD_PTR entity);
	PlayerInfo GetAllEntityInfo(DWORD_PTR entity);
	void ZeroRecoilSpread(DWORD_PTR entity);
	Vector3 GetViewTranslation();
	Vector3 GetViewRight();
	Vector3 GetViewUp();
	Vector3 GetViewForward();
	float GetFOVX();
	float GetFOVY();
	Vector3 WorldToScreen(Vector3 position);
};