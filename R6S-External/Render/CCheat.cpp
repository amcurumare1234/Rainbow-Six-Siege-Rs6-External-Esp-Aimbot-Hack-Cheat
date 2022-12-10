//          _                          _          __ ___   ___   ___  
//    /\   | |                        | |        /_ |__ \ / _ \ / _ \ 
//   /  \  | | _____  ____ _ _ __   __| | ___ _ __| |  ) | | | | (_) |
//  / /\ \ | |/ _ \ \/ / _` | '_ \ / _` |/ _ \ '__| | / /| | | |> _ < 
// / ____ \| |  __/>  < (_| | | | | (_| |  __/ |  | |/ /_| |_| | (_) |
///_/    \_\_|\___/_/\_\__,_|_| |_|\__,_|\___|_|  |_|____|\___/ \___/ 

#include "stdafx.h"
#include "Settings.h"

ID3D11Device *CCheat::pDevice = NULL;
ID3D11DeviceContext *CCheat::pContext = NULL;
IDXGISwapChain* CCheat::pSwapChain = NULL;
ID3D11RenderTargetView* CCheat::pRenderTargetView = NULL;

void CCheat::Initialise()
{
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	HWND hWindow = FindWindowA(NULL, "Rainbow Six");

#pragma region Initialise DXGI_SWAP_CHAIN_DESC
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	ImGuiIO& io = ImGui::GetIO();

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.OutputWindow = hWindow;
	scd.SampleDesc.Count = 1;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; 
	scd.BufferDesc.Width = 1920;
	scd.BufferDesc.Height = 1080;
	scd.BufferDesc.RefreshRate.Numerator = 165; 
	scd.BufferDesc.RefreshRate.Denominator = 1; 
#pragma endregion

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		NULL, &featureLevel, 1, D3D11_SDK_VERSION,
		&scd, &CCheat::pSwapChain,
		&CCheat::pDevice, NULL, &CCheat::pContext
	)))
	{
		return;
	}

	DWORD_PTR*  pSwapChainVT = reinterpret_cast<DWORD_PTR*>(CCheat::pSwapChain);
	DWORD_PTR*  pDeviceVT = reinterpret_cast<DWORD_PTR*>(CCheat::pDevice);
	DWORD_PTR*  pContextVT = reinterpret_cast<DWORD_PTR*>(CCheat::pContext);

	pSwapChainVT = reinterpret_cast<DWORD_PTR*>(pSwapChainVT[0]);
	pDeviceVT = reinterpret_cast<DWORD_PTR*>(pDeviceVT[0]);
	pContextVT = reinterpret_cast<DWORD_PTR*>(pContextVT[0]);

	Renderer::GetInstance()->Initialize();
	UserInterface::GetInstance()->SetStyle();
	Input::GetInstance()->StartThread();

	Hooks::oPresent = reinterpret_cast<tD3D11Present>(pSwapChainVT[8]);

	Helpers::HookFunction(reinterpret_cast<PVOID*>(&Hooks::oPresent), Hooks::hkD3D11Present);
}