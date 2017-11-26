#include "SceneNode.h"

SceneNode::SceneNode(char c, ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x, float y, float z)
{
	if (c == 'P')
	{
		m_p_asset = new Player(D3DDevice, ImmediateContext, x, y, z);
	}
	else if (c == 'W')
	{
		m_w_asset = new Weapon(D3DDevice, ImmediateContext, x, y, z);
	}
	
	//m_x = x;
	//m_y = y;
	//m_z = z;
	//m_xangle = 0.0f;
	//m_zangle = 0.0f;
	//m_yangle = 0.0f;

	//m_scale = 1.0f;

}

void SceneNode::AddChildNode(SceneNode* n)
{
	m_children.push_back(n);
}

bool SceneNode::DetachNode(SceneNode* n)
{
	// Traverse tree to find node to detach

	for (int i = 0; i < m_children.size(); i++)
	{
		if (n == m_children[i])
		{
			m_children.erase(m_children.begin() + i);
			return true;
		}

		if (m_children[i]->DetachNode(n) == true) return true;
	}

	return false;
}

void SceneNode::Execute(XMMATRIX *world, XMMATRIX* view, XMMATRIX* projection)
{
	XMMATRIX local_world;

	if (m_p_asset) local_world = m_p_asset->GetWorldMatrix();
	//if (m_s_asset) local_world = m_s_asset->GetWorldMatrix();
	if (m_w_asset) local_world = m_w_asset->GetWorldMatrix();

	local_world *= *world;

	//if (m_p_asset) m_p_asset->Draw(&local_world, view, projection);
	//if (m_s_asset) m_s_asset->Draw(&local_world, view, projection);
	if (m_w_asset) m_w_asset->Draw(&local_world, view, projection);

	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Execute(&local_world, view, projection);
	}


}

int SceneNode::GetChildrenSize()
{
	return m_children.size();
}

SceneNode SceneNode::GetEquippedWeaponNode()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i]->m_w_asset->GetWeaponEquipState() == true)
		{
			return *m_children[i];
		}
	}
}

