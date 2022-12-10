#pragma once
#include <Windows.h>
#include <string>
#include "Memory.h"
#include "Overlay.h"
namespace Global {
	extern HANDLE GameHandle;
	extern LPVOID BaseAddress;
	extern std::string LocalName;
	extern Mem Memory;
	extern Overlay Over;
}