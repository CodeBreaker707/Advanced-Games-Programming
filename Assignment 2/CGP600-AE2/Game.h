#pragma once

#include "Renderer.h"
#include "Asset.h"
#include "Camera.h"
#include "Input.h"
#include "Player.h"


class Game
{

private:
	Renderer* m_render_target;

	Input* key;

	Camera* perspective;

	Player* player;
	Asset* tree;

	int prev_mouse_x = 0;
	int prev_mouse_y = 0;

	int mouse_x = 0;
	int mouse_y = 0;

	float yaw_degrees;
	float pitch_degrees;

public:
	Game(HINSTANCE hInstance, int nCmdShow);

	void MainUpdate();
};
