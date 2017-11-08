#pragma once

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>

class SystemManager
{

private:

	HINSTANCE m_hInst = NULL;
	HWND m_hWnd;

	WNDCLASSEX wcex = { 0 };

	float m_win_width = 1280.0f;
	float m_win_height = 768.0f;

	// Window Name
	char g_CGP600_Assignment_2[100] = "CGP600 Assignment 2\0";

public:

	SystemManager();

	HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
	static LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);

	HWND GetWindow();

};
