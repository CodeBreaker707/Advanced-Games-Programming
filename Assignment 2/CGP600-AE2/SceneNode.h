#pragma once

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <xnamath.h>

#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"

class SceneNode
{
private:

	vector<SceneNode*> m_children;

	float m_x, m_y, m_z;
	float m_prev_x, m_prev_y, m_prev_z;
	float m_xangle, m_yangle, m_zangle;
	float m_scale_x, m_scale_y, m_scale_z;

	float m_collider_centre_x;
	float m_collider_centre_y;
	float m_collider_centre_z;

	bool m_isColliding = false;

public:

	Player* m_p_asset;
	Enemy*  m_e_asset;
	Weapon* m_w_asset;

	SceneNode(char c, ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x_pos, float y_pos, float z_pos, float x_scale, float y_scale, float z_scale);

	void AddChildNode(SceneNode* n);

	bool DetachNode(SceneNode* n);

	void Execute(XMMATRIX* world, XMMATRIX* view, XMMATRIX* projection);

	void UpdateCollisionTree(XMMATRIX* world);

	bool CheckCollision(SceneNode* compare_tree);
	bool CheckCollision(SceneNode* compare_tree, SceneNode* object_tree_root);

	XMVECTOR GetWorldCentrePos();

	void SetXPos(float x);
	void SetYPos(float y);
	void SetZPos(float z);

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

	int GetChildrenSize();
	vector<SceneNode*> GetChildren();

	SceneNode GetEquippedWeaponNode();

};