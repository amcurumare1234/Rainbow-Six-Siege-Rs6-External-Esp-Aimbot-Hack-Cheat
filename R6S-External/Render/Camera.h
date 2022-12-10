//          _                          _          __ ___   ___   ___  
//    /\   | |                        | |        /_ |__ \ / _ \ / _ \ 
//   /  \  | | _____  ____ _ _ __   __| | ___ _ __| |  ) | | | | (_) |
//  / /\ \ | |/ _ \ \/ / _` | '_ \ / _` |/ _ \ '__| | / /| | | |> _ < 
// / ____ \| |  __/>  < (_| | | | | (_| |  __/ |  | |/ /_| |_| | (_) |
///_/    \_\_|\___/_/\_\__,_|_| |_|\__,_|\___|_|  |_|____|\___/ \___/ 

#pragma  once
#include "Vectors.h"

namespace Engine
{
	class Camera
	{
	public:
		Vector3 GetViewRight();
		Vector3 GetViewUp();
		Vector3 GetViewForward();
		Vector3 GetViewTranslation();
		float GetViewFovX();
		float GetViewFovY();
		Vector3 WorldToScreen(Vector3 position);
	};
}