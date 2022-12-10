/**
 * Rainbow Six: Siege SDK
 * Copyright (C) 2017 RangeMachine
 */

#include "Entity.h"
#include "Offsets.h"
#include "Memory.h"

namespace Engine
{


	void Entity::SpottedEsp()
	{
		auto EI = *Memory::Ptr<void**>(this, OFFSET_ENTITY_ENTITYINFO);

		auto MC = *Memory::Ptr<void**>(EI, OFFSET_ENTITYINFO_MAINCOMPONENT);

		auto EC1 = *Memory::Ptr<void**>(MC, OFFSET_MAINCOMPONENT_ESPCHAIN1);

		auto EC2 = *Memory::Ptr<void**>(EC1, OFFSET_ESPCHAIN1_ESPCHAIN2);

		int& EC3 = *Memory::Ptr<int*>(EC2, OFFSET_ESPCHAIN2_ESPCHAIN3);

		if (EC3 == 0 || EC3 == 1)
		{
			EC3 = 1;
			//WPM<int>(espChain2 + OFFSET_ESPCHAIN2_ESPCHAIN3, 1);
		}
	}
}