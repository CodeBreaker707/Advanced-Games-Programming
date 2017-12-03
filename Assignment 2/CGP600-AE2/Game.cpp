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

	m_player_node->RotateAsset(0.0f, key->m_mouse_state.lX, 0.0f);

	m_player_node->UpdateCollisionTree(&XMMatrixIdentity());

	

	//m_enemy_nodes[0]->UpdateCollisionTree(&XMMatrixIdentity());
	//player->UpdatePosVector();
	//player->UpdateLookAt();

	// Keyboard Controls

	
		if (key->IsKeyPressed(key->mve_frwd))
		{
			m_player_node->MoveAsset(sin(m_player_node->GetYAngle()) * 0.001, 0.0f, cos(m_player_node->GetYAngle()) * 0.001);
			m_player_node->UpdateCollisionTree(&XMMatrixIdentity());
			//m_player_node->m_p_asset->MovePlayer(m_player_node->m_p_asset->GetPlayerMoveSpeed());
			
			if (perspective->GetCollidingState() == false)
			{
				perspective->Move(0.0f, 0.0f, m_player_node->m_p_asset->GetPlayerMoveSpeed());
			}
		}
		if (key->IsKeyPressed(key->mve_lft))
		{
			//m_player_node->MoveAsset(-0.001, 0.0f, 0.0f);
			//player->StrafePlayer(-player->GetPlayerMoveSpeed());

			if (perspective->GetCollidingState() == false)
			{
				//perspective->Move(-m_player_node->m_p_asset->GetPlayerMoveSpeed(), 0.0f, 0.0f);
			}
		}
		if (key->IsKeyPressed(key->mve_bck))
		{
			m_player_node->MoveAsset(sin(m_player_node->GetYAngle()) * -0.001, 0.0f, cos(m_player_node->GetYAngle()) * -0.001);
			m_player_node->UpdateCollisionTree(&XMMatrixIdentity());
			//m_player_node->m_p_asset->MovePlayer(-m_player_node->m_p_asset->GetPlayerMoveSpeed());

			if (perspective->GetCollidingState() == false)
			{
				perspective->Move(0.0f, 0.0f, -m_player_node->m_p_asset->GetPlayerMoveSpeed());
			}
		}
		if (key->IsKeyPressed(key->mve_rght))
		{
			//m_player_node->MoveAsset(0.001, 0.0f, 0.0f);
			//player->StrafePlayer(player->GetPlayerMoveSpeed());

			if (perspective->GetCollidingState() == false)
			{
				//perspective->Move(m_player_node->m_p_asset->GetPlayerMoveSpeed(), 0.0f, 0.0f);
			}
		}
		if (key->IsKeyPressed(key->jump) && m_player_node->m_p_asset->GetOnGroundState() == true)
		{
			m_player_node->m_p_asset->SetOnGroundState(false);
			m_player_node->m_p_asset->SetJumpState(true);

			m_player_node->m_p_asset->SetJumpHeight(m_player_node->GetYPos() + 3.0f);
			
		}
		
		//m_player_node->m_p_asset->JumpPlayer();

		if (m_player_node->m_p_asset->GetJumpState() == true)
		{
			m_player_node->MoveAsset(0.0f, m_player_node->m_p_asset->GetJumpSpeed(), 0.0f);

			if (m_player_node->GetYPos() > m_player_node->m_p_asset->GetJumpHeight())
			{
				m_player_node->m_p_asset->SetJumpState(false);
			}
		}

		for (int i = 0; i < objs.size(); i++)
		{
			objs[i]->CheckCollision(m_player_node);
		}

		if (m_player_node->GetChildrenSize() != 0 && m_player_node->m_p_asset->GetWeaponCarryingState() == true)
		{

			if (key->IsKeyPressed(DIK_G))
			{
				m_spear_nodes.push_back(m_player_node->GetEquippedWeaponNode());
				m_root_node->AddChildNode(m_player_node->GetEquippedWeaponNode());

				m_player_node->GetEquippedWeaponNode()->SetXPos(m_player_node->GetXPos() + sin(m_player_node->GetYAngle()) * 3.0);
				m_player_node->GetEquippedWeaponNode()->SetYPos(m_player_node->GetYPos() - 0.5);
				m_player_node->GetEquippedWeaponNode()->SetZPos(m_player_node->GetZPos() + cos(m_player_node->GetYAngle()) * 3.0);

				m_player_node->m_p_asset->SetWeaponCarryingState(false);

				objs.push_back(m_player_node->GetEquippedWeaponNode());

				m_player_node->GetEquippedWeaponNode()->UpdateCollisionTree(&XMMatrixIdentity());

				m_player_node->DetachNode(m_player_node->GetEquippedWeaponNode());
			}

			if (m_player_node->GetChildrenSize() != 0)
			{
				if (key->m_mouse_state.rgbButtons[0] && m_player_node->m_p_asset->GetWeaponCarryingState() == true
					&& m_player_node->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackedState() == false && m_player_node->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackCompleteState() == false)
				{
					m_player_node->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackedState(true);
					m_player_node->GetEquippedWeaponNode()->SetCurZPos();

				}

				if (m_player_node->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackedState() == true)	
				{
					
					if (m_player_node->GetEquippedWeaponNode()->GetZPos() >=
						m_player_node->GetEquippedWeaponNode()->GetCurZPos() + 1.5 )
					{
						m_player_node->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackedState(false);
						m_player_node->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackCompleteState(true);
					}
					else
					{
						m_player_node->GetEquippedWeaponNode()->MoveAsset(0.0f, 0.0f, 0.005);
					}

				}

				else if (m_player_node->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackCompleteState() == true)
				{
					

					if (m_player_node->GetEquippedWeaponNode()->GetZPos()
						<= m_player_node->GetEquippedWeaponNode()->GetCurZPos() )
					{
						m_player_node->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackCompleteState(false);
					}
					else
					{
						m_player_node->GetEquippedWeaponNode()->MoveAsset(0.0f, 0.0f, -0.005f);
					}
				}

				for (int i = 0; i < m_enemy_nodes.size(); i++)
				{

					m_enemy_nodes[i]->CheckCollision(m_player_node);
					m_enemy_nodes[i]->CheckActionCollision(m_player_node->GetEquippedWeaponNode());


					if (m_enemy_nodes[i]->IsInteracting() == true && m_player_node->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackedState() == true &&
						m_player_node->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackCompleteState() == false)
					{
						m_enemy_nodes[i]->m_e_asset->SetEnemyHealth(m_enemy_nodes[i]->m_e_asset->GetEnemyHealth() - 1);

						if (m_enemy_nodes[i]->m_e_asset->GetEnemyHealth() <= 0)
						{
							
							for (int j = 0; j < objs.size(); j++)
							{
								if (objs[j] == m_enemy_nodes[i])
								{
									objs.erase(objs.begin() + j);
								}
							}

							m_root_node->DetachNode(m_enemy_nodes[i]);

							m_enemy_nodes.erase(m_enemy_nodes.begin() + i);
						}
					}


					//	//m_enemy_nodes[i]->m_e_asset->CheckCollision(m_player_node->m_p_asset);

					//	
				}		
			}

		}
		
		
			// Checking collision with all objects against the player
	

		//m_player_node->CheckCollision(m_root_node);
		for (int i = 0; i < m_enemy_nodes.size(); i++)
		{
			m_enemy_nodes[i]->CheckInRange(m_player_node->GetWorldPos());
			m_enemy_nodes[i]->LookAt();

			m_enemy_nodes[i]->MoveAsset(sin(m_enemy_nodes[i]->GetYAngle()) * 0.001, 0.0f, cos(m_enemy_nodes[i]->GetYAngle()) * 0.001);

			if (m_enemy_nodes[i]->isHalted() == true
				&& m_enemy_nodes[i]->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackedState() == false && m_enemy_nodes[i]->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackCompleteState() == false)
			{
				m_enemy_nodes[i]->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackedState(true);
				m_enemy_nodes[i]->GetEquippedWeaponNode()->SetCurZPos();

			}

			if (m_enemy_nodes[i]->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackedState() == true)
			{

				if (m_enemy_nodes[i]->GetEquippedWeaponNode()->GetZPos() >=
					m_enemy_nodes[i]->GetEquippedWeaponNode()->GetCurZPos() + 1.5)
				{
					m_enemy_nodes[i]->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackedState(false);
					m_enemy_nodes[i]->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackCompleteState(true);
				}
				else
				{
					m_enemy_nodes[i]->GetEquippedWeaponNode()->MoveAsset(0.0f, 0.0f, 0.005);
				}

			}

			else if (m_enemy_nodes[i]->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackCompleteState() == true)
			{


				if (m_enemy_nodes[i]->GetEquippedWeaponNode()->GetZPos()
					<= m_enemy_nodes[i]->GetEquippedWeaponNode()->GetCurZPos())
				{
					m_enemy_nodes[i]->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackCompleteState(false);
				}
				else
				{
					m_enemy_nodes[i]->GetEquippedWeaponNode()->MoveAsset(0.0f, 0.0f, -0.005f);
				}
			}

			m_enemy_nodes[i]->UpdateCollisionTree(&XMMatrixIdentity());
		}
		

		if (m_player_node->m_p_asset->GetJumpState() == false && m_player_node->m_p_asset->GetOnGroundState() == false)
		{
			m_player_node->MoveAsset(0.0f, -m_player_node->m_p_asset->GetJumpSpeed(), 0.0f);
		}


		if (m_player_node->m_p_asset->GetJumpState() == true && perspective->GetCollidingState() == false)
		{
			perspective->Move(0.0f, 0.0010f, 0.0f);
		}

		if (m_player_node->m_p_asset->GetOnGroundState() == false && m_player_node->m_p_asset->GetJumpState() == false && perspective->GetCollidingState() == false)
		{
			perspective->Move(0.0f, -0.0010f, 0.0f);
		}

		
			for (int i = 0; i < m_spear_nodes.size(); i++)
			{
				//m_spear_nodes[i]->CheckCollision(m_player_node);
				m_spear_nodes[i]->CheckActionCollision(m_player_node);

				if (m_spear_nodes[i]->IsInteracting() == true && key->IsKeyPressed(key->pick_up))
				{
					m_spear_nodes[i]->SetXPos(1.0f);
					m_spear_nodes[i]->SetYPos(-0.5f);
					m_spear_nodes[i]->SetZPos(1.0f);


					m_root_node->DetachNode(m_spear_nodes[i]);

					//m_spear_nodes[i]->m_w_asset->SetWeaponEquipState(true);
					m_player_node->m_p_asset->SetWeaponCarryingState(true);
					m_player_node->AddChildNode(m_spear_nodes[i]);

					for (int j = 0; j < objs.size(); j++)
					{
						if (m_spear_nodes[i] == objs[j])
						{
							objs.erase(objs.begin() + j);
						}
					}

					m_spear_nodes.erase(m_spear_nodes.begin() + i);

				}

			}

			for (int i = 0; i < m_dynamic_nodes.size(); i++)
			{
				if (m_player_node->m_p_asset->GetPushState() == false)
				{
					m_dynamic_nodes[i]->CheckCollision(m_player_node);
				}

				m_dynamic_nodes[i]->CheckActionCollision(m_player_node);

				if (m_dynamic_nodes[i]->IsInteracting() && key->IsKeyPressed(key->interact) && m_player_node->m_p_asset->GetPushState() == false)
				{
					/*m_dynamic_nodes[i]->SetXPos(m_player_node->GetXPos());
					m_dynamic_nodes[i]->SetYPos(m_player_node->GetYPos() - 0.5f);
					m_dynamic_nodes[i]->SetZPos(m_player_node->GetZPos() + cos(m_player_node->GetYAngle()) + 2.0f);

					m_dynamic_nodes[i]->UpdateCollisionTree(&XMMatrixIdentity());*/

					m_dynamic_nodes[i]->SetXPos(0.0f);
					m_dynamic_nodes[i]->SetYPos(-0.5f);
					m_dynamic_nodes[i]->SetZPos(2.0f);

					//m_dynamic_nodes[i]->SetYAngle(m_player_node->GetYAngle());

					m_root_node->DetachNode(m_dynamic_nodes[i]);

					m_player_node->m_p_asset->SetPushState(true);

					m_player_node->AddChildNode(m_dynamic_nodes[i]);

					for (int j = 0; j < objs.size(); j++)
					{
						if (m_dynamic_nodes[i] == objs[j])
						{
							objs.erase(objs.begin() + j);
						}
					}

					m_dynamic_nodes.erase(m_dynamic_nodes.begin() + i);

				}
				

			}
		
			if (m_player_node->GetChildrenSize() != 0 && !key->IsKeyPressed(key->interact) && m_player_node->m_p_asset->GetPushState() == true)
			{
				
					m_dynamic_nodes.push_back(m_player_node->GetPushingCrate());
					m_root_node->AddChildNode(m_player_node->GetPushingCrate());

					m_player_node->GetPushingCrate()->SetXPos(m_player_node->GetXPos() + sin(m_player_node->GetYAngle()) * 3.0);
					m_player_node->GetPushingCrate()->SetYPos(m_player_node->GetYPos() - 0.5);
					m_player_node->GetPushingCrate()->SetZPos(m_player_node->GetZPos() + cos(m_player_node->GetYAngle()) * 3.0);

					m_player_node->GetPushingCrate()->SetYAngle(m_player_node->GetYAngle());

					m_player_node->GetPushingCrate()->UpdateCollisionTree(&XMMatrixIdentity());

					m_player_node->m_p_asset->SetPushState(false);

					objs.push_back(m_player_node->GetPushingCrate());

					m_player_node->DetachNode(m_player_node->GetPushingCrate());
				
			}

			// Stopping the player at collision
			for (int i = 0; i < objs.size(); i++)
			{
				m_player_node->RestrictPos(objs[i]->IsColliding());				
			}

			// Storing previous positions of the player
			for (int i = 0; i < objs.size(); i++)
			{
				m_player_node->UpdatePos(objs[i]->IsColliding());
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

		 //Check if the player is colliding with anything below		

		for (int i = 0; i < objs.size(); i++)
		{
			if (m_player_node->CheckNodeBottomCollision(objs[i]) == true)
			{
				m_player_node->m_p_asset->SetOnGroundState(true);
				break;
			}
			else
			{
				m_player_node->m_p_asset->SetOnGroundState(false);
			}

		}
	
		//m_player_node->m_p_asset->PullDown();

		

	// Mouse Controls
	

	//perspective->RotateCameraY(-key->m_mouse_state.lY);
		//if (perspective->GetCollidingState() == false)
		//{
	perspective->RotateCameraX(key->m_mouse_state.lX);//}

	

	// DRAW


	/*m_player_node->Execute(&XMMatrixIdentity(), &perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());

	for (int i = 0; i < m_enemy_nodes.size(); i++)
	{
		if (m_enemy_nodes[i]->m_e_asset->GetEnemyHealth() > 0)
		{
			m_enemy_nodes[i]->Execute(&XMMatrixIdentity(), &perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());
		}
	}*/

	m_root_node->Execute(&XMMatrixIdentity(), &perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());

	/*for (int i = 0; i < objs.size(); i++)
	{
		objs[i]->Draw(NULL, &perspective->GetViewMatrix(), &perspective->GetProjectionMatrix());
	}*/


	m_render_target->Display();

}

