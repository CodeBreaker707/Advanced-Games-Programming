#include "Game.h"

Game::Game(HINSTANCE hInstance, int nCmdShow)
{
	m_render_target = new Renderer(hInstance, nCmdShow);

	key = new Input(hInstance, m_render_target->GetWindow());

	perspective = new Camera(0.0f, 0.0f, -0.5f, 0.0f);

	player = new Asset(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext());
	tree = new Asset(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext());

	player->LoadObjModel("Assets/Sphere.obj", "Assets/tile.bmp");
	tree->LoadObjModel("Assets/Sphere.obj", "Assets/tile.bmp");

}

void Game::MainUpdate()
{
	m_render_target->ClearBuffers();

	key->ReadInputStates();

	if (key->IsKeyPressed(key->mve_frwd))
	{
		player->MoveForward(0.001);
		perspective->Move(0.001f, 0.0f);
	}
	if (key->IsKeyPressed(key->mve_lft))
	{
		player->MoveSideways(-0.001);
		perspective->Move(0.0f, 0.001f);
	}
	if (key->IsKeyPressed(key->mve_bck))
	{
		player->MoveForward(-0.001);
		perspective->Move(-0.001f, 0.0f);
	}
	if (key->IsKeyPressed(key->mve_rght))
	{
		player->MoveSideways(0.001);
		perspective->Move(0.0f, -0.001f);
	}

	if (key->IsMouseUsed())
	{
		yaw_degrees += 0.01f;

		perspective->YawRotate(yaw_degrees);

	}
	else if (!key->IsMouseUsed())
	{
		yaw_degrees -= 0.01f;

		perspective->YawRotate(yaw_degrees);

	}
	

	player->Draw(&perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());
	tree->Draw(&perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());

	m_render_target->Display();
}