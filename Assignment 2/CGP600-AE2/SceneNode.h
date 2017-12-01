#pragma once

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <xnamath.h>

#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Statik.h"
#include "Dynamic.h"

class SceneNode
{
private:

	vector<SceneNode*> m_children;

	float m_x, m_y, m_z;
	float m_cur_pos_z;
	float m_prev_x, m_prev_y, m_prev_z;
	float m_xangle, m_yangle, m_zangle;
	float m_scale_x, m_scale_y, m_scale_z;

	float x1, y1, z1, r1;
	//float l1, h1, b1;

	float x2, y2, z2, r2;
	//float l2, h2, b2;

	float dist_x;
	float dist_y;
	float dist_z;

	float main_dist;

	float sum_radius;

	float m_collider_centre_x;
	float m_collider_centre_y;
	float m_collider_centre_z;

	bool m_isColliding;
	bool m_isInteracting;

public:

	Player* m_p_asset;
	Enemy*  m_e_asset;
	Weapon* m_w_asset;
	Statik* m_s_asset;
	Dynamic* m_d_asset;

	SceneNode(char c, ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x_pos, float y_pos, float z_pos, float x_scale, float y_scale, float z_scale);

	void AddChildNode(SceneNode* n);

	bool DetachNode(SceneNode* n);

	void Execute(XMMATRIX* world, XMMATRIX* view, XMMATRIX* projection);

	void UpdateCollisionTree(XMMATRIX* world);

	bool CheckCollision(SceneNode* compare_tree);

	bool CheckActionCollision(SceneNode* compare_tree);

	void CalculateCollisionDetails(SceneNode* compare_tree);

	void CalculateDimensions1(XMVECTOR v, Asset* obj);
	void CalculateDimensions2(XMVECTOR v, Asset* obj);

	XMVECTOR GetWorldCentrePos();

	void SetXPos(float x);
	void SetYPos(float y);
	void SetZPos(float z);

	void SetCurZPos();
	float GetCurZPos();

	float GetXPos();
	float GetYPos();
	float GetZPos();

	float GetXAngle();
	float GetYAngle();
	float GetZAngle();

	float GetXScale();
	float GetYScale();
	float GetZScale();

	void RestrictPos(bool isCollding);
	void UpdatePos(bool isColliding);

	void MoveAsset(float x_dist, float y_dist, float z_dist);
	void RotateAsset(float pitch_degrees, float yaw_degrees, float roll_degrees);

	void SetCollideState(bool state);
	bool IsColliding();

	void SetInteractState(bool state);
	bool IsInteracting();

	int GetChildrenSize();
	vector<SceneNode*> GetChildren();

	SceneNode* GetEquippedWeaponNode();
	SceneNode* GetPushingCrate();

};