void Game::InitialiseGameAssets()
{
	fopen_s(&assetFile, "Scripts/Asset_Details.txt", "r");
	fgetpos(assetFile, &scriptPosition);

	m_root_node = new SceneNode('\0', m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);

	//ground = new Statik(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), 0.0f, -2.0f, 0.0f, 100.0, 0.0, 100.0);

	//objs.push_back(ground);

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	float x_scle = 0.0f;
	float y_scle = 0.0f;
	float z_scle = 0.0f;

	int num_assets;

	//int val;

	char asset_type[256];
	char node_type;

	while (Initialised == false)
	{
		fscanf(assetFile, "%s", asset_type);

		if (strstr("Camera", asset_type) != 0)
		{
			fscanf(assetFile, " %f %f %f %f", &x, &y, &z, &w);

			perspective = new Camera(x, y, z, w);
		}

		if (strstr("Player", asset_type) != 0)
		{
			fscanf(assetFile, " %c %f %f %f %f %f %f", &node_type, &x, &y, &z, &x_scle, &y_scle, &z_scle);

			m_player_node = new SceneNode(node_type, m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), x, y, z, x_scle, y_scle, z_scle);

			m_root_node->AddChildNode(m_player_node);

		}

		if (strstr("Enemy", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %f %f %f %f %f %f", &node_type, &x, &y, &z, &x_scle, &y_scle, &z_scle);

				m_enemy_nodes.push_back(new SceneNode(node_type, m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), x, y, z, x_scle, y_scle, z_scle));

				objs.push_back(m_enemy_nodes[i]);

				m_root_node->AddChildNode(m_enemy_nodes[i]);

			}


		}


		if (strstr("Spear", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %f %f %f %f %f %f", &node_type, &x, &y, &z, &x_scle, &y_scle, &z_scle);

				m_spear_nodes.push_back(new SceneNode(node_type, m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), x, y, z, x_scle, y_scle, z_scle));

				objs.push_back(m_spear_nodes[i]);

				m_root_node->AddChildNode(m_spear_nodes[i]);

			}

			
		}

		if (strstr("EWeapon", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %f %f %f %f %f %f", &node_type, &x, &y, &z, &x_scle, &y_scle, &z_scle);

				m_eweapon_nodes.push_back(new SceneNode(node_type, m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), x, y, z, x_scle, y_scle, z_scle));

				//objs.push_back(m_spear_nodes[i]);

				m_enemy_nodes[i]->AddChildNode(m_eweapon_nodes[i]);

			}


		}

		if (strstr("Dynamic", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %f %f %f %f %f %f", &node_type, &x, &y, &z, &x_scle, &y_scle, &z_scle);

				m_dynamic_nodes.push_back(new SceneNode(node_type, m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), x, y, z, x_scle, y_scle, z_scle));

				objs.push_back(m_dynamic_nodes[i]);

				m_root_node->AddChildNode(m_dynamic_nodes[i]);

			}


		}


		if (strstr("Statik", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %f %f %f %f %f %f", &node_type, &x, &y, &z, &x_scle, &y_scle, &z_scle);

				m_statik_nodes.push_back(new SceneNode(node_type, m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), x, y, z, x_scle, y_scle, z_scle));

				objs.push_back(m_statik_nodes[i]);
				m_root_node->AddChildNode(m_statik_nodes[i]);

			}

			Initialised = true;
			fclose(assetFile);
		}

		
	}


}