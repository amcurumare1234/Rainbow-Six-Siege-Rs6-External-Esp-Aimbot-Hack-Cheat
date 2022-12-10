//          _                          _          __ ___   ___   ___  
//    /\   | |                        | |        /_ |__ \ / _ \ / _ \ 
//   /  \  | | _____  ____ _ _ __   __| | ___ _ __| |  ) | | | | (_) |
//  / /\ \ | |/ _ \ \/ / _` | '_ \ / _` |/ _ \ '__| | / /| | | |> _ < 
// / ____ \| |  __/>  < (_| | | | | (_| |  __/ |  | |/ /_| |_| | (_) |
///_/    \_\_|\___/_/\_\__,_|_| |_|\__,_|\___|_|  |_|____|\___/ \___/ 

#pragma once
#include "stdafx.h"
#include "Entity.h"
#include "GameManager.h"
#include "GameRenderer.h"
#include "Settings.h"
#include "Renderer.h"

class Accuracy
{
public:
	void DoNoSpread();
	void DoNoRecoil();
	void DoAimbot();
	static void DoTrigger();
	void AccuracyMain();
	void DoTeleport();
	static Accuracy* GetInstance();

private:
	Accuracy();
	~Accuracy();

	static Accuracy* m_pInstance;
};