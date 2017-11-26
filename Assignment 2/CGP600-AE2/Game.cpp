#include "Game.h"

Game::Game(HINSTANCE hInstance, int nCmdShow)
{
	m_render_target = new Renderer(hInstance, nCmdShow);

	key = new Input(hInstance, m_render_target->GetWindow());

	Initialised = false;
	pickedUp = false;

	InitialiseGameAssets();
	

}

void Game::MainUpdate()
{
	m_render_target->ClearBuffers();

	key->ReadInputStates();
	//key->MouseBehaviour();

	m_player_node->m_p_asset->RotateAsset(0.0f, key->m_mouse_state.lX, 0.0f);
	//player->UpdatePosVector();
	//player->UpdateLookAt();

	// Keyboard Controls

	
		if (key->IsKeyPressed(key->mve_frwd))
		{
			//player->MoveAsset(0.0f, 0.0f, player->GetPlayerMoveSpeed());
			m_player_node->m_p_asset->MovePlayer(m_player_node->m_p_asset->GetPlayerMoveSpeed());
			
			if (perspective->GetCollidingState() == false)
			{
				perspective->Move(0.0f, 0.0f, m_player_node->m_p_asset->GetPlayerMoveSpeed());
			}
		}
		if (key->IsKeyPressed(key->mve_lft))
		{
			//player->MoveAsset(-player->GetPlayerMoveSpeed(), 0.0f, 0.0f);
			//player->StrafePlayer(-player->GetPlayerMoveSpeed());

			if (perspective->GetCollidingState() == false)
			{
				//perspective->Move(-player->GetPlayerMoveSpeed(), 0.0f, 0.0f);
			}
		}
		if (key->IsKeyPressed(key->mve_bck))
		{
			//player->MoveAsset(0.0f, 0.0f, -player->GetPlayerMoveSpeed());
			m_player_node->m_p_asset->MovePlayer(-m_player_node->m_p_asset->GetPlayerMoveSpeed());

			if (perspective->GetCollidingState() == false)
			{
				perspective->Move(0.0f, 0.0f, -m_player_node->m_p_asset->GetPlayerMoveSpeed());
			}
		}
		if (key->IsKeyPressed(key->mve_rght))
		{
			//player->MoveAsset(player->GetPlayerMoveSpeed(), 0.0f, 0.0f);
			//player->StrafePlayer(player->GetPlayerMoveSpeed());

			if (perspective->GetCollidingState() == false)
			{
				//perspective->Move(player->GetPlayerMoveSpeed(), 0.0f, 0.0f);
			}
		}
		if (key->IsKeyPressed(key->jump) && m_player_node->m_p_asset->GetOnGroundState() == true)
		{
			m_player_node->m_p_asset->SetOnGroundState(false);
			m_player_node->m_p_asset->SetJumpState(true);
			
		}
		
		m_player_node->m_p_asset->JumpPlayer();

		if (m_player_node->GetChildrenSize() != 0)
		{
			if (key->m_mouse_state.rgbButtons[0] && m_player_node->GetEquippedWeaponNode().m_w_asset->GetWeaponEquipState() == true
				&& m_player_node->GetEquippedWeaponNode().m_w_asset->GetWeaponAttackedState() == false)
			{
				m_player_node->GetEquippedWeaponNode().m_w_asset->SetWeaponAttackedState(true);
				m_player_node->GetEquippedWeaponNode().m_w_asset->SetCurPos();

			}

			if (m_player_node->GetEquippedWeaponNode().m_w_asset->GetWeaponAttackedState() == true)	
			{
				m_player_node->GetEquippedWeaponNode().m_w_asset->MoveAsset(0.0f, 0.0f, cos(m_player_node->GetEquippedWeaponNode().m_w_asset->GetYAngle()) * 0.005f);

				if (m_player_node->GetEquippedWeaponNode().m_w_asset->GetZPos() >
					m_player_node->GetEquippedWeaponNode().m_w_asset->GetCurPos() + (cos(m_player_node->GetEquippedWeaponNode().m_w_asset->GetYAngle()) + 0.3))
				{
					m_player_node->GetEquippedWeaponNode().m_w_asset->SetWeaponAttackedState(false);
				}

			}

			else if (m_player_node->GetEquippedWeaponNode().m_w_asset->GetWeaponAttackedState() == false &&
				m_player_node->GetEquippedWeaponNode().m_w_asset->GetZPos()
				>= m_player_node->GetEquippedWeaponNode().m_w_asset->GetCurPos() + cos(m_player_node->GetEquippedWeaponNode().m_w_asset->GetYAngle()))
			{
				m_player_node->GetEquippedWeaponNode().m_w_asset->MoveAsset(0.0f, 0.0f, cos(m_player_node->GetEquippedWeaponNode().m_w_asset->GetYAngle()) * -0.005f);
			}

		}
		
		
			// Checking collision with all objects against the player

		for (int i = 0; i < objs.size(); i++)
		{
			objs[i]->CheckCollision(m_player_node->m_p_asset);
		}
		

			for (int i = 0; i < m_club_nodes.size(); i++)
			{

				if (m_club_nodes[i]->m_w_asset->IsColliding() == true && key->IsKeyPressed(key->interact))
				{
					m_club_nodes[i]->m_w_asset->SetXPos(1.0f);
					m_club_nodes[i]->m_w_asset->SetYPos(-0.5f);
					m_club_nodes[i]->m_w_asset->SetZPos(1.0f);

					m_club_nodes[i]->m_w_asset->SetWeaponEquipState(true);
					m_player_node->AddChildNode(m_club_nodes[i]);	

					for (int j = 0; j < objs.size(); j++)
					{
						if (m_club_nodes[i]->m_w_asset == objs[j])
						{
							objs.erase(objs.begin() + j);
						}
					}

					m_club_nodes.erase(m_club_nodes.begin() + i);

					m_player_node->GetEquippedWeaponNode().m_w_asset->SetCurPos();

				}
				
			}			
		

			// Stopping the player at collision
			for (int i = 0; i < objs.size(); i++)
			{
				m_player_node->m_p_asset->RestrictPos(objs[i]->IsColliding());
			}

			// Storing previous positions of the player
			for (int i = 0; i < objs.size(); i++)
			{
				m_player_node->m_p_asset->UpdatePos(objs[i]->IsColliding());
			}
		

		// Stopping the camera at collision
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

		count = 0;

		 //Check if the player is not colliding with anything
		for (int i = 0; i < objs.size(); i++)
		{
			if (objs[i]->IsColliding() == false)
			{
				count += 1;
			}
		}

		// If yes, then player is not on ground
		if (count == objs.size())
		{
			m_player_node->m_p_asset->SetOnGroundState(false);
		}
		

		for (int i = 0; i < objs.size(); i++)
		{
			if (m_player_node->m_p_asset->CheckPlayerFeetonGround(objs[i]) == true)
			{
				m_player_node->m_p_asset->SetOnGroundState(true);
				break;
			}

		}
	
		m_player_node->m_p_asset->PullDown();


		if (m_player_node->m_p_asset->GetJumpState() == true && perspective->GetCollidingState() == false)
		{
			perspective->Move(0.0f, 0.0015f, 0.0f);
		}

		if (m_player_node->m_p_asset->GetOnGroundState() == false && m_player_node->m_p_asset->GetJumpState() == false && perspective->GetCollidingState() == false)
		{
			perspective->Move(0.0f, -0.0015f, 0.0f);
		}

	// Mouse Controls
	

	//perspective->RotateCameraY(-key->m_mouse_state.lY);
		//if (perspective->GetCollidingState() == false)
		//{
	perspective->RotateCameraX(key->m_mouse_state.lX);
		//}

	

	// DRAW



	m_player_node->Execute(&XMMatrixIdentity(), &perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());

	for (int i = 0; i < objs.size(); i++)
	{
		objs[i]->Draw(NULL, &perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());
	}


	m_render_target->Display();
}

