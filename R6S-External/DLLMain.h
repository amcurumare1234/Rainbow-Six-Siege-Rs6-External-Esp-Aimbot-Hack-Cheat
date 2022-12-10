#pragma once
#include <Windows.h>
#include "XorStr.h"
#include "GameManager.h"
#include "StatusManager.h"
#include <iostream>



namespace Hook
{
	extern LONG_PTR oriWndProc;

	LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}