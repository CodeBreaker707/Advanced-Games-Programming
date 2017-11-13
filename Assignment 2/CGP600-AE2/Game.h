#pragma once

#include <iostream>
#include "Renderer.h"
#include "Asset.h"
#include "Camera.h"
#include "Input.h"


class Game
{

private:
	Renderer* m_render_target;

	Input* key;

	Camera* perspective;

	Asset* player;
	Asset* tree;

	float yaw_degrees;

public:
	Game(HINSTANCE hInstance, int nCmdShow);

	void MainUpdate();
};
