#include "stdafx.h"
#include "Globals.h"
#include "Utility.h"
#include "Memory.h"
#include "Overlay.h"
#include <iostream>
namespace Global {
	HANDLE GameHandle = 0x0;

	LPVOID BaseAddress = 0;

	std::string LocalName = "Adınız:";

	Mem Memory = Mem();

	Overlay Over = Overlay();
}
int main()
{
	if (Global::GameHandle == INVALID_HANDLE_VALUE || Global::GameHandle == NULL || Global::GameHandle == (HANDLE)0x0) {
		std::cout << "R6 bulunamıyor." << std::endl;
		system("pause");
		return 1;
	}
	Global::Memory.SetBaseAddress();
	std::cout << "Base adresi: " << std::hex << Global::Memory.GetBaseAddress() << std::dec << std::endl;
	Global::Over.SetupWindow();
	std::cout << "Pencere modu" << std::endl;
	std::cout << "Local adınız: " << Global::LocalName << std::endl;
	Global::Over.Loop();
	system("pause");
	return EXIT_SUCCESS;
	return 0;
}

