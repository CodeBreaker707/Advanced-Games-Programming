#include "Game.h"

Game::Game(HINSTANCE hInstance, int nCmdShow)
{
	// Initialising the renderer object
	m_render_target = new Renderer(hInstance, nCmdShow);

	// Initialising the input object
	key = new Input(hInstance, m_render_target->GetWindow());

	// Initialising the UI object
	hud = new UI("Assets/font3.png", m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext());

	// Boolean variables are initialised
	Initialised = false;
	cineCamera = false;

	// Calling the function to initialise assets
	InitialiseGameAssets();

}

void Game::MainUpdate()
{
	// Setting the blend state to true for the UI object
	//m_render_target->SetBlendState(true);

	// This is called to clear the screen before
	// drawing again
	m_render_target->ClearBuffers();

	// Reads if any key is pressed
	key->ReadInputStates();

	// To check if the swapCamera key is released
	key->IsKeyReleased(key->swapCamera);
	
	// If the cinematic camera is not enabled,
	// play the game
	if (cineCamera == false)
	{
		// This restricts the pitch of the player's rotation
		m_player_node->RestrictPitch();

		// This rotates the player based on mouse input
		m_player_node->RotateAsset(key->m_mouse_state.lY, key->m_mouse_state.lX, 0.0f);


		// Keyboard Controls

		// If the forward key is pressed, this returns true
		if (key->IsKeyPressed(key->mve_frwd))
		{
			// We move the player forward in the
			// direction the player is facing. This
			// is determined by the yaw angle and movement
			// speed is retrieved from the player class

			m_player_node->MoveAsset(sin(m_player_node->GetYAngle()) * m_player_node->m_p_asset->GetPlayerMoveSpeed(), 0.0f,
			cos(m_player_node->GetYAngle()) * m_player_node->m_p_asset->GetPlayerMoveSpeed());
		}
		if (key->IsKeyPressed(key->mve_lft))
		{
			// We strafe the player left in the
			// direction the player is facing.

			m_player_node->MoveAsset(sin(m_player_node->GetYAngle() + XMConvertToRadians(90)) * -m_player_node->m_p_asset->GetPlayerMoveSpeed(), 0.0f,
				cos(m_player_node->GetYAngle() + XMConvertToRadians(90)) * -m_player_node->m_p_asset->GetPlayerMoveSpeed());

		}
		if (key->IsKeyPressed(key->mve_bck))
		{
			// We move the player back in the
			// direction the player is facing.

			m_player_node->MoveAsset(sin(m_player_node->GetYAngle()) * -m_player_node->m_p_asset->GetPlayerMoveSpeed(), 0.0f,
				cos(m_player_node->GetYAngle()) * -m_player_node->m_p_asset->GetPlayerMoveSpeed());

		}
		if (key->IsKeyPressed(key->mve_rght))
		{
			// We strafe the player right in the
			// direction the player is facing.

			m_player_node->MoveAsset(sin(m_player_node->GetYAngle() + XMConvertToRadians(90)) * m_player_node->m_p_asset->GetPlayerMoveSpeed(), 0.0f,
				cos(m_player_node->GetYAngle() + XMConvertToRadians(90)) * m_player_node->m_p_asset->GetPlayerMoveSpeed());

		}
		// If the player pressed the jump key and
		// if the player is on ground, this returns true
		if (key->IsKeyPressed(key->jump) && m_player_node->GetOnGroundState() == true)
		{
			// Set the player on ground boolean to false
			m_player_node->SetOnGroundState(false);

			// Set the jump state to true
			m_player_node->m_p_asset->SetJumpState(true);

			// Set the jump height 3 units higher 
			// from our current position
			m_player_node->m_p_asset->SetJumpHeight(m_player_node->GetYPos() + 3.0f);

		}

		// If the swap camera key is pressed,
		// enable the cinematic camera
		if (key->IsKeyPressedOnce(key->swapCamera))
		{
			cineCamera = true;
		}
		
		// If the sprint key is pressed, this
		// returns true
		if (key->IsKeyPressed(key->sprint))
		{
			// We double the speed multiplier 
			m_player_node->m_p_asset->SetSpeedMultiplier(2.0f);
		}
		else if (m_player_node->m_p_asset->GetSpeedMultiplier() != 1.0f)
		{
			// We reset the speed multiplier if the sprint
			// key is not pressed
			m_player_node->m_p_asset->SetSpeedMultiplier(1.0f);
		}

		
		// If the player is jumping, this returns true
		if (m_player_node->m_p_asset->GetJumpState() == true)
		{
			// We gradually increase the player's Y position
			m_player_node->MoveAsset(0.0f, m_player_node->m_p_asset->GetJumpSpeed(), 0.0f);

			// If we had reached the jump height, stop jumping 
			if (m_player_node->GetYPos() > m_player_node->m_p_asset->GetJumpHeight())
			{
				m_player_node->m_p_asset->SetJumpState(false);
			}
		}

		// Calling the function to update the player's
		// collision tree
		m_player_node->UpdateCollisionTree(&XMMatrixIdentity());

		// Checking collision of every object
		// in the world against the player
		for (int i = 0; i < objs.size(); i++)
		{
			objs[i]->CheckCollision(m_player_node);
		}

		// If the player has children and is carrying a weapon, this returns true
		if (m_player_node->GetChildrenSize() != 0 && m_player_node->m_p_asset->GetWeaponCarryingState() == true)
		{
			// If the drop key is pressed, this returns true
			if (key->IsKeyPressed(key->drop))
			{
				// We push the weapon back to the vector
				m_spear_nodes.push_back(m_player_node->GetEquippedWeaponNode());

				// It is now a child of the root node
				m_root_node->AddChildNode(m_player_node->GetEquippedWeaponNode());

				// Place the weapon a few units from 
				// the player in the world
				m_player_node->GetEquippedWeaponNode()->SetXPos(m_player_node->GetXPos() + sin(m_player_node->GetYAngle()) * 3.0);
				m_player_node->GetEquippedWeaponNode()->SetYPos(m_player_node->GetYPos() - 0.5);
				m_player_node->GetEquippedWeaponNode()->SetZPos(m_player_node->GetZPos() + cos(m_player_node->GetYAngle()) * 3.0);

				// Set the player weapon carrying 
				// state to false
				m_player_node->m_p_asset->SetWeaponCarryingState(false);

				// Push the weapon back to the all objects vector
				objs.push_back(m_player_node->GetEquippedWeaponNode());

				// Finally, detach the weapon 
				// node from the player node
				m_player_node->DetachNode(m_player_node->GetEquippedWeaponNode());

			}

			// A double check because this shouldn't be executed
			// if the player dropped the weapon
			if (m_player_node->GetChildrenSize() != 0)
			{
				// If the left mouse button is pressed, the player is
				// carrying a weapon and if the player isn't attacking,
				// this returns true
				if (key->m_mouse_state.rgbButtons[0] && m_player_node->m_p_asset->GetWeaponCarryingState() == true
					&& m_player_node->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackedState() == false && m_player_node->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackCompleteState() == false)
				{
					// Set the attack state to true
					m_player_node->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackedState(true);

					// Set the current Z position of the weapon
					m_player_node->GetEquippedWeaponNode()->SetCurZPos();

				}

				// If the weapon is in attack state, 
				// this returns true
				if (m_player_node->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackedState() == true)
				{
					// If the weapon has reached enough distance, this
					// will return true
					if (m_player_node->GetEquippedWeaponNode()->GetZPos() >=
						m_player_node->GetEquippedWeaponNode()->GetCurZPos() + 1.5)
					{
						// The weapon isn't attacking anymore and 
						// is now in completed state
						m_player_node->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackedState(false);
						m_player_node->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackCompleteState(true);
					}
					// Else move the weapon forward
					else
					{
						m_player_node->GetEquippedWeaponNode()->MoveAsset(0.0f, 0.0f, 0.005);
					}

				}

				// If the weapon has completed its attack,
				// this will return true
				else if (m_player_node->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackCompleteState() == true)
				{

					// If the weapon has reached its original
					// position, this will return true
					if (m_player_node->GetEquippedWeaponNode()->GetZPos()
						<= m_player_node->GetEquippedWeaponNode()->GetCurZPos())
					{
						// Now the weapon has attacked and completed
						m_player_node->GetEquippedWeaponNode()->m_w_asset->SetWeaponAttackCompleteState(false);
					}
					// Else move the weapon back
					else
					{
						m_player_node->GetEquippedWeaponNode()->MoveAsset(0.0f, 0.0f, -0.005f);
					}
				}

				// We now loop through every enemy
				// in the game
				for (int i = 0; i < m_enemy_nodes.size(); i++)
				{
					// Checking collision of every enemy,
					// against the weapon
					m_enemy_nodes[i]->CheckActionCollision(m_player_node->GetEquippedWeaponNode());

					// If the weapon is colliding and in attack state,
					// this returns true
					if (m_enemy_nodes[i]->IsInteracting() == true && m_player_node->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackedState() == true &&
						m_player_node->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackCompleteState() == false)
					{
						// This will reduce the enemy's health by 1
						m_enemy_nodes[i]->m_e_asset->SetEnemyHealth(m_enemy_nodes[i]->m_e_asset->GetEnemyHealth() - 1);

						// If the enemy's health is or below 0,
						// this returns true
						if (m_enemy_nodes[i]->m_e_asset->GetEnemyHealth() <= 0)
						{
							// We now loop through every object in the game
							for (int j = 0; j < objs.size(); j++)
							{
								// If the enemy object matches the
								// object in the all objects vector,
								// this returns true
								if (objs[j] == m_enemy_nodes[i])
								{
									// We erase the object from the vector
									objs.erase(objs.begin() + j);
								}
							}

							// Detaching the enemy node from the
							// root node
							m_root_node->DetachNode(m_enemy_nodes[i]);

							// Erasing the enemy's weapon node
							m_eweapon_nodes.erase(m_eweapon_nodes.begin() + i);

							// Finally, we erase the enemy itself
							m_enemy_nodes.erase(m_enemy_nodes.begin() + i);
						}
					}
	
				}
			}

		}

		// Looping through every enemy
		for (int i = 0; i < m_enemy_nodes.size(); i++)
		{

			m_enemy_nodes[i]->CheckInRange(m_player_node->GetWorldPos());

			// Looping through every object in the game
			for (int j = 0; j < objs.size(); j++)
			{
				// Checking collision of every enemy 
				// against every other object
				m_enemy_nodes[i]->CheckCollision(objs[j]);

				// If the enemy is colliding with anything,
				// this will return true
				if (m_enemy_nodes[i]->IsColliding())
				{
					// Revert all enemy states
					m_enemy_nodes[i]->SetHaltState(false);
					m_enemy_nodes[i]->SetInRangeState(false);

					// Make the enemy move to its previous spot
					m_enemy_nodes[i]->SetToPreviousSpot();

					// Break the loop to stop further collision
					// checks
					break;

				}
			}

			// Make the enemy look at its movement
			// spots
			m_enemy_nodes[i]->LookAt();

			// Make the enemy move in the direction it's facing
			m_enemy_nodes[i]->MoveAsset(sin(m_enemy_nodes[i]->GetYAngle()) * 0.001, 0.0f, cos(m_enemy_nodes[i]->GetYAngle()) * 0.001);

			// If the enemy has reached closer to the player, then commence attack
			// Same attack logic as the player
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

			// Check the collision of the player 
			// against every other enemy weapon
			m_player_node->CheckActionCollision(m_enemy_nodes[i]->GetEquippedWeaponNode());

			// If the enemy weapon is colliding with the
			// player, this returns true
			if (m_player_node->IsInteracting() && m_enemy_nodes[i]->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackedState() == true &&
				m_enemy_nodes[i]->GetEquippedWeaponNode()->m_w_asset->GetWeaponAttackCompleteState() == false)
			{
				// This reduces the player's health by 1
				m_player_node->m_p_asset->SetPlayerHealth(m_player_node->m_p_asset->GetPlayerHealth() - 1);

				// If the player has lost all its
				// health, this will return true
				if (m_player_node->m_p_asset->GetPlayerHealth() <= 0)
				{
					// Restart the game
					RestartGame();
				}
			}
		}

		// Loop through every player 
		// weapon in the game
		for (int i = 0; i < m_spear_nodes.size(); i++)
		{
			// Check collision of every weapon
			// against the player
			m_spear_nodes[i]->CheckActionCollision(m_player_node);

			// If the weapon is colliding and if the player
			// has pressed the pick up key button, this
			// returns true
			if (m_spear_nodes[i]->IsInteracting() == true && key->IsKeyPressed(key->pick_up))
			{
				// Setting the position of the weapon
				// from player's position
				m_spear_nodes[i]->SetXPos(1.0f);
				m_spear_nodes[i]->SetYPos(-0.5f);
				m_spear_nodes[i]->SetZPos(1.0f);

				//m_spear_nodes[i]->SetYAngle(m_player_node->GetYAngle());

				m_root_node->DetachNode(m_spear_nodes[i]);

				//m_spear_nodes[i]->m_w_asset->SetWeaponEquipState(true);
				m_player_node->m_p_asset->SetWeaponCarryingState(true);
				m_player_node->AddChildNode(m_spear_nodes[i]);

				for (int j = 0; j < objs.size(); j++)
				{
					if (m_spear_nodes[i] == objs[j])
					{
						objs.erase(objs.begin() + j);
						break;
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
						break;
					}
				}

				m_dynamic_nodes.erase(m_dynamic_nodes.begin() + i);

			}


		}

		if (m_player_node->GetChildrenSize() != 0 && !key->IsKeyPressed(key->interact) && m_player_node->m_p_asset->GetPushState() == true)
		{

			m_dynamic_nodes.push_back(m_player_node->GetPushingCrate());
			m_root_node->AddChildNode(m_player_node->GetPushingCrate());

			m_player_node->GetPushingCrate()->SetXPos(m_player_node->GetXPos() + sin(m_player_node->GetYAngle()) * 2.5);
			m_player_node->GetPushingCrate()->SetYPos(m_player_node->GetYPos() - 0.5);
			m_player_node->GetPushingCrate()->SetZPos(m_player_node->GetZPos() + cos(m_player_node->GetYAngle()) * 2.5);

			m_player_node->GetPushingCrate()->SetYAngle(m_player_node->GetYAngle());

			//m_player_node->GetPushingCrate()->UpdateCollisionTree(&XMMatrixIdentity());

			m_player_node->m_p_asset->SetPushState(false);

			objs.push_back(m_player_node->GetPushingCrate());

			m_player_node->DetachNode(m_player_node->GetPushingCrate());

		}

		// Stopping the player at collision
		for (int i = 0; i < objs.size(); i++)
		{
			m_player_node->RestrictPos(objs[i]->IsColliding());


			/*if (m_player_node->GetChildrenSize() != 0)
			{
				if (m_player_node->m_p_asset->GetPushState() == true)
				{
					m_player_node->GetPushingCrate()->CheckActionCollision(objs[2]);
					m_player_node->RestrictPos(m_player_node->GetPushingCrate()->IsInteracting());
				}
			}*/
		}

		// Storing previous positions of the player
		for (int i = 0; i < objs.size(); i++)
		{

			m_player_node->UpdatePos(objs[i]->IsColliding());


			/*if (m_player_node->GetChildrenSize() != 0)
			{
				if (m_player_node->m_p_asset->GetPushState() == true)
				{
					m_player_node->GetPushingCrate()->CheckActionCollision(objs[i]);
					m_player_node->UpdatePos(m_player_node->GetPushingCrate()->IsInteracting());
				}
			}*/
		}



		//Check if the player is colliding with anything below		

		for (int i = 0; i < objs.size(); i++)
		{
			if (m_player_node->CheckNodeBottomCollision(objs[i]) == true)
			{
				m_player_node->SetOnGroundState(true);
				break;
			}
			else
			{
				m_player_node->SetOnGroundState(false);
			}

		}


		for (int i = 0; i < objs.size(); i++)
		{
			for (int j = 0; j < objs.size(); j++)
			{
				if (objs[i]->CheckNodeBottomCollision(objs[j]) == true)
				{
					objs[i]->SetOnGroundState(true);
					break;
				}
				else
				{
					objs[i]->SetOnGroundState(false);
				}
			}
		}

		if (m_player_node->m_p_asset->GetJumpState() == false)
		{
			m_player_node->ApplyGravity();
		}



		for (int i = 0; i < objs.size(); i++)
		{
			objs[i]->ApplyGravity();
			objs[i]->UpdateCollisionTree(&XMMatrixIdentity());
		}


		view[0]->MoveWithPlayer(m_player_node->GetXPos(), m_player_node->GetYPos(), m_player_node->GetZPos());


		// Mouse Controls

		
		view[0]->RotateCameraY(-key->m_mouse_state.lY);
		view[0]->RotateCameraX(key->m_mouse_state.lX);


		hud->AddText("HEALTH:", -0.98, 0.95, 0.04);

	}
	else
	{
		if (key->IsKeyPressedOnce(key->swapCamera))
		{
			cineCamera = false;
		}
		

		view[1]->RotateCameraY(-key->m_mouse_state.lY);
		view[1]->RotateCameraX(key->m_mouse_state.lX);
		

		if (key->IsKeyPressed(key->mve_frwd))
		{
			view[1]->Move(0.0f, 0.0f, 0.01f);
		}
		if (key->IsKeyPressed(key->mve_lft))
		{
			view[1]->Move(-0.01f, 0.0f, 0.0f);
		}
		if (key->IsKeyPressed(key->mve_bck))
		{
			view[1]->Move(0.0f, 0.0f, -0.01f);

		}
		if (key->IsKeyPressed(key->mve_rght))
		{
			view[1]->Move(0.01f, 0.0f, 0.0f);
		}
		
	}

	


	// DRAW

	if (cineCamera == false)
	{
		m_root_node->Execute(&XMMatrixIdentity(), &view[0]->GetViewMatrix(), &view[0]->GetProjectionMatrix());

	}
	else
	{
		m_root_node->Execute(&XMMatrixIdentity(), &view[1]->GetViewMatrix(), &view[1]->GetProjectionMatrix());

	}

	//hud->RenderText();
	//m_render_target->SetBlendState(false);


	m_render_target->Display();

}

