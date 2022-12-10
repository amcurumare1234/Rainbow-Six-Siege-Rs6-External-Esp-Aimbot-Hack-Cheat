//          _                          _          __ ___   ___   ___  
//    /\   | |                        | |        /_ |__ \ / _ \ / _ \ 
//   /  \  | | _____  ____ _ _ __   __| | ___ _ __| |  ) | | | | (_) |
//  / /\ \ | |/ _ \ \/ / _` | '_ \ / _` |/ _ \ '__| | / /| | | |> _ < 
// / ____ \| |  __/>  < (_| | | | | (_| |  __/ |  | |/ /_| |_| | (_) |
///_/    \_\_|\___/_/\_\__,_|_| |_|\__,_|\___|_|  |_|____|\___/ \___/ 

#include "GadgetManager.h"
#include "StatusManager.h"
#include "GameManager.h"
#include "GameRenderer.h"
#include "Camera.h"
#include <sstream>                                                                     
                                                                    
namespace Engine
{
	GadgetManager* GadgetManager::GetInstance() {
		return *reinterpret_cast<GadgetManager**>(ADDRESS_NETWORKMANAGER + reinterpret_cast<uint64_t>(GetModuleHandle(nullptr)));
	}

	Array<Gadget*> GadgetManager::GetGadgetList()
	{
		auto ptr1 = *Engine::Memory::Ptr<void**>(this, 0x350);
		auto ptr2 = *Engine::Memory::Ptr<void**>(ptr1, 0x18);
		return *Memory::Ptr<Array<Gadget*>*>(ptr2, 0x100);
	}

