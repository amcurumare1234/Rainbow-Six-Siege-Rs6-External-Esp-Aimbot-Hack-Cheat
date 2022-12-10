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
#include "NetworkManager.h"
#include "InterfaceManager.h"

void* TeleportTemp;

namespace Engine
{
	NetworkManager* NetworkManager::GetInstance()
	{
		return *reinterpret_cast<NetworkManager**>(ADDRESS_NETWORKMANAGER + reinterpret_cast<uint64_t>(GetModuleHandle(NULL)));
	}

	void NetworkManager::Teleport(float xpos, float ypos, float zpos) {
		auto NetManager = *Memory::Ptr<void**>(this, 0xB0);
		auto NetManager2 = *Memory::Ptr<void**>(NetManager, 0x8);
		auto NetManager3 = *Memory::Ptr<void**>(NetManager2, 0x118);

		auto pGameManager = Engine::GameManager::GetInstance();
		auto LPManager = pGameManager->GetLocalPlayer();

		auto LocalPos = LPManager->GetPosition();

		for (int i = 0; i < 400; i++) {
			NetObject CurObj = *Memory::Ptr<NetObject*>(NetManager3, (i * 0x0080));

			if (LocalPos.z == CurObj.Z) {
				CurObj.X = xpos;
				CurObj.Z = zpos + 0.1;
				CurObj.Y = ypos;

				for (int i2 = 0; i2 < 800; i2++) {
					NetObject CurObj1 = *Memory::Ptr<NetObject*>(NetManager3, (i * 0x0080));
					if (!(zpos == CurObj1.Z)) {
						Vector3* pos = Memory::Ptr<Vector3*>(NetManager3, (i * 0x0080));
						*pos = Vector3(xpos, ypos, zpos);
						Sleep(1);
					}
					else {
						continue;
					}
				}
				return;
			}
		}
	}

	uintptr_t* NetworkManager::GetAddress(uintptr_t address)
	{
		return *reinterpret_cast<uintptr_t**>(address + reinterpret_cast<uint64_t>(GetModuleHandle(NULL)));
	}
}