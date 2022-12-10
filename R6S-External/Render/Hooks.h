//          _                          _          __ ___   ___   ___  
//    /\   | |                        | |        /_ |__ \ / _ \ / _ \ 
//   /  \  | | _____  ____ _ _ __   __| | ___ _ __| |  ) | | | | (_) |
//  / /\ \ | |/ _ \ \/ / _` | '_ \ / _` |/ _ \ '__| | / /| | | |> _ < 
// / ____ \| |  __/>  < (_| | | | | (_| |  __/ |  | |/ /_| |_| | (_) |
///_/    \_\_|\___/_/\_\__,_|_| |_|\__,_|\___|_|  |_|____|\___/ \___/ 

#pragma once
#include "stdafx.h"

// DEFINITIONS

typedef HRESULT(__stdcall *tD3D11Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__stdcall *tD3D11DrawIndexed)(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void(__stdcall *tD3D11CreateQuery)(ID3D11Device* pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery);
typedef void(__stdcall *tD3D11EndScene)(ID3D11Device* pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery);

namespace Hooks
{
	//ORIGINAL FUNCTIONS

	extern tD3D11CreateQuery oCreateQuery;
	extern tD3D11Present oPresent;
	extern tD3D11DrawIndexed oDrawIndexed;

	//HOOK FUNCTIONS

	HRESULT hkD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
}