void Game::InitialiseGameAssets()
{
	fopen_s(&assetFile, "Scripts/Asset_Details.txt", "r");

	m_root_node = new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), '\0', NULL, NULL, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0);

	//ground = new Statik(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), 0.0f, -2.0f, 0.0f, 100.0, 0.0, 100.0);

	//objs.push_back(ground);

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float cam_rot = 0.0f;
	float fov = 0.0f;
	float n_clip = 0.0f;
	float f_clip = 0.0f;

	float x_scle = 0.0f;
	float y_scle = 0.0f;
	float z_scle = 0.0f;

	char assetObj[256];
	char textureFile[256];

	//string assetObj;
	//string textureFile;

	int num_assets;

	int gravityState = 0;

	//int val;

	char asset_type[256];
	char node_type;

	while (Initialised == false)
	{
		fscanf(assetFile, "%s", asset_type);

		if (strstr("Camera", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, "%f %f %f %f %f %f %f", &x, &y, &z, &cam_rot, &fov, &n_clip, &f_clip);

				view.push_back(new Camera(x, y, z, cam_rot, fov, m_render_target->GetWindowWidth(), m_render_target->GetWindowHeight(), n_clip, f_clip));

			}
		}

		if (strstr("Player", asset_type) != 0)
		{
			fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

			m_player_node = new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState);

			m_root_node->AddChildNode(m_player_node);

		}

		if (strstr("Enemy", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

				m_enemy_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState));

				objs.push_back(m_enemy_nodes[i]);

				m_root_node->AddChildNode(m_enemy_nodes[i]);

			}


		}


		if (strstr("Spear", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

				m_spear_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState));

				objs.push_back(m_spear_nodes[i]);

				m_root_node->AddChildNode(m_spear_nodes[i]);

			}

			
		}

		if (strstr("Club", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

				m_eweapon_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState));

				m_enemy_nodes[i]->AddChildNode(m_eweapon_nodes[i]);

			}


		}

		if (strstr("Dynamic", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

				m_dynamic_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState));

				objs.push_back(m_dynamic_nodes[i]);

				m_root_node->AddChildNode(m_dynamic_nodes[i]);

			}


		}


		if (strstr("Statik", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

				m_statik_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState));

				objs.push_back(m_statik_nodes[i]);
				m_root_node->AddChildNode(m_statik_nodes[i]);

			}

			Initialised = true;
			fclose(assetFile);
		}

		
	}


}

