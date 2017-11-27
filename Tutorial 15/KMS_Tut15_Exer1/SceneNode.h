#pragma once

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <xnamath.h>

#include "Model.h"

class SceneNode
{
private:

	Model* m_p_model;

	vector<SceneNode*> m_children;

	float m_x, m_y, m_z;
	float dx, dz;
	float m_xangle, m_zangle, m_yangle;
	float m_scale;

	float m_world_centre_x;
	float m_world_centre_y;
	float m_world_centre_z;

	float m_world_scale;

public:

	SceneNode(float x, float y, float z);

	void AddChildNode(SceneNode* n);

	bool DetachNode(SceneNode* n);

	void Execute(XMMATRIX* world, XMMATRIX* view, XMMATRIX* projection);

	void UpdateRot(float pitch_degrees, float yaw_degrees, float roll_degrees);

	void MoveForward(float distance);

	void UpdateCollisionTree(XMMATRIX* world, float scale);

	bool CheckCollision(SceneNode* compare_tree);
	bool CheckCollision(SceneNode* compare_tree, SceneNode* object_tree_root);

	bool IncX(float inc, SceneNode* root_node);
	bool IncZ(float inc, SceneNode* root_node);

	void SetModel(Model* obj);

	XMVECTOR GetWorldCentrePos();

	float GetX();
	float GetZ();

};
