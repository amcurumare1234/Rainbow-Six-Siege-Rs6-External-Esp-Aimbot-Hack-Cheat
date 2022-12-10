//          _                          _          __ ___   ___   ___  
//    /\   | |                        | |        /_ |__ \ / _ \ / _ \ 
//   /  \  | | _____  ____ _ _ __   __| | ___ _ __| |  ) | | | | (_) |
//  / /\ \ | |/ _ \ \/ / _` | '_ \ / _` |/ _ \ '__| | / /| | | |> _ < 
// / ____ \| |  __/>  < (_| | | | | (_| |  __/ |  | |/ /_| |_| | (_) |
///_/    \_\_|\___/_/\_\__,_|_| |_|\__,_|\___|_|  |_|____|\___/ \___/ 

#include "Accuracy.h"
#include "Offsets.h"
#include "Vectors.h"
#include "GameManager.h"
#include "InterfaceManager.h"
#include "GameRenderer.h"
#include "WeaponManager.h"
#include "NetworkManager.h"
#include "StatusManager.h"
#include <windows.h>
#include "Offsets.h"
#include "Memory.h"
#include "Camera.h"

constexpr float r2d = 57.2957795131f;
constexpr float d2r = 0.01745329251f;
#define PI 3.14159265358979323846f

Accuracy* Accuracy::m_pInstance;

Vector3 selectedPos;

Engine::Entity * GetClosestFOVPlayerTeleport()
{
	auto pGameManager = Engine::GameManager::GetInstance();
	auto pCameraManager = Engine::GameRenderer::GetInstance()->GetCamera();
	ImGuiIO& io = ImGui::GetIO();

	int target = -1;
	float lastDistance = 360.0f;

	for (int i = 0; i < pGameManager->GetEntities().GetSize(); i++)
	{
		auto pEntity = pGameManager->GetEntities()[i];

		if (pEntity != nullptr)
		{
			auto iHealth = pEntity->GetHealth();

			if (iHealth > 0 && iHealth < 200)
			{
				auto fEntityPosition = pEntity->GetHead();

				if (fEntityPosition.x == 0.0f && fEntityPosition.y == 0.0f && fEntityPosition.z == 0.0f)
					return nullptr;

				auto fEntityPositionScreen = pCameraManager->WorldToScreen(fEntityPosition);

				if (fEntityPositionScreen.x == 0.0f && fEntityPositionScreen.y == 0.0f)
					return nullptr;

				if (fEntityPositionScreen.z >= 1.0f)
				{
					auto x = fEntityPositionScreen.x - io.DisplaySize.x / 2;
					auto y = fEntityPositionScreen.y - io.DisplaySize.y / 2;

					auto distanceFromCrosshair = (float)sqrt((x * x) + (y * y));

					if (distanceFromCrosshair <= 360.0f && distanceFromCrosshair <= lastDistance) {
						lastDistance = distanceFromCrosshair;
						target = i;
						Settings::teleportTarget = target;
						selectedPos = pGameManager->GetEntities()[i]->GetPosition();
					}
				}
			}
		}
	}
	return pGameManager->GetEntities()[target];
}

float W2SDistance(Vector3 position)
{
	ImGuiIO& io = ImGui::GetIO();
	auto pCameraManager = Engine::GameRenderer::GetInstance()->GetCamera();
	Vector3 v = pCameraManager->WorldToScreen(position);
	return (abs(v.x - (io.DisplaySize.x / 2)) + abs(v.y - (io.DisplaySize.y / 2)));
}