void Game::RestartGame()
{
	Initialised = false;

	fopen_s(&assetFile, "Scripts/Asset_Details.txt", "r");

	m_player_node->ResetToInitalPos();

	for (int i = 0; i < m_enemy_nodes.size(); i++)
	{
		for (int j = 0; j < objs.size(); j++)
		{
			if (objs[j] == m_enemy_nodes[i])
			{
				objs.erase(objs.begin() + j);
				break;
			}
		}

		m_root_node->DetachNode(m_enemy_nodes[i]);

	}

	m_enemy_nodes.clear();
	m_eweapon_nodes.clear();

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	float x_scle = 0.0f;
	float y_scle = 0.0f;
	float z_scle = 0.0f;

	int num_assets;

	int gravityState = 0;

	char assetObj[256];
	char textureFile[256];

	char asset_type[256];
	char node_type;

	while (Initialised == false)
	{
		fscanf(assetFile, "%s", asset_type);

		if (strstr("Enemy", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

				m_enemy_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState));

				objs.push_back(m_enemy_nodes[i]);

				m_root_node->AddChildNode(m_enemy_nodes[i]);

			}


		}

		if (strstr("Club", asset_type) != 0)
		{
			fscanf(assetFile, "%d", &num_assets);

			for (int i = 0; i < num_assets; i++)
			{
				fscanf(assetFile, " %c %s %s %f %f %f %f %f %f %d", &node_type, &assetObj, &textureFile, &x, &y, &z, &x_scle, &y_scle, &z_scle, &gravityState);

				m_eweapon_nodes.push_back(new SceneNode(m_render_target->GetD3DDevice(), m_render_target->GetDeviceContext(), node_type, assetObj, textureFile, x, y, z, x_scle, y_scle, z_scle, gravityState));

				m_enemy_nodes[i]->AddChildNode(m_eweapon_nodes[i]);

			}

			Initialised = true;
			fclose(assetFile);

		}
	}

}