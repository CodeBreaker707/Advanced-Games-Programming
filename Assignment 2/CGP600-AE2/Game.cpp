#include "Game.h"

Game::Game(HINSTANCE hInstance, int nCmdShow)
{
	m_render_target = new Renderer(hInstance, nCmdShow);

	key = new Input(hInstance, m_render_target->GetWindow());

	perspective = new Camera(0.0f, 0.0f, -0.5f, 0.0f);
	
	player = new Asset(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), 0.0f, 0.0f, 10.0f);
	tree = new Asset(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), 5.0f, 0.0f, 20.0f);

	player->LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");
	tree->LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");

}

void Game::MainUpdate()
{
	m_render_target->ClearBuffers();

	key->ReadInputStates();
	key->MouseBehaviour();

	

	// Keyboard Controls
	
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
		if (key->IsKeyPressed(DIK_UP))
		{
			player->Jump(0.001);
		}
		if (key->IsKeyPressed(DIK_DOWN))
		{
			player->Jump(-0.001);
		}
	

	player->CheckCollision(tree);

	if (player->IsColliding() == true)
	{
		player->RestrictPos();
	}
	
	// Mouse Controls

	if (key->IsMouseMoving() == true)
	{

		if (key->IsMouseMovingRight() == true)
		{
			yaw_degrees += 0.1f;

			perspective->YawRotate(yaw_degrees);

		}
		else if (key->IsMouseMovingRight() == false)
		{
			yaw_degrees -= 0.1f;

			perspective->YawRotate(yaw_degrees);

		}

		if (key->IsMouseMovingUp() == true)
		{
			pitch_degrees -= 0.1f;

			perspective->PitchRotate(pitch_degrees);

		}
		else if (key->IsMouseMovingUp() == false)
		{
			pitch_degrees += 0.1f;

			perspective->PitchRotate(pitch_degrees);

		}
	}

	
	

	player->Draw(&perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());
	tree->Draw(&perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());


	m_render_target->Display();
}