Vector3 VectorAngles(Vector3 forward)
{
	Vector3 angles;
	float tmp, yaw, pitch;

	if (forward.y == 0 && forward.y == 0)
	{
		yaw = 0;
		if (forward.y > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (float)(atan2(forward.y, forward.x) * 180 / PI);


		tmp = (float)sqrt(forward.x * forward.x + forward.y * forward.y);
		pitch = (float)(atan2(forward.z * -1, tmp) * 180 / PI);

	}

	yaw += 90;

	if (yaw > 180)
	{
		yaw -= 360;
	}
	if (pitch > 180)
	{
		pitch -= 360;
	}

	angles.x = pitch;
	angles.y = yaw;
	angles.z = 0;
	return angles;
}

Vector3 ToEuler(Vector4 q)
{
	Vector3 end = Vector3();

	float sinr = (float)(+2.0 * (q.w * q.x + q.y * q.z));
	float cosr = (float)(+1.0 - 2.0 * (q.x * q.x + q.y * q.y));
	end.z = (float)atan2(sinr, cosr);

	double sinp = +2.0 * (q.w * q.y - q.z * q.x);
	if (abs(sinp) >= 1)
		end.x = (float)copysign(PI / 2, sinp);
	else
		end.x = (float)asin(sinp);

	double siny = +2.0 * (q.w * q.z + q.x * q.y);
	double cosy = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
	end.y = (float)atan2(siny, cosy);

	return end;
}

float GetDifference(float firstAngle, float secondAngle)
{
	float difference = secondAngle - firstAngle;
	while (difference < -180.f) difference += 360.f;
	while (difference > 180.f) difference -= 360.f;
	return difference;
}

Vector4 QuaternionFromYPR(float yaw, float pitch, float roll)
{
	Vector4 q;
	double cy = cos(yaw * 0.5);
	double sy = sin(yaw * 0.5);
	double cr = cos(pitch * 0.5);
	double sr = sin(pitch * 0.5);
	double cp = cos(roll * 0.5);
	double sp = sin(roll * 0.5);

	q.w = (float)(cy * cr * cp + sy * sr * sp);
	q.x = (float)(cy * sr * cp - sy * cr * sp);
	q.y = (float)(cy * cr * sp + sy * sr * cp);
	q.z = (float)(sy * cr * cp - cy * sr * sp);
	return q;
}

Vector4 GetAngle(Vector3 viewTranslation, Vector3 enemyHead, Vector4 originalAngle)
{
	Vector3 aimAngle = VectorAngles(viewTranslation - enemyHead);
	Vector3 currentAngle = ToEuler(originalAngle);
	currentAngle *= (float)(180.f / PI);
	Vector3 currentAngleV = Vector3(currentAngle.z, currentAngle.y, currentAngle.x);

	Vector3 smoothedAngle = Vector3(aimAngle.x, aimAngle.y, aimAngle.z);
	smoothedAngle.x = GetDifference(currentAngleV.x, smoothedAngle.x);
	smoothedAngle.y = GetDifference(currentAngleV.y, smoothedAngle.y);
	smoothedAngle *= (Settings::aimSmooth * 0.1f);
	currentAngleV.x += smoothedAngle.x;
	currentAngleV.y += smoothedAngle.y;

	if (currentAngleV.x > 89.0f && currentAngleV.x <= 180.0f)
	{
		currentAngleV.x = 89.0f;
	}
	while (currentAngleV.x > 180.f)
	{
		currentAngleV.x -= 360.f;
	}
	while (currentAngleV.x < -89.0f)
	{
		currentAngleV.x = -89.0f;
	}
	while (currentAngleV.y > 180.f)
	{
		currentAngleV.y -= 360.f;
	}
	while (currentAngleV.y < -180.f)
	{
		currentAngleV.y += 360.f;
	}

	aimAngle *= (float)(PI / 180.f);
	currentAngle *= (float)(PI / 180.f);
	currentAngleV *= (float)(PI / 180.f);

	Vector4 newQuaternion = QuaternionFromYPR(currentAngleV.y, currentAngleV.x, 0);
	return newQuaternion;
}

Accuracy::Accuracy()
{
}

Accuracy::~Accuracy()
{
}

void Accuracy::AccuracyMain()
{
	auto pStatusManager = Engine::StatusManager::GetInstance();
	if (pStatusManager->IsInGame()) {
		Accuracy::DoAimbot();
		Accuracy::DoNoRecoil();
		Accuracy::DoNoSpread();
		Accuracy::DoTrigger();
		Accuracy::DoTeleport();
	}
	else {
		Settings::teleportTarget = -1;
		Settings::aimbotTarget = -1;
	}
}

bool isShooting = false;

void Accuracy::DoTrigger()
{
	if (Settings::bAccuracyTrigger) {
		auto pWeaponManager = Engine::WeaponManager::GetInstance();
		if (pWeaponManager->CrosshairID() == 1) {
			if (pWeaponManager->FireState() == 0) {
				pWeaponManager->ForceFire(1);
				isShooting = true;
			}
			else {
				pWeaponManager->ForceFire(1);
				Sleep(1);
				pWeaponManager->ForceFire(0);
			}
		}
		else {
			if (pWeaponManager->FireMode() == 0 && isShooting) {
				pWeaponManager->ForceFire(0);
				isShooting = false;
			}
		}
	}
}

void Accuracy::DoAimbot()
{
	int target = -1;

	float fBestDistanceFromCross = FLT_MAX;
	float fTargetDistance = FLT_MAX;

	auto pGameManager = Engine::GameManager::GetInstance();
	auto pCameraManager = Engine::GameRenderer::GetInstance()->GetCamera();

	if (Settings::AimbotKey == 1) {
		if (Settings::bAccuracyAimbot && GetAsyncKeyState(VK_CAPITAL))
		{
			auto lEntity = pGameManager->GetLocalPlayer();
			if (lEntity->GetHealth() > 0) {
				auto iLocalTeam = pGameManager->GetLocalPlayer()->GetTeam();
				for (int i = 0; i < pGameManager->GetEntities().GetSize(); i++)
				{
					auto pEntity = pGameManager->GetEntities()[i];
					int iPlayerHealth = pEntity->GetHealth();

					if (iPlayerHealth > 0)
					{
						int iEntityTeam = pEntity->GetTeam();

						if (iEntityTeam != iLocalTeam)
						{
							auto vCurrentEntityHeadPos = pEntity->GetHead();
							auto vCurrentEntityWSPos = pCameraManager->WorldToScreen(vCurrentEntityHeadPos);
							auto vCurrentEntityPos = pEntity->GetPosition();
							auto vCurrentLocalPos = pGameManager->GetLocalPlayer()->GetPosition();
							auto fCurrentDistanceFromCross = W2SDistance(vCurrentEntityHeadPos);
							auto fCurrentDistance = pCameraManager->GetViewTranslation().Distance(vCurrentEntityHeadPos);
							auto fzDifference = vCurrentLocalPos.z - vCurrentEntityPos.z;
							auto fDistance = vCurrentLocalPos.Distance(vCurrentEntityPos);

							if (fDistance < Settings::aimDist)
							{
								if (fCurrentDistance < 2 && vCurrentEntityWSPos.z >= 1.f && fzDifference < 1.5f && fzDifference > -1.5f)
								{
									if (fCurrentDistance < fTargetDistance)
									{
										fBestDistanceFromCross = fCurrentDistanceFromCross;
										fTargetDistance = fCurrentDistance;
										target = i;
									}
								}
								else if (fCurrentDistanceFromCross <= Settings::aimFov && fCurrentDistanceFromCross >= -(Settings::aimFov) && vCurrentEntityWSPos.z >= 1.f)
								{
									if (vCurrentEntityHeadPos.y > 1 || vCurrentEntityHeadPos.y < -1)
									{
										if (fCurrentDistanceFromCross < fBestDistanceFromCross)
										{
											if (fCurrentDistance < fTargetDistance)
											{
												fBestDistanceFromCross = fCurrentDistanceFromCross;
												fTargetDistance = fCurrentDistance;
												target = i;
											}
										}
									}
								}
							}
						}
					}
				}
				if (target != -1) {
					Vector3 headPos = pGameManager->GetEntities()[target]->GetHead();

					Vector4 qAngles = pGameManager->GetLocalPlayer()->GetViewAngles();
					Vector3 ViewTranslation = pCameraManager->GetViewTranslation();
					Vector4 qCAngles = GetAngle(ViewTranslation, headPos, qAngles);

					pGameManager->GetLocalPlayer()->SetViewAngles(qCAngles);

					Settings::aimbotTarget = target;
					fTargetDistance = FLT_MAX;
					fBestDistanceFromCross = FLT_MAX;
					target = NULL;
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	else if (Settings::AimbotKey == 2) {
		if (Settings::bAccuracyAimbot && (GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			auto lEntity = pGameManager->GetLocalPlayer();
			if (lEntity->GetHealth() > 0) {
				auto iLocalTeam = pGameManager->GetLocalPlayer()->GetTeam();
				for (int i = 0; i < pGameManager->GetEntities().GetSize(); i++)
				{
					auto pEntity = pGameManager->GetEntities()[i];
					int iPlayerHealth = pEntity->GetHealth();

					if (iPlayerHealth > 0)
					{
						int iEntityTeam = pEntity->GetTeam();

						if (iEntityTeam != iLocalTeam)
						{
							auto vCurrentEntityHeadPos = pEntity->GetHead();
							auto vCurrentEntityWSPos = pCameraManager->WorldToScreen(vCurrentEntityHeadPos);
							auto vCurrentEntityPos = pEntity->GetPosition();
							auto vCurrentLocalPos = pGameManager->GetLocalPlayer()->GetPosition();
							auto fCurrentDistanceFromCross = W2SDistance(vCurrentEntityHeadPos);
							auto fCurrentDistance = pCameraManager->GetViewTranslation().Distance(vCurrentEntityHeadPos);
							auto fzDifference = vCurrentLocalPos.z - vCurrentEntityPos.z;
							auto fDistance = vCurrentLocalPos.Distance(vCurrentEntityPos);

							if (fDistance < Settings::aimDist)
							{
								if (fCurrentDistance < 2 && vCurrentEntityWSPos.z >= 1.f && fzDifference < 1.5f && fzDifference > -1.5f)
								{
									if (fCurrentDistance < fTargetDistance)
									{
										fBestDistanceFromCross = fCurrentDistanceFromCross;
										fTargetDistance = fCurrentDistance;
										target = i;
									}
								}
								else if (fCurrentDistanceFromCross <= Settings::aimFov && fCurrentDistanceFromCross >= -(Settings::aimFov) && vCurrentEntityWSPos.z >= 1.f)
								{
									if (vCurrentEntityHeadPos.y > 1 || vCurrentEntityHeadPos.y < -1)
									{
										if (fCurrentDistanceFromCross < fBestDistanceFromCross)
										{
											if (fCurrentDistance < fTargetDistance)
											{
												fBestDistanceFromCross = fCurrentDistanceFromCross;
												fTargetDistance = fCurrentDistance;
												target = i;
											}
										}
									}
								}
							}
						}
					}
				}
				if (target != -1) {
					Vector3 headPos = pGameManager->GetEntities()[target]->GetHead();

					Vector4 qAngles = pGameManager->GetLocalPlayer()->GetViewAngles();
					Vector3 ViewTranslation = pCameraManager->GetViewTranslation();
					Vector4 qCAngles = GetAngle(ViewTranslation, headPos, qAngles);

					pGameManager->GetLocalPlayer()->SetViewAngles(qCAngles);

					Settings::aimbotTarget = target;
					fTargetDistance = FLT_MAX;
					fBestDistanceFromCross = FLT_MAX;
					target = NULL;
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	else if (Settings::AimbotKey == 3) {
		if (Settings::bAccuracyAimbot && (GetKeyState(VK_RBUTTON) & 0x100) != 0)
		{
			auto lEntity = pGameManager->GetLocalPlayer();
			if (lEntity->GetHealth() > 0) {
				auto iLocalTeam = pGameManager->GetLocalPlayer()->GetTeam();
				for (int i = 0; i < pGameManager->GetEntities().GetSize(); i++)
				{
					auto pEntity = pGameManager->GetEntities()[i];
					int iPlayerHealth = pEntity->GetHealth();

					if (iPlayerHealth > 0)
					{
						int iEntityTeam = pEntity->GetTeam();

						if (iEntityTeam != iLocalTeam)
						{
							auto vCurrentEntityHeadPos = pEntity->GetHead();
							auto vCurrentEntityWSPos = pCameraManager->WorldToScreen(vCurrentEntityHeadPos);
							auto vCurrentEntityPos = pEntity->GetPosition();
							auto vCurrentLocalPos = pGameManager->GetLocalPlayer()->GetPosition();
							auto fCurrentDistanceFromCross = W2SDistance(vCurrentEntityHeadPos);
							auto fCurrentDistance = pCameraManager->GetViewTranslation().Distance(vCurrentEntityHeadPos);
							auto fzDifference = vCurrentLocalPos.z - vCurrentEntityPos.z;
							auto fDistance = vCurrentLocalPos.Distance(vCurrentEntityPos);

							if (fDistance < Settings::aimDist)
							{
								if (fCurrentDistance < 2 && vCurrentEntityWSPos.z >= 1.f && fzDifference < 1.5f && fzDifference > -1.5f)
								{
									if (fCurrentDistance < fTargetDistance)
									{
										fBestDistanceFromCross = fCurrentDistanceFromCross;
										fTargetDistance = fCurrentDistance;
										target = i;
									}
								}
								else if (fCurrentDistanceFromCross <= Settings::aimFov && fCurrentDistanceFromCross >= -(Settings::aimFov) && vCurrentEntityWSPos.z >= 1.f)
								{
									if (vCurrentEntityHeadPos.y > 1 || vCurrentEntityHeadPos.y < -1)
									{
										if (fCurrentDistanceFromCross < fBestDistanceFromCross)
										{
											if (fCurrentDistance < fTargetDistance)
											{
												fBestDistanceFromCross = fCurrentDistanceFromCross;
												fTargetDistance = fCurrentDistance;
												target = i;
											}
										}
									}
								}
							}
						}
					}
				}
				if (target != -1) {
					Vector3 headPos = pGameManager->GetEntities()[target]->GetHead();

					Vector4 qAngles = pGameManager->GetLocalPlayer()->GetViewAngles();
					Vector3 ViewTranslation = pCameraManager->GetViewTranslation();
					Vector4 qCAngles = GetAngle(ViewTranslation, headPos, qAngles);

					pGameManager->GetLocalPlayer()->SetViewAngles(qCAngles);

					Settings::aimbotTarget = target;
					fTargetDistance = FLT_MAX;
					fBestDistanceFromCross = FLT_MAX;
					target = NULL;
				}
			}
		}
	}
}

void Accuracy::DoNoSpread()
{
	if (Settings::bAccuracyNoSpread) {
		auto pWeaponManager = Engine::WeaponManager::GetInstance();
		pWeaponManager->NoSpread();
	}
}

void Accuracy::DoNoRecoil()
{
	if (Settings::bAccuracyNoRecoil) {
		auto pWeaponManager = Engine::WeaponManager::GetInstance();
		pWeaponManager->NoRecoil();
	}
}

void Accuracy::DoTeleport()
{
	if (Settings::bTeleport) {
		if (GetAsyncKeyState(VK_F1)) {
			GetClosestFOVPlayerTeleport();
		}

		if (GetAsyncKeyState(VK_F2)) {
			if (Settings::teleportTarget != -1) {
				auto pGameManager = Engine::GameManager::GetInstance();
				auto pHealth = pGameManager->GetEntities()[Settings::teleportTarget]->GetHealth();
				auto lHealth = pGameManager->GetLocalPlayer()->GetHealth();

				if (pHealth > 0 && pHealth < 200 &&
					lHealth > 0 && lHealth < 200) {
					auto pNetworkManager = Engine::NetworkManager::GetInstance();

					pNetworkManager->Teleport(selectedPos.x, selectedPos.y, selectedPos.z);
				}
			}
		}
	}
}

Accuracy* Accuracy::GetInstance()
{
	if (!m_pInstance)
		m_pInstance = new Accuracy();

	return m_pInstance;
}