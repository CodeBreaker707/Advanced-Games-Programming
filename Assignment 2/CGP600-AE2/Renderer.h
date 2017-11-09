#pragma once

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>

class Renderer
{
private:

	D3D_DRIVER_TYPE         m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           m_pD3DDevice = NULL;
	ID3D11DeviceContext*    m_pImmediateContext = NULL;
	IDXGISwapChain*         m_pSwapChain = NULL;
	ID3D11RenderTargetView* m_pBackBufferRTView = NULL;

public:

	Renderer();

	HRESULT InitialiseD3D(HWND wnd);

	void RenderFrame();

	void ShutdownD3D();
};
