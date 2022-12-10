#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <Dwmapi.h> 
#include <TlHelp32.h>

class Overlay {

public:
	 
	Overlay();

	int ScreenWidth = 1920;

	int ScreenHeight = 1080;

	LPDIRECT3D9 d3;
	LPDIRECT3DDEVICE9 d3dev;

	LPD3DXFONT d3Font;

	HWND hWindow;
	HWND tWindow;

	MARGINS margins = { 0, 0, ScreenWidth, ScreenHeight };

	ID3DXLine* d3Line;

	
	void InitD3D(HWND hWindow);

	
	void Render();

	
	void SetupWindow();

	
	WPARAM Loop();

	
	void ESP();

	void DrawCircle(int X, int Y, int radius, int numSides, DWORD Color);

	void FillArea(float x, float y, float width, float height, D3DCOLOR color);

	void FillRectangle(double x, double y, double w, double h, D3DCOLOR color);

	void DrawBox(float x, float y, float width, float height, D3DCOLOR color);

	void DrawString(int x, int y, DWORD color, LPD3DXFONT g_pFont, const char * fmt, ...);
};