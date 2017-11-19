#include "Game.h"

Game::Game(HINSTANCE hInstance, int nCmdShow)
{
	m_render_target = new Renderer(hInstance, nCmdShow);

	key = new Input(hInstance, m_render_target->GetWindow());

	perspective = new Camera(2.0f, 0.0f, 0.5f, 0.0f);
	
	player = new Player(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), 2.0f, 0.0f, 10.0f);

	ground = new Statik(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), 0.0f, -2.0f, 0.0f);

	tree[0] = new Statik(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), 5.0f, 0.0f, 20.0f);
	tree[1] = new Statik(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), 2.0f, 0.0f, 25.0f);

	ground->LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");

	objs.push_back(ground);

	tree[0]->LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");
	tree[1]->LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");

	objs.push_back(tree[0]);
	objs.push_back(tree[1]);

	ground->ScaleAsset(100.0f, 0.0f, 100.0f);

}

void Game::MainUpdate()
{
	m_render_target->ClearBuffers();

	key->ReadInputStates();
	key->MouseBehaviour();

	// Keyboard Controls
	
		if (key->IsKeyPressed(key->mve_frwd))
		{
			player->MoveAsset(0.0f, 0.0f, player->GetPlayerMoveSpeed());

			
			if (perspective->GetCollidingState() == false)
			{
				perspective->Move(0.0f, 0.0f, player->GetPlayerMoveSpeed());
			}
		}
		if (key->IsKeyPressed(key->mve_lft))
		{
			player->MoveAsset(-player->GetPlayerMoveSpeed(), 0.0f, 0.0f);

			if (perspective->GetCollidingState() == false)
			{
				perspective->Move(-player->GetPlayerMoveSpeed(), 0.0f, 0.0f);
			}
		}
		if (key->IsKeyPressed(key->mve_bck))
		{
			player->MoveAsset(0.0f, 0.0f, -player->GetPlayerMoveSpeed());

			if (perspective->GetCollidingState() == false)
			{
				perspective->Move(0.0f, 0.0f, -player->GetPlayerMoveSpeed());
			}
		}
		if (key->IsKeyPressed(key->mve_rght))
		{
			player->MoveAsset(player->GetPlayerMoveSpeed(), 0.0f, 0.0f);

			if (perspective->GetCollidingState() == false)
			{
				perspective->Move(player->GetPlayerMoveSpeed(), 0.0f, 0.0f);
			}
		}
		if (key->IsKeyPressed(DIK_SPACE) && player->GetOnGroundState() == true)
		{
			player->SetOnGroundState(false);
			player->SetJumpState(true);
			
			if (perspective->GetCollidingState() == false)
			{
				//perspective->Move(0.0f, 0.001f, 0.0f);
			}
		}
		/*if (key->IsKeyPressed(DIK_DOWN))
		{
			player->MoveAsset(0.0f, -0.001, 0.0f);
			
			if (perspective->GetCollidingState() == false)
			{
				perspective->Move(0.0f, -0.001f, 0.0f);
			}

		}*/

		player->OnAirBehaviour();

		for (int i = 0; i < objs.size(); i++)
		{
			objs[i]->CheckCollision(player);
		}

		for (int i = 0; i < objs.size(); i++)
		{
			if (objs[i]->IsColliding() == true)
			{
				perspective->SetCollidingState(true);
				break;
			}
			else
			{
				perspective->SetCollidingState(false);
			}
		}

		for (int i = 0; i < objs.size(); i++)
		{
			player->RestrictPos(objs[i]->IsColliding());
		}

		for (int i = 0; i < objs.size(); i++)
		{
			player->UpdatePos(objs[i]->IsColliding());
		}

		for (int i = 0; i < objs.size(); i++)
		{
			if (objs[i]->IsColliding() == true)
			{
				player->SetOnGroundState(true);
				break;
			}
		}

		count = 0;

		for (int i = 0; i < objs.size(); i++)
		{
			if (objs[i]->IsColliding() == false)
			{
				count += 1;
			}
		}

		if (count == objs.size() && player->GetJumpState() == false)
		{
			player->PullDown();	
		}

		

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

	ground->Draw(&perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());

	tree[0]->Draw(&perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());
	tree[1]->Draw(&perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());


	m_render_target->Display();
}