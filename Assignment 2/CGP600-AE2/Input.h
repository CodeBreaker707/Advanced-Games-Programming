#pragma once

#include <dinput.h>

class Input
{
private:
	IDirectInput8* m_direct_input;
	IDirectInputDevice8* m_keyboard_device;
	IDirectInputDevice8* m_mouse_device;

	long prev_mouse_x = 0;
	long prev_mouse_y = 0;

	long mouse_x = 0;
	long mouse_y = 0;

	bool ButtonPushedOnce;

	unsigned char m_keyboard_keys_state[256];

public:

	DIMOUSESTATE m_mouse_state;

	unsigned char mve_frwd;
	unsigned char mve_lft;
	unsigned char mve_bck;
	unsigned char mve_rght;
	unsigned char jump;
	unsigned char sprint;
	unsigned char pick_up;
	unsigned char drop;
	unsigned char interact;
	unsigned char swapCamera;

	BYTE attack;

	Input(HINSTANCE hInst, HWND hWnd);
	~Input();

	HRESULT InitialiseInput(HINSTANCE hInst, HWND hWnd);

	void ReadInputStates();

	bool IsKeyPressed(unsigned char DI_Keycode);
	bool IsKeyPressedOnce(unsigned char DI_Keycode);
	bool IsKeyReleased(unsigned char DI_Keycode);

	void MouseBehaviour();

	bool IsMouseMoving();
	bool IsMouseMovingRight();
	bool IsMouseMovingUp();

};