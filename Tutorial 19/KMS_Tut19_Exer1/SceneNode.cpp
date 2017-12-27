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

	local_world *= XMMatrixScaling(m_scale, m_scale, m_scale);

	local_world *= XMMatrixRotationX(XMConvertToRadians(m_xangle));
	local_world *= XMMatrixRotationY(XMConvertToRadians(m_yangle));
	local_world *= XMMatrixRotationZ(XMConvertToRadians(m_zangle));

	local_world *= XMMatrixTranslation(m_x, m_y, m_z);

	local_world *= *world;

	if (m_p_model) m_p_model->Draw(&local_world, view, projection);

	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Execute(&local_world, view, projection);
	}


}

void SceneNode::SetModel(Model* obj)
{
	m_p_model = obj;
}

void SceneNode::UpdateRot(float pitch_degrees, float yaw_degrees, float roll_degrees)
{
	m_xangle += pitch_degrees;
	m_yangle += yaw_degrees;
	m_zangle += roll_degrees;
}

void SceneNode::MoveForward(float distance)
{
	m_x += sin(m_yangle) * distance;
	m_z += cos(m_yangle) * distance;
}

void SceneNode::UpdateCollisionTree(XMMATRIX* world, float scale)
{
	XMMATRIX local_world = XMMatrixIdentity();

	local_world *= XMMatrixScaling(m_scale, m_scale, m_scale);

	local_world *= XMMatrixRotationX(XMConvertToRadians(m_xangle));
	local_world *= XMMatrixRotationY(XMConvertToRadians(m_yangle));
	local_world *= XMMatrixRotationZ(XMConvertToRadians(m_zangle));

	local_world *= XMMatrixTranslation(m_x, m_y, m_z);

	local_world *= *world;

	m_world_scale = scale * m_scale;

	XMVECTOR v;

	if (m_p_model)
	{
		v = XMVectorSet(m_p_model->GetBoundingSphereX(), m_p_model->GetBoundingSphereY(), m_p_model->GetBoundingSphereZ(), 0.0f);
	}
	else
	{
		v = XMVectorSet(0.0, 0.0, 0.0, 0.0);
	}

	v = XMVector3Transform(v, local_world);
	m_world_centre_x = XMVectorGetX(v);
	m_world_centre_y = XMVectorGetY(v);
	m_world_centre_z = XMVectorGetZ(v);

	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->UpdateCollisionTree(&local_world, m_world_scale);
	}


}

bool SceneNode::CheckCollision(SceneNode* compare_tree)
{
	return CheckCollision(compare_tree, this);
}

bool SceneNode::CheckCollision(SceneNode* compare_tree, SceneNode* object_tree_root)
{
	if (object_tree_root == compare_tree)
	{
		return false;
	}

	if (m_p_model && compare_tree->m_p_model)
	{
		XMVECTOR v1 = GetWorldCentrePos();
		XMVECTOR v2 = compare_tree->GetWorldCentrePos();
		//XMVECTOR vdiff = v1 - v2;

		float x1 = XMVectorGetX(v1);
		float y1 = XMVectorGetY(v1);
		float z1 = XMVectorGetZ(v1);

		float x2 = XMVectorGetX(v2);
		float y2 = XMVectorGetY(v2);
		float z2 = XMVectorGetZ(v2);

		float dx = x1 - x2;
		float dy = y1 - y2;
		float dz = z1 - z2;
		
		float dist = (dx * dx) + (dy * dy) + (dz * dz);

		float r1 = compare_tree->m_p_model->GetBoundingSphereRadius() * compare_tree->m_world_scale;
		float r2 = this->m_p_model->GetBoundingSphereRadius() * m_world_scale;

		float sum_radius = r1 + r2;

		if (dist < sum_radius * sum_radius)
		{
			return true;
		}
	}

	for (int i = 0; i < compare_tree->m_children.size(); i++)
	{
		if (CheckCollision(compare_tree->m_children[i], object_tree_root) == true) return true;
	}

	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i]->CheckCollision(compare_tree, object_tree_root) == true) return true;
	}

	return false;

}

bool SceneNode::IncX(float inc, SceneNode* root_node)
{
	float old_x = m_x;
	m_x += inc;

	XMMATRIX identity = XMMatrixIdentity();

	root_node->UpdateCollisionTree(&identity, 1.0);

	if (CheckCollision(root_node) == true)
	{
		m_x = old_x;

		return true;
	}

	return false;

}

bool SceneNode::IncZ(float inc, SceneNode* root_node)
{
	float old_z = m_z;
	m_z += inc;

	XMMATRIX identity = XMMatrixIdentity();

	root_node->UpdateCollisionTree(&identity, 1.0);

	if (CheckCollision(root_node) == true)
	{
		m_z = old_z;

		return true;
	}

	return false;

}

XMVECTOR SceneNode::GetWorldCentrePos()
{
	return XMVectorSet(m_world_centre_x, m_world_centre_y, m_world_centre_z, 0.0f);
}

float SceneNode::GetX()
{
	return m_x;
}

float SceneNode::GetZ()
{
	return m_z;
}