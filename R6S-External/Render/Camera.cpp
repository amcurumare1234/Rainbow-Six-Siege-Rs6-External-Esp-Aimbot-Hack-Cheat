//          _                          _          __ ___   ___   ___  
//    /\   | |                        | |        /_ |__ \ / _ \ / _ \ 
//   /  \  | | _____  ____ _ _ __   __| | ___ _ __| |  ) | | | | (_) |
//  / /\ \ | |/ _ \ \/ / _` | '_ \ / _` |/ _ \ '__| | / /| | | |> _ < 
// / ____ \| |  __/>  < (_| | | | | (_| |  __/ |  | |/ /_| |_| | (_) |
///_/    \_\_|\___/_/\_\__,_|_| |_|\__,_|\___|_|  |_|____|\___/ \___/ 

#include "Camera.h"
#include "Offsets.h"
#include "Memory.h"
#include "ImGui/imgui.h"

namespace Engine
{
	Vector3 Camera::GetViewRight()
	{
		if (!Memory::IsValidPtr<Camera>(this))
			return *(new Vector3());

		return *Memory::Ptr<Vector3*>(this, OFFSET_CAMERA_VIEWRIGHT);
	}

	Vector3 Camera::GetViewUp()
	{
		if (!Memory::IsValidPtr<Camera>(this))
			return *(new Vector3());

		return *Memory::Ptr<Vector3*>(this, OFFSET_CAMERA_VIEWUP);
	}

	Vector3 Camera::GetViewForward()
	{
		if (!Memory::IsValidPtr<Camera>(this))
			return *(new Vector3());

		return *Memory::Ptr<Vector3*>(this, OFFSET_CAMERA_VIEFORWARD);
	}

	Vector3 Camera::GetViewTranslation()
	{
		if (!Memory::IsValidPtr<Camera>(this))
			return *(new Vector3());

		return *Memory::Ptr<Vector3*>(this, OFFSET_CAMERA_VIEWTRANSLATION);
	}

	float Camera::GetViewFovX()
	{
		if (!Memory::IsValidPtr<Camera>(this))
			return 0.0f;

		return *Memory::Ptr<float*>(this, OFFSET_CAMERA_VIEWFOVX);
	}

	float Camera::GetViewFovY()
	{
		if (!Memory::IsValidPtr<Camera>(this))
			return 0.0f;

		return *Memory::Ptr<float*>(this, OFFSET_CAMERA_VIEWFOVY);
	}

	Vector3 Camera::WorldToScreen(Vector3 position)
	{
		if (!Memory::IsValidPtr<Camera>(this))
			return *(new Vector3());

		Vector3 temp = position - GetViewTranslation();

		float x = temp.Dot(GetViewRight());
		float y = temp.Dot(GetViewUp());
		float z = temp.Dot(GetViewForward() * -1);

		ImGuiIO& io = ImGui::GetIO();

		return *(new Vector3(
			(io.DisplaySize.x / 2) * (1 + x / GetViewFovX() / z),
			(io.DisplaySize.y / 2) * (1 - y / GetViewFovY() / z),
			z));
	}
}