void Game::InitialiseGameAssets()
{
	fopen_s(&assetFile, "Scripts/Asset_Positions.txt", "r");
	fgetpos(assetFile, &scriptPosition);

	ground = new Statik(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), 0.0f, -2.0f, 0.0f);

	objs.push_back(ground);

	ground->ScaleAsset(100.0f, 1.0f, 100.0f);

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	int num_assets;

	//int val;

	char asset_type[256];
	char node_type;

	while (Initialised == false)
	{
		fscanf(assetFile, "%s", asset_type);

		if (strstr("Camera", asset_type) != 0)
		{
			fscanf(assetFile, "%f %f %f %f", &x, &y, &z, &w);

			perspective = new Camera(x, y, z, w);
		}

		if (strstr("Player", asset_type) != 0)
		{
			fscanf(assetFile, " %c %f %f %f", &node_type, &x, &y, &z);

			m_player_node = new SceneNode(node_type, m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), x, y, z);

		}

		if (strstr("Trees", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, "%f %f %f", &x, &y, &z);

				trees.push_back(new Statik(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), x, y, z));

				objs.push_back(trees[i]);

			}
		}

		if (strstr("Clubs", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %f %f %f", &node_type, &x, &y, &z);

				m_club_nodes.push_back(new SceneNode(node_type, m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), x, y, z));

				m_club_nodes[i]->m_w_asset->ScaleAsset(0.2f, 0.2f, 2.0f);

				objs.push_back(m_club_nodes[i]->m_w_asset);

			}

			Initialised = true;
		}
		
	}


}