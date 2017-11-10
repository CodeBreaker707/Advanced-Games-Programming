#pragma once

#include "Renderer.h"
#include "Asset.h"
#include "Camera.h"

class Game
{

private:
	Renderer* m_render_target;

	Camera* perspective;

	Asset* player;

public:
	Game(HINSTANCE hInstance, int nCmdShow);

	void MainUpdate();
};
