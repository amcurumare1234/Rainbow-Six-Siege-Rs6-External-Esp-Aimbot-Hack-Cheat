//          _                          _          __ ___   ___   ___  
//    /\   | |                        | |        /_ |__ \ / _ \ / _ \ 
//   /  \  | | _____  ____ _ _ __   __| | ___ _ __| |  ) | | | | (_) |
//  / /\ \ | |/ _ \ \/ / _` | '_ \ / _` |/ _ \ '__| | / /| | | |> _ < 
// / ____ \| |  __/>  < (_| | | | | (_| |  __/ |  | |/ /_| |_| | (_) |
///_/    \_\_|\___/_/\_\__,_|_| |_|\__,_|\___|_|  |_|____|\___/ \___/ 

#include "Entity.h"
#include "Offsets.h"
#include "Memory.h"

namespace Engine
{
	Vector3 Entity::GetPosition()
	{
		if (!Memory::IsValidPtr<Entity>(this))
			return *(new Vector3());

		return *Memory::Ptr<Vector3*>(this, OFFSET_ENTITY_POSITION);
	}

	Vector3 Entity::GetNeck()
	{
		if (!Memory::IsValidPtr<Entity>(this))
			return *(new Vector3());

		return *Memory::Ptr<Vector3*>(this, OFFSET_ENTITY_NECK);
	}

	Vector3 Entity::GetHead()
	{
		if (!Memory::IsValidPtr<Entity>(this))
			return *(new Vector3());

		return *Memory::Ptr<Vector3*>(this, OFFSET_ENTITY_HEAD);
	}

	//Vector3 Entity::GetRightHand()
	//{
	//	if (!Memory::IsValidPtr<Entity>(this))
	//		return *(new Vector3());

	//	return *Memory::Ptr<Vector3*>(this, OFFSET_ENTITY_RIGHTHAND);
	//}

	//Vector3 Entity::GetLeftHand()
	//{
	//	if (!Memory::IsValidPtr<Entity>(this))
	//		return *(new Vector3());

	//	return *Memory::Ptr<Vector3*>(this, OFFSET_ENTITY_LEFTHAND);
	//}

	INT8 Entity::GetTeam()
	{
		if (!Memory::IsValidPtr<Entity>(this))
			return 0;

		auto pEntityInfo = *Memory::Ptr<void**>(this, OFFSET_ENTITY_PLAYERINFO);

		if (!Memory::IsValidPtr(pEntityInfo))
			return 0;

		auto pMainComponent = *Memory::Ptr<void**>(pEntityInfo, 0x0);

		if (!Memory::IsValidPtr(pMainComponent))
			return 0;

		return *Memory::Ptr<INT8*>(pMainComponent, OFFSET_PLAYERINFO_COMPONENT_TEAM);
	}

	std::string Entity::GetName() {
		if (!Memory::IsValidPtr<Entity>(this))
			return *(new std::string());

		auto pPlayerInfo = *Memory::Ptr<void**>(this, OFFSET_ENTITY_PLAYERINFO);

		if (!Memory::IsValidPtr(pPlayerInfo))
			return *(new std::string());

		auto pPlayerInfoDeref = *Memory::Ptr<void**>(pPlayerInfo, 0x0);

		if (!Memory::IsValidPtr(pPlayerInfoDeref))
			return *(new std::string());

		auto ptr = *Memory::Ptr<void**>(pPlayerInfoDeref, OFFSET_PLAYERINFO_COMPONENT_NAME);

		if (!Memory::IsValidPtr(ptr))
			return *(new std::string());

		return *Memory::Ptr<std::string*>(ptr, 0x0);
	}

	float Entity::GetHealth()
	{
		if (!Memory::IsValidPtr<Entity>(this))
			return 0.0f;

		auto pEntityInfo = *Memory::Ptr<void**>(this, OFFSET_ENTITY_ENTITYINFO);

		if (!Memory::IsValidPtr(pEntityInfo))
			return 0.0f;

		auto pMainComponent = *Memory::Ptr<void**>(pEntityInfo, OFFSET_ENTITYINFO_MAINCOMPONENT);

		if (!Memory::IsValidPtr(pMainComponent))
			return 0.0f;

		auto pChildComponent = *Memory::Ptr<void**>(pMainComponent, OFFSET_MAINCOMPONENT_CHILDCOMPONENT);

		if (!Memory::IsValidPtr(pChildComponent))
			return 0.0f;

		return *Memory::Ptr<INT8 *>(pChildComponent, OFFSET_CHILDCOMPONENT_HEALTH);
	}

	void Entity::SpottedEsp() {
		if (!Engine::Memory::IsValidPtr(this))
			return;

		auto entityInfo = *Engine::Memory::Ptr<void**>(this, OFFSET_ENTITY_ENTITYINFO);
		if (!Engine::Memory::IsValidPtr(entityInfo))
			return;

		auto mainComponent = *Engine::Memory::Ptr<void**>(entityInfo, OFFSET_ENTITYINFO_MAINCOMPONENT);
		if (!Engine::Memory::IsValidPtr(mainComponent))
			return;

		auto espChain1 = *Engine::Memory::Ptr<void**>(mainComponent, OFFSET_MAINCOMPONENT_ESPCHAIN1);
		if (!Engine::Memory::IsValidPtr(espChain1))
			return;

		auto espChain2 = *Engine::Memory::Ptr<void**>(espChain1, OFFSET_ESPCHAIN1_ESPCHAIN2);
		if (!Engine::Memory::IsValidPtr(espChain2))
			return;

		int* espChain3 = Engine::Memory::Ptr<int*>(espChain2, OFFSET_ESPCHAIN2_ESPCHAIN3);

		*espChain3 = 1;
	}

	Vector4 Entity::GetViewAngles() {
		auto ViewAngles1 = *Engine::Memory::Ptr<void**>(this, OFFSET_MISC_PLAYER_VIEWANGLE_1);
		auto ViewAngles2 = *Engine::Memory::Ptr<void**>(ViewAngles1, OFFSET_MISC_PLAYER_VIEWANGLE_2);
		return *Engine::Memory::Ptr<Vector4*>(ViewAngles2, OFFSET_MISC_PLAYER_VIEWANGLE_3);
	}

	void Entity::SetViewAngles(Vector4 angle) {
		auto ViewAngles1 = *Engine::Memory::Ptr<void**>(this, OFFSET_MISC_PLAYER_VIEWANGLE_1);
		auto ViewAngles2 = *Engine::Memory::Ptr<void**>(ViewAngles1, OFFSET_MISC_PLAYER_VIEWANGLE_2);
		*Engine::Memory::Ptr<Vector4*>(ViewAngles2, OFFSET_MISC_PLAYER_VIEWANGLE_3) = angle;
	}

	BYTE Entity::IsFlashed() {
		if (!Memory::IsValidPtr<Entity>(this))
			return 0;
		return *Memory::Ptr<BYTE*>(this, 0x350);
	}

	float Entity::GetDistance(float Xx, float Yy, float xX, float yY) {
		return sqrt((yY - Yy) * (yY - Yy) + (xX - Xx) * (xX - Xx));
	}
}