//          _                          _          __ ___   ___   ___  
//    /\   | |                        | |        /_ |__ \ / _ \ / _ \ 
//   /  \  | | _____  ____ _ _ __   __| | ___ _ __| |  ) | | | | (_) |
//  / /\ \ | |/ _ \ \/ / _` | '_ \ / _` |/ _ \ '__| | / /| | | |> _ < 
// / ____ \| |  __/>  < (_| | | | | (_| |  __/ |  | |/ /_| |_| | (_) |
///_/    \_\_|\___/_/\_\__,_|_| |_|\__,_|\___|_|  |_|____|\___/ \___/ 

#include <windows.h>
#include "GameManager.h"
#include "Offsets.h"
#include "Memory.h"
#include "Camera.h"
#include "GameRenderer.h"

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

	//Ghetto local player function
	Entity* GameManager::GetLocalPlayer()
	{
		Entity* pLocalPlayer = NULL;
		auto pGameManager = Engine::GameManager::GetInstance();
		auto pCamera = Engine::GameRenderer::GetInstance()->GetCamera();

		for (uint32_t i = 0; i < pGameManager->GetEntities().GetSize(); i++)
		{
			auto pEntity = pGameManager->GetEntities()[i];

			if (Memory::IsValidPtr<Entity>(pEntity))
			{
				if (pCamera->GetViewTranslation().Distance(pEntity->GetHead()) < 0.1f)
					pLocalPlayer = pEntity;
			}
		}

		return pLocalPlayer;
	}

	uintptr_t* GameManager::GetAddress(uintptr_t address)
	{
		return *reinterpret_cast<uintptr_t**>(address + reinterpret_cast<uint64_t>(GetModuleHandle(NULL)));
	}
}