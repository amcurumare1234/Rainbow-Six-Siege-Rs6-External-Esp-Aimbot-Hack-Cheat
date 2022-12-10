/**
* Rainbow Six: Siege SDK
* Copyright (C) 2017 RangeMachine
*/

#include <windows.h>

#include "Offsets.h"
#include "Memory.h"
#include "StatusManager.h"

namespace Engine
{

	StatusManager* StatusManager::GetInstance()
	{
		return *reinterpret_cast<StatusManager**>(ADDRESS_STATUSMANAGER + reinterpret_cast<uint64_t>(GetModuleHandle(NULL)));
	}

	bool StatusManager::IsIngame()
	{
		auto p0 = *Memory::Ptr<int*>(this, OFFSET_ISINGAME);

		if (p0 == 1)
			return true;
		else
			return false;
	}
}