	std::string GadgetManager::GetGadgetName(Gadget* pGadget)
	{
		std::string name = XorStr("Unknown");

		if ((pGadget->type == 0x10702400) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 15) && (pGadget->pDetails->type2 == 0xbe04cfbe))
			name = XorStr("Drone");
		else if ((pGadget->type == 0x10780400) && (pGadget->state == 0x2e84000) && (pGadget->pDetails->type1 == 4) && (pGadget->pDetails->type2 == 0xbcda3eb4))
			name = XorStr("Flashbang");
		else if ((pGadget->type == 0x10f02480) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 12) && (pGadget->pDetails->type2 == 0xbda55196))
			name = XorStr("C4");
		else if ((pGadget->type == 0x10480400) && (pGadget->state == 0xe81000) && (pGadget->pDetails->type1 == 7) && (pGadget->pDetails->type2 == 0xbf3fffa4))
			name = XorStr("Shield");
		else if ((pGadget->type == 0x10a86400) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 6) && (pGadget->pDetails->type2 == 0xbf2a1ab6))
			name = XorStr("Frostmat");
		else if ((pGadget->type == 0x10b02480) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 14) && (pGadget->pDetails->type2 == 0xbd75c28f))
			name = XorStr("Blackeye");
		else if ((pGadget->type == 0x10b84480) && (pGadget->state == 0x8e84000) && (pGadget->pDetails->type1 == 9) && (pGadget->pDetails->type2 == 0xc0243063))
			name = XorStr("Kapkan-Trap");
		else if ((pGadget->type == 0x10e86480) && (pGadget->state == 0xe81000) && (pGadget->pDetails->type1 == 12) && (pGadget->pDetails->type2 == 0xbe1d9359))
			name = XorStr("Camera");
		else if ((pGadget->type == 0x10702400) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 14) && (pGadget->pDetails->type2 == 0xbe03c799))
			name = XorStr("Twitch-Drone");
		else if ((pGadget->type == 0x10706400) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 8) && (pGadget->pDetails->type2 == 0xbe7c74fd))
			name = XorStr("Breaching-Charge");
		else if ((pGadget->type == 0x10702400) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 9) && (pGadget->pDetails->type2 == 0xbde60584))
			name = XorStr("Fuze-Charge");
		else if ((pGadget->type == 0x10706400) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 8) && (pGadget->pDetails->type2 == 0xbe99b404))
			name = XorStr("Thermite-Charge");
		else if ((pGadget->type == 0x10784400) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 6) && (pGadget->pDetails->type2 == 0xbe08380e))
			name = XorStr("Claymore");
		else if ((pGadget->type == 0x11502489) && (pGadget->state == 0x6e84000) && (pGadget->pDetails->type1 == 17) && (pGadget->pDetails->type2 == 0xbdcccccd))
			name = XorStr("Gu-Mine");
		else if ((pGadget->type == 0x10b02400) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 10) && (pGadget->pDetails->type2 == 0xbdf5c28f))
			name = XorStr("Hibana-Pellet");
		else if ((pGadget->type == 0x10780400) && (pGadget->state == 0x4e84000) && (pGadget->pDetails->type1 == 7) && (pGadget->pDetails->type2 == 0xbe888180))
			name = XorStr("Defuser Dropped");
		else if ((pGadget->type == 0x10780400) && (pGadget->state == 0x4e84000) && (pGadget->pDetails->type1 == 7) && (pGadget->pDetails->type2 == 0xbe7fdb00))
			name = XorStr("Defuser Planted");
		else if ((pGadget->type == 0x10b02480) && (pGadget->state == 0x4e84000) && (pGadget->pDetails->type1 == 9) && (pGadget->pDetails->type2 == 0xbe8f18c2))
			name = XorStr("Battery");
		else if ((pGadget->type == 0x10f02480) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 13) && (pGadget->pDetails->type2 == 0xbe4c7ba0))
			name = XorStr("Magpie");
		else if ((pGadget->type == 0x10b82400) && (pGadget->state == 0x4e84000) && (pGadget->pDetails->type1 == 8) && (pGadget->pDetails->type2 == 0xbed45cb0))
			name = XorStr("Armor");
		else if ((pGadget->type == 0x10b02480) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 13) && (pGadget->pDetails->type2 == 0xbd5ea2ad))
			name = XorStr("Grzmot-Mine");
		else if ((pGadget->type == 0x10b02480) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 15) && (pGadget->pDetails->type2 == 0xbe8e3587))
			name = XorStr("Jammer");
		else if ((pGadget->type == 0x11102489) && (pGadget->state == 0x4e84000) && (pGadget->pDetails->type1 == 20) && (pGadget->pDetails->type2 == 0xbe051eb8))
			name = XorStr("Yokai");
		else if ((pGadget->type == 0x10f02480) && (pGadget->state == 0x4e84000) && (pGadget->pDetails->type1 == 12) && (pGadget->pDetails->type2 == 0xbd37e96a))
			name = XorStr("Gas");
		else if ((pGadget->type == 0x10B02480) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 13) && (pGadget->pDetails->type2 == 0xbecccccd))
			name = XorStr("Clone");
		else if ((pGadget->type == 0x10484400) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 7) && (pGadget->pDetails->type2 == 0xbf3b69cd))
			name = XorStr("Mira");
		else if ((pGadget->type == 0x10780400) && (pGadget->state == 0x2e84000) && (pGadget->pDetails->type1 == 5) && (pGadget->pDetails->type2 == 0xbd229da1))
			name = XorStr("EMP");
		else if ((pGadget->type == 0x10780400) && (pGadget->state == 0x2e84000) && (pGadget->pDetails->type1 == 4) && (pGadget->pDetails->type2 == 0xbd17bd90))
			name = XorStr("Grenade");
		else if ((pGadget->type == 0x10702400) && (pGadget->state == 0xe90000) && (pGadget->pDetails->type1 == 7) && (pGadget->pDetails->type2 == 0xbecca78e))
			name = XorStr("Turret");
		else if ((pGadget->type == 0x10480400) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 5) && (pGadget->pDetails->type2 == 0xbf3d0099))
			name = XorStr("Castle-Single-Door");
		else if ((pGadget->type == 0x10480400) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 5) && (pGadget->pDetails->type2 == 0xbfa85839))
			name = XorStr("Castle-Double-Door");
		else if ((pGadget->type == 0x10480400) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 5) && (pGadget->pDetails->type2 == 0xbf32b969))
			name = XorStr("Castle-Single-Window");
		else if ((pGadget->type == 0x10480400) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 5) && (pGadget->pDetails->type2 == 0xbf8e6224))
			name = XorStr("Castle-Double-Window");
		else if ((pGadget->type == 0x11702480) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 8) && (pGadget->pDetails->type2 == 0xbeb45f59))
			name = XorStr("Heartbeat-Scanner");
		else if ((pGadget->type == 0x10f84480) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 7) && (pGadget->pDetails->type2 == 0xbf800000))
			name = XorStr("Phone");
		else if ((pGadget->type == 0x10780400) && (pGadget->state == 0x6e84000) && (pGadget->pDetails->type1 == 5) && (pGadget->pDetails->type2 == 0xbd0192a9))
			name = XorStr("Smoke-Grenade");
		else if ((pGadget->type == 0x11302480) && (pGadget->state == 0x8e90000) && (pGadget->pDetails->type1 == 21) && (pGadget->pDetails->type2 == 0xbe8175d6))
			name = XorStr("Evileye");
		else if ((pGadget->type == 0x11302400) && (pGadget->state == 0xe84000) && (pGadget->pDetails->type1 == 9) && (pGadget->pDetails->type2 == 0xbdadf6c0))
			name = XorStr("Iq-Scanner");
		else if ((pGadget->type == 0x10b04400) && (pGadget->state == 0xe90000) && (pGadget->pDetails->type1 == 8) && (pGadget->pDetails->type2 == 0xbe800000))
			name = XorStr("Reinforced-Camera1");
		/*else if ((pGadget->type == 0x10b02480 state == 0xe84000 pDetails->type1 == 13 pDetails->type2 == 0xbe800000))
			name = XorStr("Reinforced-Camera2");*/

		return name;
	}

	void GadgetManager::DrawGadgets()
	{
		auto pStatusManager = Engine::StatusManager::GetInstance();
		auto pCameraManager = Engine::GameRenderer::GetInstance()->GetCamera();
		if (pStatusManager->IsInGame()) {
			if (Settings::bGadget) {

				if (this->GetGadgetList()[0] == nullptr)
					return;

				for (uint64_t i = 0; i < this->GetGadgetList().GetSize(); i++)
				{
					Engine::Gadget* pGadget = this->GetGadgetList()[i];
					float distance = pCameraManager->GetViewTranslation().Distance(pGadget->pos);
					if (pGadget == nullptr)
						continue;

					if (pGadget->pDetails == nullptr)
						continue;

					std::string name = this->GetGadgetName(pGadget);

					if (!strcmp(name.c_str(), XorStr("Unknown")))
						continue;

					auto pCameraManager = Engine::GameRenderer::GetInstance()->GetCamera();
					Vector3 w2s_pos = pCameraManager->WorldToScreen(pGadget->pos);

					if (w2s_pos.z <= 0.5f)
						continue;

					Renderer::GetInstance()->DrawOutlinedText(Renderer::GetInstance()->m_pDefault, name, ImVec2(w2s_pos.x, w2s_pos.y), 12.f, 0xFFFBAE00, true);
				}
			}
			if (Settings::bGadgetDebug) {

				if (this->GetGadgetList()[0] == nullptr)
					return;

				for (uint64_t i = 0; i < this->GetGadgetList().GetSize(); i++)
				{
					Engine::Gadget* pGadget = this->GetGadgetList()[i];
					float distance = pCameraManager->GetViewTranslation().Distance(pGadget->pos);
					if (pGadget == nullptr)
						continue;

					if (pGadget->pDetails == nullptr)
						continue;

					auto pCameraManager = Engine::GameRenderer::GetInstance()->GetCamera();
					Vector3 w2s_pos = pCameraManager->WorldToScreen(pGadget->pos);

					if (w2s_pos.z <= 0.5f)
						continue;

					std::string nameDebug;
					nameDebug += "";
					std::stringstream type_str_str;
					type_str_str << std::hex << pGadget->type;
					nameDebug += type_str_str.str();

					nameDebug += ";";
					std::stringstream state_str_str;
					state_str_str << std::hex << pGadget->state;
					nameDebug += state_str_str.str();

					nameDebug += ";";
					nameDebug += std::to_string(pGadget->pDetails->type1);

					nameDebug += ";";
					std::stringstream type2_str_str;
					type2_str_str << std::hex << pGadget->pDetails->type2;
					nameDebug += type2_str_str.str();
					float fDebug = rand() % 30;

					if (distance < 5.0f) {
					Renderer::GetInstance()->DrawOutlinedText(Renderer::GetInstance()->m_pDefault, nameDebug, ImVec2(w2s_pos.x, w2s_pos.y-fDebug), 12.f, 0xFFFBAE00, true);
					}
				}
			}
		}
	}
}

