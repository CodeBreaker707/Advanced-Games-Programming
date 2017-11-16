#include "Game.h"

Game::Game(HINSTANCE hInstance, int nCmdShow)
{
	m_render_target = new Renderer(hInstance, nCmdShow);

	key = new Input(hInstance, m_render_target->GetWindow());

	perspective = new Camera(0.0f, 0.0f, -0.5f, 0.0f);
	
	player = new Player(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), 0.0f, 0.0f, 10.0f);
	tree = new Asset();

	tree->InitialiseAsset(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), 5.0f, 0.0f, 20.0f);

	//player->LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");
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
			player->MoveAsset(0.0f, 0.0f, 0.001);
			perspective->Move(0.001f, 0.0f);
		}
		if (key->IsKeyPressed(key->mve_lft))
		{
			player->MoveAsset(-0.001, 0.0f, 0.0f);
			perspective->Move(0.0f, 0.001f);
		}
		if (key->IsKeyPressed(key->mve_bck))
		{
			player->MoveAsset(0.0f, 0.0f, -0.001);
			perspective->Move(-0.001f, 0.0f);
		}
		if (key->IsKeyPressed(key->mve_rght))
		{
			player->MoveAsset(0.001, 0.0f, 0.0f);
			perspective->Move(0.0f, -0.001f);
		}
		if (key->IsKeyPressed(DIK_UP))
		{
			player->MoveAsset(0.0f, 0.001, 0.0f);
		}
		if (key->IsKeyPressed(DIK_DOWN))
		{
			player->MoveAsset(0.0f, -0.001, 0.0f);
		}
	

	player->CheckCollision(tree);

	player->UpdatePos();
	
	
	// Mouse Controls

	if (key->IsMouseMoving() == true)
	{

		if (key->IsMouseMovingRight() == true)
		{
			yaw_degrees += 0.5f;

			perspective->YawRotate(yaw_degrees);

		}
		else if (key->IsMouseMovingRight() == false)
		{
			yaw_degrees -= 0.5f;

			perspective->YawRotate(yaw_degrees);

		}

		/*if (key->IsMouseMovingUp() == true)
		{
			pitch_degrees -= 0.5f;

			perspective->PitchRotate(pitch_degrees);

		}
		else if (key->IsMouseMovingUp() == false)
		{
			pitch_degrees += 0.5f;

			perspective->PitchRotate(pitch_degrees);

		}*/

	}

	
	

	player->Draw(&perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());
	tree->Draw(&perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());


	m_render_target->Display();
}