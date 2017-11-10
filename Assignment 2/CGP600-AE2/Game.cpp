#include "Game.h"

Game::Game(HINSTANCE hInstance, int nCmdShow)
{
	m_render_target = new Renderer(hInstance, nCmdShow);

	perspective = new Camera(0.0f, 0.0f, -0.5f, 0.0f);

	player = new Asset(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext());

	player->LoadObjModel("Assets/Sphere.obj", "Assets/tile.bmp");

}

void Game::MainUpdate()
{
	m_render_target->ClearBuffers();

	player->Draw(&perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());

	m_render_target->Display();
}