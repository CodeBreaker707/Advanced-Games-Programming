#include "Input.h"

Input::Input(HINSTANCE hInst, HWND hWnd)
{
	InitialiseInput(hInst, hWnd);

	mve_frwd = DIK_W;
	mve_lft = DIK_A;
	mve_bck = DIK_S;
	mve_rght = DIK_D;
	jump = DIK_SPACE;
	pick_up = DIK_E;
	interact = DIK_F;
	attack = m_mouse_state.rgbButtons[0];

}

Input::~Input()
{
	m_keyboard_device->Unacquire();
	m_keyboard_device->Release();

	m_mouse_device->Unacquire();
	m_mouse_device->Acquire();

	m_direct_input->Release();
}

HRESULT Input::InitialiseInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// INITIALISING KEYBOARD DEVICE

	ZeroMemory(m_keyboard_keys_state, sizeof(m_keyboard_keys_state));

	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_direct_input, NULL);

	if (FAILED(hr)) return hr;

	hr = m_direct_input->CreateDevice(GUID_SysKeyboard, &m_keyboard_device, NULL);

	if (FAILED(hr)) return hr;

	hr = m_keyboard_device->SetDataFormat(&c_dfDIKeyboard);

	if (FAILED(hr)) return hr;

	hr = m_keyboard_device->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	if (FAILED(hr)) return hr;

	hr = m_keyboard_device->Acquire();

	if (FAILED(hr)) return hr;

	// INITIALISING MOUSE DEVICE

	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_direct_input, NULL);

	if (FAILED(hr)) return hr;

	hr = m_direct_input->CreateDevice(GUID_SysMouse, &m_mouse_device, NULL);

	if (FAILED(hr)) return hr;

	hr = m_mouse_device->SetDataFormat(&c_dfDIMouse);

	if (FAILED(hr)) return hr;

	hr = m_mouse_device->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	if (FAILED(hr)) return hr;

	hr = m_mouse_device->Acquire();

	if (FAILED(hr)) return hr;

	return S_OK;

}

void Input::ReadInputStates()
{
	HRESULT hr;

	// KEYBOARD READ STATE

	hr = m_keyboard_device->GetDeviceState(sizeof(m_keyboard_keys_state), (LPVOID)&m_keyboard_keys_state);

	if (FAILED(hr))
	{
		if (hr == DIERR_INPUTLOST || (hr == DIERR_NOTACQUIRED))
		{
			m_keyboard_device->Acquire();
		}
	}

	// MOUSE READ STATE

	hr = m_mouse_device->GetDeviceState(sizeof(m_mouse_state), (LPVOID)&m_mouse_state);

	if (FAILED(hr))
	{
		if (hr == DIERR_INPUTLOST || (hr == DIERR_NOTACQUIRED))
		{
			m_mouse_device->Acquire();
		}
	}
}

bool Input::IsKeyPressed(unsigned char DI_Keycode)
{
	return m_keyboard_keys_state[DI_Keycode] & 0x80;
}

void Input::MouseBehaviour()
{
	if (mouse_y == prev_mouse_y)
	{
		isMouseMoving = false;
	}
	else
	{
		isMouseMoving = true;
	}

	// Comparing Mouse's X position

	if (mouse_x > prev_mouse_x)
	{

		isMouseMovingRight = true;

		prev_mouse_x = mouse_x;

	}
	else if (mouse_x < prev_mouse_x)
	{
		isMouseMovingRight = false;

		prev_mouse_x = mouse_x;

	}

	if (mouse_y > prev_mouse_y)
	{

		isMouseMovingUp = true;

		prev_mouse_y = mouse_y;

	}
	else if (mouse_y < prev_mouse_y)
	{
		isMouseMovingUp = false;

		prev_mouse_y = mouse_y;

	}

	mouse_x += m_mouse_state.lX;
	//mouse_y += m_mouse_state.lY;

}

bool Input::IsMouseMoving()
{
	return isMouseMoving;
}

bool Input::IsMouseMovingRight()
{
	return isMouseMovingRight;
}

bool Input::IsMouseMovingUp()
{
	return isMouseMovingUp;
}