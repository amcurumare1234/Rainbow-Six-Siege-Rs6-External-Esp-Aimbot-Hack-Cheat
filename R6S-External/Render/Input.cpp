//          _                          _          __ ___   ___   ___  
//    /\   | |                        | |        /_ |__ \ / _ \ / _ \ 
//   /  \  | | _____  ____ _ _ __   __| | ___ _ __| |  ) | | | | (_) |
//  / /\ \ | |/ _ \ \/ / _` | '_ \ / _` |/ _ \ '__| | / /| | | |> _ < 
// / ____ \| |  __/>  < (_| | | | | (_| |  __/ |  | |/ /_| |_| | (_) |
///_/    \_\_|\___/_/\_\__,_|_| |_|\__,_|\___|_|  |_|____|\___/ \___/ 

#pragma once

#include "Input.h"
#include "Settings.h"

#include "ImGui/imgui.h"

Input* Input::m_pInstance;

Input::Input()
{

}

Input::~Input()
{

}

void Input::StartThread()
{
	m_hThread = CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(MenuKeyMonitor), NULL, NULL, NULL);
}

void Input::StopThread()
{
	TerminateThread(m_hThread, 0);
}

bool init = false;

void Input::MenuKeyMonitor()
{
	HWND gameWindow = FindWindowA(NULL, "Rainbow Six");

	while (true)
	{
		if (Settings::Menu)
		{
			POINT mousePosition;
			GetCursorPos(&mousePosition);
			ScreenToClient(gameWindow, &mousePosition);

			ImGuiIO& io = ImGui::GetIO();
			io.MousePos.x = mousePosition.x;
			io.MousePos.y = mousePosition.y;

			bool hover = ImGui::IsWindowHovered();

			if (GetAsyncKeyState(VK_LBUTTON)){
				io.MouseDown[0] = true;
			}
			else {
				io.MouseDown[0] = false;
			}
		}
		else
		{
			std::this_thread::sleep_for(
				std::chrono::milliseconds(250));
		}

		if (GetAsyncKeyState(VK_INSERT))
		{
			Settings::Menu = !Settings::Menu;

			std::this_thread::sleep_for(
				std::chrono::milliseconds(250));
		}
	}
}

Input* Input::GetInstance()
{
	if (!m_pInstance)
		m_pInstance = new Input();

	return m_pInstance;
}