//          _                          _          __ ___   ___   ___  
//    /\   | |                        | |        /_ |__ \ / _ \ / _ \ 
//   /  \  | | _____  ____ _ _ __   __| | ___ _ __| |  ) | | | | (_) |
//  / /\ \ | |/ _ \ \/ / _` | '_ \ / _` |/ _ \ '__| | / /| | | |> _ < 
// / ____ \| |  __/>  < (_| | | | | (_| |  __/ |  | |/ /_| |_| | (_) |
///_/    \_\_|\___/_/\_\__,_|_| |_|\__,_|\___|_|  |_|____|\___/ \___/ 

#include "Renderer.h"
#include "Entity.h"
#include "GameManager.h"
#include "GameRenderer.h"
#include "GadgetManager.h"
#include "Settings.h"
#include "Rubik.h"
#include "Visuals.h"
#include "Accuracy.h"
#include "Offsets.h"

Renderer* Renderer::m_pInstance;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Initialize()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 16.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	m_pDefault = io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 15.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	m_pTabs = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\trebucbd.ttf", 62.f);
	m_pSubTabs = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\trebuc.ttf", 25.f);
}

void Renderer::BeginScene()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin(XorStr("D3D11_Buffer"), reinterpret_cast<bool*>(true), ImVec2(0, 0), 0.0f, ImGuiWindowFlags_NoTitleBar);

	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
	ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);
}

void Renderer::DrawScene()
{
	if (Settings::bUnload) {
		// KILL STILL RUNNING TREADS
		Input::GetInstance()->StopThread();

		// UNHOOK THE FUNCTIONS
		Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oPresent), Hooks::hkD3D11Present);
	}

	Visuals::GetInstance()->VisualsMain();
	Accuracy::GetInstance()->AccuracyMain();
	Engine::GadgetManager::GetInstance()->DrawGadgets();
}

void Renderer::EndScene()
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->PushClipRectFullScreen();

	ImGui::End();
	ImGui::PopStyleColor();
}

void Renderer::DrawBox(const ImVec2& pos, const ImVec2& headPosition, uint32_t color)
{
	float width = (headPosition.y + 15 - pos.y) / 4.5f;

	DrawLine(ImVec2(pos.x - width, pos.y), ImVec2(pos.x + width, pos.y), color, 1.5f);
	DrawLine(ImVec2(pos.x - width, headPosition.y), ImVec2(pos.x - width, pos.y), color, 1.5f);

	DrawLine(ImVec2(pos.x + width, headPosition.y), ImVec2(pos.x + width, pos.y), color, 1.5f);
	DrawLine(ImVec2(pos.x - width, headPosition.y), ImVec2(pos.x + width, headPosition.y), color, 1.5f);
}

float Renderer::DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, uint32_t color, bool center)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	std::stringstream steam(text);
	std::string line;
	float y = 0.0f;
	int i = 0;

	while (std::getline(steam, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
		if (center)
		{
			window->DrawList->AddText(pFont, size + 0.2f, ImVec2(pos.x - textSize.x / 2.0f - 1, pos.y + textSize.y * i - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
		}
		else
		{
			window->DrawList->AddText(pFont, size + 0.2f, ImVec2(pos.x - 1, pos.y + textSize.y * i - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
		}

		y = pos.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}

float Renderer::DrawText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, uint32_t color, bool center)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	std::stringstream steam(text);
	std::string line;
	float y = 0.0f;
	int i = 0;

	while (std::getline(steam, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
		if (center)
		{
			window->DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
		}
		else
		{
			window->DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
		}

		y = pos.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}

void Renderer::DrawLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddLine(from, to, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), thickness);
}

void Renderer::DrawHealth(const ImVec2& scalepos, const ImVec2& scaleheadPosition, INT8 health, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	uint32_t backcolor = 0xFF555656;
	uint32_t color = 0xFF009B1C;

	float width = (scaleheadPosition.y + 15 - scalepos.y) / 4.5f;
	float healthwidth1 = (scalepos.y - scaleheadPosition.y);
	float healthwidth2 = healthwidth1 / 120;
	float defhealthwidth = healthwidth2 * health;

	DrawLine(ImVec2(scalepos.x - width + 5, scaleheadPosition.y), ImVec2(scalepos.x - width + 5, scalepos.y), backcolor, 2.5f);
	DrawLine(ImVec2(scalepos.x - width + 5, scalepos.y - defhealthwidth), ImVec2(scalepos.x - width + 5, scalepos.y), color, 2.5f);
}

void Renderer::DrawCircle(const ImVec2& position, float radius, uint32_t color, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddCircle(position, radius, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), 64, thickness);
}

void Renderer::DrawCircleScale(const ImVec2& position, float radius, uint32_t color, const ImVec2& scalepos, const ImVec2& scaleheadPosition, float thickness)
{
	float rad = (scaleheadPosition.y + 15 - scalepos.y) / 10.5f;
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddCircle(position, rad, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), 64, thickness);
}

void Renderer::DrawCircleFilled(const ImVec2& position, float radius, uint32_t color)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddCircleFilled(position, radius, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), 64);
}

Renderer* Renderer::GetInstance()
{
	if (!m_pInstance)
		m_pInstance = new Renderer();

	return m_pInstance;
}