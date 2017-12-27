#pragma once

#include <dinput.h>

class Input
{
private:
	IDirectInput8* m_direct_input;
	IDirectInputDevice8* m_keyboard_device;
	IDirectInputDevice8* m_mouse_device;

	DIMOUSESTATE m_mouse_state;

	long prev_mouse_x = 0;
	long prev_mouse_y = 0;
	
	long mouse_x = 0;
	long mouse_y = 0;

	bool isMouseMoving = false;
	bool isMouseMovingRight = false;
	bool isMouseMovingUp = false;

	unsigned char m_keyboard_keys_state[256];

public:

	unsigned char mve_frwd = DIK_W;
	unsigned char mve_lft = DIK_A;
	unsigned char mve_bck = DIK_S;
	unsigned char mve_rght = DIK_D;

	Input(HINSTANCE hInst, HWND hWnd);
	~Input();

	HRESULT InitialiseInput(HINSTANCE hInst, HWND hWnd);

	void ReadInputStates();

	bool IsKeyPressed(unsigned char DI_Keycode);
	void MouseBehaviour();

	bool IsMouseMoving();
	bool IsMouseMovingRight();
	bool IsMouseMovingUp();

};