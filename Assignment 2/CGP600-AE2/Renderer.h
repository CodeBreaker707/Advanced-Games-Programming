#pragma once

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>

class Renderer
{
private:

	HINSTANCE m_hInst = NULL;

	WNDCLASSEX wcex = { 0 };

	D3D_DRIVER_TYPE         m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           m_pD3DDevice = NULL;
	ID3D11DeviceContext*    m_pImmediateContext = NULL;
	IDXGISwapChain*         m_pSwapChain = NULL;
	ID3D11RenderTargetView* m_pBackBufferRTView = NULL;
	ID3D11DepthStencilView* m_pZBuffer;
	ID3D11BlendState* m_pAlphaBlendEnable; // 14
	ID3D11BlendState* m_pAlphaBlendDisable; // 14

	float m_win_width;
	float m_win_height;

	// Window Name
	char g_CGP600_Assignment_2[100] = "CGP600 Assignment 2\0";

	//float rgba_clear_colour[4] = { 1.0f, 0.74f, 1.0f, 1.0f };
	//float rgba_clear_colour[4];

public:

	Renderer(HINSTANCE hInstance, int nCmdShow);
	~Renderer();

	static LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);

	HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);

	HRESULT InitialiseD3D();

	void ClearBuffers();
	void Display();

	void SetBlendState(bool state);

	float GetWindowWidth();
	float GetWindowHeight();

	void ShutdownD3D();

	HWND GetWindow();

	ID3D11Device* GetD3DDevice();
	ID3D11DeviceContext* GetDeviceContext();
};
