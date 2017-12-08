#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>

#include "Renderer.h"
//#include "Asset.h"
#include "Camera.h"
#include "Input.h"
#include "Player.h"
#include "Statik.h"
#include "Weapon.h"
#include "Dynamic.h"
#include "SceneNode.h"
#include "UI.h"


class Game
{

private:
	Renderer* m_render_target;

	Input* key;

	vector<Camera*> view;

	FILE* assetFile;
	fpos_t scriptPosition;

	SceneNode* m_root_node;

	SceneNode* m_player_node;

	vector <SceneNode*> m_spear_nodes;
	vector <SceneNode*> m_enemy_nodes;
	vector <SceneNode*> m_eweapon_nodes;
	vector <SceneNode*> m_statik_nodes;
	vector <SceneNode*> m_dynamic_nodes;

	UI* hud;

	vector<SceneNode*> objs;

	bool pickedUp;
	bool Initialised;
	bool cineCamera;

	int camNum;

	//int prev_mouse_x = 0;
	//int prev_mouse_y = 0;

	//int mouse_x = 0;
	//int mouse_y = 0;

	//float yaw_degrees;
	//float pitch_degrees;

public:
	Game(HINSTANCE hInstance, int nCmdShow);

	void MainUpdate();

	void InitialiseGameAssets();
	void RestartGame();

};
