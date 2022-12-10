//          _                          _          __ ___   ___   ___  
//    /\   | |                        | |        /_ |__ \ / _ \ / _ \ 
//   /  \  | | _____  ____ _ _ __   __| | ___ _ __| |  ) | | | | (_) |
//  / /\ \ | |/ _ \ \/ / _` | '_ \ / _` |/ _ \ '__| | / /| | | |> _ < 
// / ____ \| |  __/>  < (_| | | | | (_| |  __/ |  | |/ /_| |_| | (_) |
///_/    \_\_|\___/_/\_\__,_|_| |_|\__,_|\___|_|  |_|____|\___/ \___/ 

#include "stdafx.h"
#include "Hooks.h"
#include "Helpers.h"
#include "CCheat.h"
#include "Renderer.h"
#include "UserInterface.h"

tD3D11Present Hooks::oPresent = NULL;
bool bOnce = false;
HRESULT __stdcall Hooks::hkD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!bOnce)
	{
		HWND hWindow = FindWindowA(NULL, "Rainbow Six");
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)&CCheat::pDevice)))
		{
			pSwapChain->GetDevice(__uuidof(CCheat::pDevice), (void**)&CCheat::pDevice);
			CCheat::pDevice->GetImmediateContext(&CCheat::pContext);
		}

		ID3D11Texture2D* renderTargetTexture = nullptr;
		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&renderTargetTexture))))
		{
			CCheat::pDevice->CreateRenderTargetView(renderTargetTexture, NULL, &CCheat::pRenderTargetView);
			renderTargetTexture->Release();
		}
	
		ImGui_ImplDX11_Init(hWindow, CCheat::pDevice, CCheat::pContext);
		ImGui_ImplDX11_CreateDeviceObjects();

		bOnce = true;
	}

	CCheat::pContext->OMSetRenderTargets(1, &CCheat::pRenderTargetView, NULL);

	ImGui_ImplDX11_NewFrame();

	Renderer::GetInstance()->BeginScene();
	Renderer::GetInstance()->DrawScene();
	Renderer::GetInstance()->EndScene();

	UserInterface::GetInstance()->Render();

	ImGui::Render();

	return Hooks::oPresent(pSwapChain, SyncInterval, Flags);
}