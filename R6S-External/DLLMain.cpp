#include "DLLMain.h"


LONG_PTR Hook::oriWndProc = NULL;

void Log(char* szMessage)
{
	std::cout << "-- " << szMessage << std::endl;
}



LRESULT __stdcall Hook::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	//Log(XorStr("WndProc called"));
	auto pStatusManager = Engine::StatusManager::GetInstance();
	auto pGameManager = Engine::GameManager::GetInstance();

	if (pStatusManager->IsIngame())
	{
		for (uint64_t i = 0; i < pGameManager->GetEntities().GetSize(); i++)
		{
			Engine::Entity* pEntity = pGameManager->GetEntities()[i];

			if (pEntity == nullptr)
				continue;

			pEntity->SpottedEsp();
		}
	}

	return CallWindowProcW((WNDPROC)Hook::oriWndProc, hWnd, uMsg, wParam, lParam);
}

void Init()
{
	//AllocConsole();
	//freopen("CON", "w", stdout);
	//SetConsoleTitleA(XorStr("Project NoEye"));

	Hook::oriWndProc = SetWindowLongPtr(FindWindowA(NULL, XorStr("Rainbow Six")), GWLP_WNDPROC, (LONG_PTR)Hook::WndProc);

	MessageBoxA(0, XorStr("Interrogation in progress..."), XorStr("Shhhhh"), MB_SYSTEMMODAL);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Init), NULL, NULL, NULL);
		break;

	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

