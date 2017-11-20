#include "SceneNode.h"

SceneNode::SceneNode(float x, float y, float z)
{
	m_p_model = NULL;

	m_x = x;
	m_y = y;
	m_z = z;
	m_xangle = 0.0f;
	m_zangle = 0.0f;
	m_yangle = 0.0f;

	m_scale = 1.0f;

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
	XMMATRIX local_world = XMMatrixIdentity();

	local_world *= XMMatrixRotationX(XMConvertToRadians(m_xangle));
	local_world *= XMMatrixRotationX(XMConvertToRadians(m_xangle));
	local_world *= XMMatrixRotationX(XMConvertToRadians(m_xangle));

	local_world *= XMMatrixScaling(m_scale, m_scale, m_scale);

	local_world *= XMMatrixTranslation(m_x, m_y, m_z);

	local_world *= *world;

	if (m_p_model) m_p_model->Draw(&local_world, view, projection);

	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Execute(&local_world, view, projection);
	}


}

void Model::UpdateRot(float pitch_degrees, float yaw_degrees, float roll_degrees)
{
	m_xangle += pitch_degrees;
	m_yangle += yaw_degrees;
	m_zangle += roll_degrees;
}

void Model::MoveForward(float distance)
{
	m_x += sin(m_yangle) * distance;
	m_z += cos(m_yangle) * distance;
}

float Model::GetX()
{
	return m_x;
}

float Model::GetZ()
{
	return m_z;
}