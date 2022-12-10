/**
* Rainbow Six: Siege SDK
* Copyright (C) 2017 RangeMachine
*/

#pragma once

#include "Array.h"
#include "Entity.h"

namespace Engine
{
	class StatusManager
	{
	public:
		static StatusManager* GetInstance();
		bool IsIngame();
	};
}