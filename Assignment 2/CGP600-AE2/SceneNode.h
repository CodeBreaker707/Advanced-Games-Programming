#pragma once

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <xnamath.h>

#include "Player.h"
#include "Statik.h"
#include "Weapon.h"

class SceneNode
{
private:

	vector<SceneNode*> m_children;

	float m_x, m_y, m_z;
	float dx, dz;
	float m_xangle, m_zangle, m_yangle;
	float m_scale;

public:

	Player* m_p_asset;
	Weapon* m_w_asset;

	SceneNode(char c, ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x, float y, float z);

	void AddChildNode(SceneNode* n);

	bool DetachNode(SceneNode* n);

	void Execute(XMMATRIX* world, XMMATRIX* view, XMMATRIX* projection);

	int GetChildrenSize();

	SceneNode GetEquippedWeaponNode();

};