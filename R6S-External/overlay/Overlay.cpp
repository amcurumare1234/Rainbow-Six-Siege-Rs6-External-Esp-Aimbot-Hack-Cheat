#include "stdafx.h"
#include "Overlay.h"
#include <iostream>
#include "Globals.h"
#include "Utility.h"
#include <time.h>
#include "Include.h"
#include "ABClient.h"
#include <thread>        
#include <chrono>       
#include <stdlib.h> 
#include "Util.h"
using namespace asmjs;
using namespace std;

Overlayyy::Overlayyy() {};

static DWORD readPtr(DWORD_PTR base, ABClient abc)
{

	DWORD_PTR address;
	DWORD_PTR value = 0;
	DWORD fadd = 0;
	address = base + 0x47C3498;
	abc.RpmRaw(address, &value, 8);
	value = value + 0xC00;
	abc.RpmRaw(value, &value, 8);
	value = value + 0x398;
	abc.RpmRaw(value, &value, 8);
	value = value + 0x1C8;
	abc.RpmRaw(value, &fadd, sizeof(DWORD));
	return fadd;

}
void leftclick() {

	mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
	std::this_thread::sleep_for(std::chrono::milliseconds(150));
	mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);

}




WPARAM Overlayyy::Loop() {
	ABClient xyz;
	xyz.Connect();
	xyz.Ping();
	xyz.AccuireProcessHandle(Util::GetProcessIdsByName("RainbowSix.exe")[0], 0x1478);
	auto base = xyz.GetProcessModuleBase("RainbowSix.exe");

	MSG message;
	RECT rect;

	while (TRUE) {

		if (readPtr(base, xyz) == 1)
			leftclick();

		ZeroMemory(&message, sizeof(MSG));

		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		if (message.message == WM_QUIT) {
			exit(0);
		}

		tWindow = NULL;
		tWindow = FindWindow(NULL, L"Rainbow Six");
		if (!tWindow) {
			std::cout << "Application exiting, failed to find the specified window!" << std::endl;
			ExitProcess(0);
		}

		ZeroMemory(&rect, sizeof(RECT));

		GetWindowRect(tWindow, &rect);
		ScreenWidth = rect.right - rect.left;
		ScreenHeight = rect.bottom - rect.top;

		MoveWindow(hWindow, rect.left, rect.top, ScreenWidth, ScreenHeight, true);

		HWND hWindow2 = GetForegroundWindow();
		HWND hWindow3 = GetWindow(hWindow2, GW_HWNDPREV);

		SetWindowPos(hWindow, hWindow3, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		UpdateWindow(hWindow);

		Render();

		Sleep(5);
	}
	return message.wParam;
}
bool once1 = FALSE;
bool twice2 = FALSE;
void Overlayyy::Render() {
	d3dev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	d3dev->BeginScene();

	MainLoop();

	d3dev->EndScene();
	d3dev->Present(NULL, NULL, NULL, NULL);
}
bool okayy = true;
void Overlayyy::MainLoop() {
	//Update addresses
	Global::Memory.UpdateAddresses();

	//Set our colors for later
	DWORD boxColor = D3DCOLOR_ARGB(255, 0, 255, 255);
	DWORD headCircleColor = D3DCOLOR_ARGB(255, 0, 255, 255);
	DWORD healthBarColor = D3DCOLOR_ARGB(255, 255, 100, 100);
	DWORD nameColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	DWORD blackColor = D3DCOLOR_ARGB(255, 0, 0, 0);
	DWORD redColor = D3DCOLOR_ARGB(255, 255, 0, 0);
	DWORD greenColor = D3DCOLOR_ARGB(255, 0, 255, 0);
	DWORD grayColor = D3DCOLOR_ARGB(255, 128, 128, 128);

	//Get our local entity
	DWORD_PTR LocalEntity = Global::Memory.GetLocalEntity();

	//Get all the info on local player
	PlayerInfo LocalPlayer = Global::Memory.GetAllEntityInfo(LocalEntity);
	//LocalPlayer.TeamId = team(Global::Memory.GetLocalEntityID(), (DWORD_PTR)Global::BaseAddress);
	//Loop through first 12 entities
	//Feel free to add an actual entity count
	//12 will work fine for multiplayer matches tho
	for (int i = 1; i < 11; i++) {
		
		//Get the current entity
		DWORD_PTR Entity = Global::Memory.GetEntity(i);

		//Get a PlayerInfo struct with all of this entity's info
		PlayerInfo Player = Global::Memory.GetAllEntityInfo(Entity);
		//Player.Name = teamN(i, (DWORD_PTR)Global::BaseAddress);
		//Player.TeamId = team(i, (DWORD_PTR)Global::BaseAddress);
		//	cout << Player.TeamId << endl;
		//	system("PAUSE");
		//Some checks to validate that its a real player
		//Only draws players on the other team
		//Won't draw on terrorsit in thunt/situation or the hostage becuase their name is blank
		if (Player.Health > 0 //Health over 0 //Health less than 200
			&& Player.Health <= 200 //Health less than 200
			&& Player.w2s.z >= 1.0f //Player is actually on the screen
			&& Player.w2sHead.z >= 1.0f //Player is actually on the screen
			//&& Player.Name != Global::LocalName//Name isn't empty
			//&& (Player.TeamId != Global::ID)
			//&& !Player.Name.empty()//Not on our team
			)
		{

			//Find out the box size to fit the player
			//Scales well at all distances
			//Works like 70% of the time when player is repelling
			//Box is kinda small and looks far away when laying down
			float BoxHeight1 = Player.w2s.y - Player.w2sHead.y;
			float BoxHeight2 = std::abs(Player.w2s.x - Player.w2sHead.x);
			float BoxHeight = (BoxHeight1 > BoxHeight2) ? BoxHeight1 : BoxHeight2;
			float BoxWidth = BoxHeight / 1.5f;

			//Draw the box around the player
			//Draws a black box first, then the actual colored box on top of it to create a black outline, makes it 10x cleaner
			Global::Over.DrawBox(Player.w2sHead.x - (BoxWidth / 2) - 1, Player.w2sHead.y + (BoxHeight / 500) - (BoxHeight / 5) - 1, BoxWidth + 2, BoxHeight + 17, blackColor);
			Global::Over.DrawBox(Player.w2sHead.x - (BoxWidth / 2), Player.w2sHead.y + (BoxHeight / 500) - (BoxHeight / 5), BoxWidth, BoxHeight + 15, boxColor);

			//Draw a circle around the players head
			Global::Over.DrawCircle(Player.w2sHead.x, Player.w2sHead.y, BoxHeight / 15, 30, headCircleColor);

			//Draw health bars
			//Find the current health
			//Health goes to 120, 0-20 is down, 20-120 is regular health
			float CurrentHealth = Player.Health / 120.0f;
			if (CurrentHealth < 0 || CurrentHealth >= 1) CurrentHealth = 1;

			//Draw the health bar
			//Like the box, draws a black outline
			Global::Over.FillArea(Player.w2sHead.x - (BoxWidth / 2) - 6 - 1, Player.w2sHead.y + (BoxHeight / 500) - (BoxHeight / 5) - 1, 5, BoxHeight + 17 + 2, blackColor);
			Global::Over.FillArea(Player.w2sHead.x - (BoxWidth / 2) - 6, Player.w2sHead.y + (BoxHeight / 500) - (BoxHeight / 5), 3, (BoxHeight + 15) * CurrentHealth, healthBarColor);

			//Draw the players name
			//once again, with a black outline
		//		Global::Over.DrawString(Player.w2sHead.x - (BoxWidth / 2), (Player.w2sHead.y - (BoxHeight / 5)) + BoxHeight + 10, blackColor, Global::Over.d3Font, "%s", Player.Name);
			//Global::Over.DrawString(Player.w2sHead.x - (BoxWidth / 2) - 1, (Player.w2sHead.y - (BoxHeight / 5)) + BoxHeight + 10 - 1, nameColor, Global::Over.d3Font, "%s", Player.Name);

			//Status shit
			DWORD aimbotStatusColor;
			DWORD spreadStatusColor;
			DWORD recoilStatusColor;

			if (Global::Aimbot == TRUE) {
				aimbotStatusColor = greenColor;
			}
			else {
				aimbotStatusColor = redColor;
			}

			if (Global::Spread == TRUE) {
				spreadStatusColor = greenColor;
			}
			else {
				spreadStatusColor = redColor;
			}

			if (Global::Recoil == TRUE) {
				recoilStatusColor = greenColor;
			}
			else {
				recoilStatusColor = redColor;
			}

			//Global::Over.DrawString(25, 25, boxColor, Global::Over.d3Font, "Siege External", "");

			//Global::Over.DrawString(25, 55, aimbotStatusColor, Global::Over.d3Font, "Aimbot [F5]", "");
			//	Global::Over.DrawString(25, 85, spreadStatusColor, Global::Over.d3Font, "No Spread [F1]", "");
			//Global::Over.DrawString(25, 115, recoilStatusColor, Global::Over.d3Font, "No Recoil [F2]", "");

			//No spread
			Global::Memory.ZeroSpread(LocalEntity);

			//No recoil
			Global::Memory.ZeroRecoil(LocalEntity);

			//Aimbot
			if (GetAsyncKeyState(VK_F5) & 1) {
				Global::Aimbot = !Global::Aimbot;
			}

			if (Global::Aimbot == TRUE) {
				Vector3 headPos = Player.w2sHead;

				float headX = headPos.x - GetSystemMetrics(SM_CXSCREEN) / 2;
				float headY = headPos.y - GetSystemMetrics(SM_CYSCREEN) / 2;
				if (headX >= -50 && headX <= 50 && headY >= -50 && headY <= 50) {
				//	if (GetAsyncKeyState(VK_RBUTTON) && headX != 0 && headY != 0) {
					//	mouse_event(MOUSEEVENTF_MOVE, headX, headY, NULL, NULL);
					//}
				}
			}

		}
	}
}

void Overlayyy::InitD3D(HWND handleWindow) {
	//Create our direct3d object
	d3 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS direct3DPresetParams;

	ZeroMemory(&direct3DPresetParams, sizeof(direct3DPresetParams));

	direct3DPresetParams.Windowed = true;
	direct3DPresetParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	direct3DPresetParams.hDeviceWindow = handleWindow;
	direct3DPresetParams.BackBufferFormat = D3DFMT_A8R8G8B8;
	direct3DPresetParams.BackBufferWidth = ScreenWidth;
	direct3DPresetParams.BackBufferHeight = ScreenHeight;

	direct3DPresetParams.EnableAutoDepthStencil = TRUE;
	direct3DPresetParams.AutoDepthStencilFormat = D3DFMT_D16;


	d3->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, handleWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &direct3DPresetParams, &d3dev);
	D3DXCreateFontA(d3dev, 17, 0, FW_HEAVY, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &d3Font);

	D3DXCreateLine(d3dev, &d3Line);
}