// Gadget offsets (All tested)
//Drone: type: 0x10702400 state: 0xe84000 type1: 15 type2: 0xbe04cfbe
//Flashbang: type: 0x10780400 state: 0x2e84000 type1: 4 type2: 0xbcda3eb4
//C4: type: 0x10f02480 state: 0xe84000 type1: 12 type2: 0xbda55196
//Shield: type: 0x10480400 state: 0xe81000 type1: 7 type2: 0xbf3fffa4
//Frostmat: type: 0x10a86400 state: 0xe84000 type1: 6 type2: 0xbf2a1ab6
//Blackeye: type: 0x10b02480 state: 0xe84000 type1: 14 type2: 0xbd75c28f
//Kapkan - Trap: type: 0x10b84480 state: 0x8e84000 type1: 9 type2: 0xc0243063
//Camera: type: 0x10e86480 state: 0xe81000 type1: 12 type2: 0xbe1d9359
//Twitch - Drone: type: 0x10702400 state: 0xe84000 type1: 14 type2: 0xbe03c799
//Breaching - Charge: type: 0x10706400 state: 0xe84000 type1: 8 type2: 0xbe7c74fd
//Fuze - Charge: type: 0x10702400 state: 0xe84000 type1: 9 type2: 0xbde60584
//Thermite - Charge: type: 0x10706400 state: 0xe84000 type1: 8 type2: 0xbe99b404
//Claymore: type: 0x10784400 state: 0xe84000 type1: 6 type2: 0xbe08380e
//Gu - Mine: type: 0x11502489 state: 0x6e84000 type1: 17 type2: 0xbdcccccd
//Hibana - Pellet: type: 0x10b02400 state: 0xe84000 type1: 10 type2: 0xbdf5c28f
//Defuser Dropped: type: 0x10780400 state: 0x4e84000 type1: 7 type2: 0xbe888180
//Defuser Planted: type: 0x10780400 state: 0x4e84000 type1: 7 type2: 0xbe7fdb00
//Battery: type: 0x10b02480 state: 0x4e84000 type1: 9 type2: 0xbe8f18c2
//Magpie: type: 0x10f02480 state: 0xe84000 type1: 13 type2: 0xbe4c7ba0
//Armor: type: 0x10b82400 state: 0x4e84000 type1: 8 type2: 0xbed45cb0
//Grzmot - Mine: type: 0x10b02480 state: 0xe84000 type1: 13 type2: 0xbd5ea2ad
//Jammer: type: 0x10b02480 state: 0xe84000 type1: 15 type2: 0xbe8e3587
//Yokai: type: 0x11102489 state: 0x4e84000 type1: 20 type2: 0xbe051eb8
//Gas: type: 0x10f02480 state: 0x4e84000 type1: 12 type2: 0xbd37e96a
//Clone: type: 0x10884409 state: 0x40881000 type1: 10 type2: 0xbea0d690
//Mira: type: 0x10484400 state: 0xe84000 type1: 7 type2: 0xbf3b69cd
//EMP: type: 0x10780400 state: 0x2e84000 type1: 5 type2: 0xbd229da1
//Grenade: type: 0x10780400 state: 0x2e84000 type1: 4 type2: 0xbd17bd90
//Turret: type: 0x10702400 state: 0xe90000 type1: 7 type2: 0xbecca78e
//Castle - Single - Door: type: 0x10480400 state: 0xe84000 type1: 5 type2: 0xbf3d0099
//Castle - Double - Door: type: 0x10480400 state: 0xe84000 type1: 5 type2: 0xbfa85839
//Castle - Single - Window: type: 0x10480400 state: 0xe84000 type1: 5 type2: 0xbf32b969
//Castle - Double - Window: type: 0x10480400 state: 0xe84000 type1: 5 type2: 0xbf8e6224
//Heartbeat-Scanner: type: 0x11702480: 0xe84000 type1: 8 type2: 0xbeb45f59
//Dokkaebi-Dropped-Phone: type: 0x10f84480 state: 0xe84000 type1: 7 type2: 0xbf800000
//Smoke - Grenade: type: 0x10780400 state: 0x6e84000 type1: 5 type2: 0xbd0192a9
//Evileye: type: 0x11302480 state: 0x8e90000 type1: 21 type2: 0xbe8175d6
//Iq-Scanner: type: 0x11302400 state: 0xe84000 type1: 9 type2: 0xbdadf6c0
//Reinforced-Camera: type: 0x10b04400 state: 0xe90000 type1: 8 type2: 0xbe800000
