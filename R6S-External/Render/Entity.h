//          _                          _          __ ___   ___   ___  
//    /\   | |                        | |        /_ |__ \ / _ \ / _ \ 
//   /  \  | | _____  ____ _ _ __   __| | ___ _ __| |  ) | | | | (_) |
//  / /\ \ | |/ _ \ \/ / _` | '_ \ / _` |/ _ \ '__| | / /| | | |> _ < 
// / ____ \| |  __/>  < (_| | | | | (_| |  __/ |  | |/ /_| |_| | (_) |
///_/    \_\_|\___/_/\_\__,_|_| |_|\__,_|\___|_|  |_|____|\___/ \___/ 

#pragma once
#include "Vectors.h"
#include "stdafx.h"

namespace Engine
{
	class Entity
	{
	public:
		Vector3 GetPosition();
		Vector3 GetNeck();
		Vector3 GetHead();
		//Vector3 GetRightHand();
		//Vector3 GetLeftHand();
		INT8 GetTeam();
		std::string GetName();
		float GetHealth();
		void SpottedEsp();
		BYTE IsFlashed();
		Vector4 GetViewAngles();
		void SetViewAngles(Vector4 angle);
		float GetDistance(float Xc, float Yy, float xX, float yY);
	};
}