void Overlayyy::DrawCircle(int X, int Y, int radius, int numSides, DWORD color) {
	D3DXVECTOR2 Line[128];
	float Step = M_PI * 2.0 / numSides;
	int Count = 0;
	for (float a = 0; a < M_PI*2.0; a += Step)
	{
		float X1 = radius * cos(a) + X;
		float Y1 = radius * sin(a) + Y;
		float X2 = radius * cos(a + Step) + X;
		float Y2 = radius * sin(a + Step) + Y;
		Line[Count].x = X1;
		Line[Count].y = Y1;
		Line[Count + 1].x = X2;
		Line[Count + 1].y = Y2;
		Count += 2;
	}
	d3Line->Draw(Line, Count, color);
}

void Overlayyy::FillArea(float x, float y, float width, float height, D3DCOLOR color) {
	D3DXVECTOR2 vectorLine[2];
	d3Line->SetWidth(width);

	vectorLine[0].x = x + width / 2;
	vectorLine[0].y = y;
	vectorLine[1].x = x + width / 2;
	vectorLine[1].y = y + height;

	d3Line->Draw(vectorLine, 2, color);
}

void Overlayyy::FillRectangle(double x, double y, double w, double h, D3DCOLOR color)
{
	struct Vertex
	{
		float x, y, z, h;
		D3DCOLOR color;
	};

	Vertex vertices[4] =
	{
		x,  y, 0.0f, 1.0f, color,
		x + w, y, 0.0f, 1.0f, color,

		x + w, y + h, 0.0f, 1.0f, color,
		x,  y + h, 0.0f, 1.0f, color,
	};

	d3dev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	d3dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	d3dev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	d3dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(Vertex));
}

