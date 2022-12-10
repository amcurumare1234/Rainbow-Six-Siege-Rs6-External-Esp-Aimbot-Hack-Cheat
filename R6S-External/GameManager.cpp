/**
 * Rainbow Six: Siege SDK
 * Copyright (C) 2017 RangeMachine
 */

#include <windows.h>

#include "GameManager.h"
#include "Offsets.h"
#include "Memory.h"

namespace Engine
{
	Array<Entity*> GameManager::GetEntities()
	{
		return *Memory::Ptr<Array<Entity*>*>(this, OFFSET_GAMEMANAGER_ENTITYLIST);
	}

	GameManager* GameManager::GetInstance()
	{
		return *reinterpret_cast<GameManager**>(ADDRESS_GAMEMANAGER + reinterpret_cast<uint64_t>(GetModuleHandle(NULL)));
	}
}