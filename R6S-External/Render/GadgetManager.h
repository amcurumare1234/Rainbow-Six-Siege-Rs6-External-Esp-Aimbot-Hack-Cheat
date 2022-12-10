//          _                          _          __ ___   ___   ___  
//    /\   | |                        | |        /_ |__ \ / _ \ / _ \ 
//   /  \  | | _____  ____ _ _ __   __| | ___ _ __| |  ) | | | | (_) |
//  / /\ \ | |/ _ \ \/ / _` | '_ \ / _` |/ _ \ '__| | / /| | | |> _ < 
// / ____ \| |  __/>  < (_| | | | | (_| |  __/ |  | |/ /_| |_| | (_) |
///_/    \_\_|\___/_/\_\__,_|_| |_|\__,_|\___|_|  |_|____|\___/ \___/ 

#include "Vectors.h"
#include <vector>
#include "Memory.h"
#include "Offsets.h"
#include <vector>
#include "GameRenderer.h"
#include "Settings.h"
#include "Helpers.h"
#include "XorStr.h"
#include "Array.h"
#include <Windows.h>

namespace Engine
{
	struct GadgetDetails
	{
		char pad_1[0xB0];
		DWORD type1;
		char pad_2[0x10];
		DWORD type2;
	};

	struct Gadget
	{
		char pad_1[0x24];
		DWORD state;
		DWORD64 type;
		char pad_2[0x30];
		Vector3 pos;
		char pad_3[0x14];
		GadgetDetails* pDetails;
	};

	class GadgetManager
	{
	public:

		void DrawGadgets();
		std::string GetGadgetName(Gadget* pGadget);
		Array<Gadget*> GetGadgetList();
		static GadgetManager* GetInstance();
	};
}