void Overlayyy::DrawBox(float x, float y, float width, float height, D3DCOLOR color) {
	D3DXVECTOR2 points[5];
	points[0] = D3DXVECTOR2(x, y);
	points[1] = D3DXVECTOR2(x + width, y);
	points[2] = D3DXVECTOR2(x + width, y + height);
	points[3] = D3DXVECTOR2(x, y + height);
	points[4] = D3DXVECTOR2(x, y);
	d3Line->SetWidth(1);
	d3Line->Draw(points, 5, color);
}

void Overlayyy::DrawString(int x, int y, DWORD color, LPD3DXFONT g_pFont, const char *fmt, ...)
{
	char buf[1024] = { 0 };
	va_list va_alist;
	RECT FontPos = { x, y, x + 120, y + 16 };
	va_start(va_alist, fmt);
	vsprintf_s(buf, fmt, va_alist);
	va_end(va_alist);
	g_pFont->DrawTextA(NULL, buf, -1, &FontPos, DT_NOCLIP, color);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		Global::Over.Render();
		break;
	case WM_CREATE:
		DwmExtendFrameIntoClientArea(hWnd, &Global::Over.margins);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

static const char alphanum[] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

int stringLength = sizeof(alphanum) - 1;

char genRandom()
{
	return alphanum[rand() % stringLength];
}

void Overlayyy::SetupWindow() {

	RECT rect;

	while (!tWindow) {
		tWindow = FindWindow(NULL, L"Rainbow Six");
	}

	if (tWindow != NULL) {
		GetWindowRect(tWindow, &rect);
	}
	else {
		std::cout << "Application exiting, error preapring the window. Error code: " << GetLastError() << std::endl;
		Sleep(3000);
		ExitProcess(0);
	}

	WNDCLASSEX windowClass;

	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)RGB(0, 0, 0);

	srand(time(0));
	std::wstring Str;
	for (unsigned int i = 0; i < 20; ++i)
	{
		Str += genRandom();

	}
	windowClass.lpszClassName = Str.c_str();

	RegisterClassEx(&windowClass);

	hWindow = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, windowClass.lpszClassName, Str.c_str(), WS_POPUP, rect.left, rect.top, ScreenWidth, ScreenHeight, NULL, NULL, windowClass.hInstance, NULL);

	SetLayeredWindowAttributes(hWindow, RGB(0, 0, 0), 0, ULW_COLORKEY);
	SetLayeredWindowAttributes(hWindow, 0, 255, LWA_ALPHA);

	ShowWindow(hWindow, SW_SHOW);
	InitD3D(hWindow);
}
