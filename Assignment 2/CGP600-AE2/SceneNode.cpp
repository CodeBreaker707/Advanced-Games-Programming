#include "SceneNode.h"

SceneNode::SceneNode(char c, ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x_pos, float y_pos, float z_pos, float x_scale, float y_scale, float z_scale)
{
	if (c == 'P')
	{
		m_p_asset = new Player(D3DDevice, ImmediateContext);
	}
	else if (c == 'E')
	{
		m_e_asset = new Enemy(D3DDevice, ImmediateContext);
	}
	else if (c == 'W')
	{
		m_w_asset = new Weapon(D3DDevice, ImmediateContext);
	}
	else if (c == 'S')
	{
		m_s_asset = new Statik(D3DDevice, ImmediateContext);
	}
	else if (c == 'D')
	{
		m_d_asset = new Dynamic(D3DDevice, ImmediateContext);
	}
	
		m_x = x_pos;
		m_y = y_pos;
		m_z = z_pos;

		/*x1 = 0.0f;
		y1 = 0.0f;
		z1 = 0.0f;

		l1 = 0.0f;
		h1 = 0.0f;
		b1 = 0.0f;

		x2 = 0.0f;
		y2 = 0.0f;
		z2 = 0.0f;

		l2 = 0.0f;
		h2 = 0.0f;
		z2 = 0.0f;*/

		m_xangle = 0.0f;
		m_yangle = 0.0f;
		m_zangle = 0.0f;

		m_scale_x = x_scale;
		m_scale_y = y_scale;
		m_scale_z = z_scale;

		m_collider_centre_x = x_pos;
		m_collider_centre_y = y_pos;
		m_collider_centre_z = z_pos;

		m_isColliding = false;
	

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

	//if (m_p_asset) local_world = m_p_asset->GetWorldMatrix();
	//else if (m_e_asset) local_world = m_e_asset->GetWorldMatrix();
	//else if (m_w_asset) local_world = m_w_asset->GetWorldMatrix();
	//else
	//{
		local_world *= XMMatrixScaling(m_scale_x, m_scale_y, m_scale_z);
		local_world *= XMMatrixRotationRollPitchYaw(m_xangle, m_yangle, m_zangle);
		local_world *= XMMatrixTranslation(m_x, m_y, m_z);
	//}
	
	
	local_world *= *world;
	

	if (m_p_asset) m_p_asset->Draw(&local_world, view, projection);
	if (m_e_asset && m_e_asset->GetEnemyHealth() > 0) m_e_asset->Draw(&local_world, view, projection);
	if (m_w_asset) m_w_asset->Draw(&local_world, view, projection);
	if (m_s_asset) m_s_asset->Draw(&local_world, view, projection);
	if (m_d_asset) m_d_asset->Draw(&local_world, view, projection);

	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Execute(&local_world, view, projection);
	}


}

void SceneNode::UpdateCollisionTree(XMMATRIX* world)
{
	XMMATRIX local_world = XMMatrixIdentity();

	local_world *= XMMatrixScaling(m_scale_x, m_scale_y, m_scale_z);

	local_world *= XMMatrixRotationX(XMConvertToRadians(m_xangle));
	local_world *= XMMatrixRotationY(XMConvertToRadians(m_yangle));
	local_world *= XMMatrixRotationZ(XMConvertToRadians(m_zangle));

	local_world *= XMMatrixTranslation(m_x, m_y, m_z);

	local_world *= *world;

	XMVECTOR v;

	if (m_p_asset)
	{
		v = XMVectorSet(XMVectorGetX(m_p_asset->box->GetColliderPos()), XMVectorGetY(m_p_asset->box->GetColliderPos()), XMVectorGetZ(m_p_asset->box->GetColliderPos()), 0.0f);
		//v = XMVectorSet(m_p_asset->GetXPos(), m_p_asset->GetYPos(), m_p_asset->GetZPos(), 0.0f);
	}
	else if (m_w_asset)
	{
		v = XMVectorSet(XMVectorGetX(m_w_asset->box->GetColliderPos()), XMVectorGetY(m_w_asset->box->GetColliderPos()), XMVectorGetZ(m_w_asset->box->GetColliderPos()), 0.0f);
		//v = XMVectorSet(m_w_asset->GetXPos(), m_w_asset->GetYPos(), m_w_asset->GetZPos(), 0.0f);
	}
	else if (m_e_asset)
	{
		v = XMVectorSet(XMVectorGetX(m_e_asset->box->GetColliderPos()), XMVectorGetY(m_e_asset->box->GetColliderPos()), XMVectorGetZ(m_e_asset->box->GetColliderPos()), 0.0f);
		//v = XMVectorSet(m_e_asset->GetXPos(), m_e_asset->GetYPos(), m_e_asset->GetZPos(), 0.0f);
	}
	else if (m_s_asset)
	{
		v = XMVectorSet(XMVectorGetX(m_s_asset->box->GetColliderPos()), XMVectorGetY(m_s_asset->box->GetColliderPos()), XMVectorGetZ(m_s_asset->box->GetColliderPos()), 0.0f);
	}
	else if (m_d_asset)
	{
		v = XMVectorSet(XMVectorGetX(m_d_asset->box->GetColliderPos()), XMVectorGetY(m_d_asset->box->GetColliderPos()), XMVectorGetZ(m_d_asset->box->GetColliderPos()), 0.0f);
	}
	else
	{
		v = XMVectorSet(0.0, 0.0, 0.0, 0.0);
	}

	v = XMVector3Transform(v, local_world);
	m_collider_centre_x = XMVectorGetX(v);
	m_collider_centre_y = XMVectorGetY(v);
	m_collider_centre_z = XMVectorGetZ(v);

	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->UpdateCollisionTree(&local_world);
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

	XMVECTOR v1 = GetWorldCentrePos();
	XMVECTOR v2 = compare_tree->GetWorldCentrePos();

	/*float x1 = 0.0f;
	float y1 = 0.0f;
	float z1 = 0.0f;

	float l1 = 0.0f;
	float h1 = 0.0f;
	float b1 = 0.0f;

	float x2 = 0.0f;
	float y2 = 0.0f;
	float z2 = 0.0f;

	float l2 = 0.0f;
	float h2 = 0.0f;
	float b2 = 0.0f;*/

	if (m_p_asset)
	{
		/*x1 = XMVectorGetX(v1) - (m_p_asset->box->GetLength(m_scale_x) / 2);
		y1 = XMVectorGetY(v1) + (m_p_asset->box->GetHeight(m_scale_y) / 2);
		z1 = XMVectorGetZ(v1) - (m_p_asset->box->GetBreadth(m_scale_z) / 2);

		l1 = m_p_asset->box->GetLength(m_scale_x);
		h1 = m_p_asset->box->GetHeight(m_scale_y);
		b1 = m_p_asset->box->GetBreadth(m_scale_z);*/
		CalculateDimensions1(v1, m_p_asset);

	}


	else if (m_e_asset)
	{
		/*x1 = XMVectorGetX(v1) - (m_e_asset->box->GetLength(m_scale_x) / 2);
		y1 = XMVectorGetY(v1) + (m_e_asset->box->GetHeight(m_scale_y) / 2);
		z1 = XMVectorGetZ(v1) - (m_e_asset->box->GetBreadth(m_scale_z) / 2);

		l1 = m_e_asset->box->GetLength(m_scale_x);
		h1 = m_e_asset->box->GetHeight(m_scale_y);
		b1 = m_e_asset->box->GetBreadth(m_scale_z);*/
		CalculateDimensions1(v1, m_e_asset);
	}


	else if (m_w_asset)
	{
		/* x1 = XMVectorGetX(v1) - (m_w_asset->box->GetLength(m_scale_x) / 2);
		 y1 = XMVectorGetY(v1) + (m_w_asset->box->GetHeight(m_scale_y) / 2);
		 z1 = XMVectorGetZ(v1) - (m_w_asset->box->GetBreadth(m_scale_z) / 2);

		 l1 = m_w_asset->box->GetLength(m_scale_x);
		 h1 = m_w_asset->box->GetHeight(m_scale_y);
		 b1 = m_w_asset->box->GetBreadth(m_scale_z);*/

		 CalculateDimensions1(v1, m_w_asset);
	}
	else if (m_s_asset)
	{
		CalculateDimensions1(v1, m_s_asset);
	}
	else if (m_d_asset)
	{
		CalculateDimensions1(v1, m_d_asset);
	}

	if (compare_tree->m_p_asset)
	{
		/* x2 = XMVectorGetX(v2) - (compare_tree->m_p_asset->box->GetLength(compare_tree->GetXScale()) / 2);
		 y2 = XMVectorGetY(v2) + (compare_tree->m_p_asset->box->GetHeight(compare_tree->GetYScale()) / 2);
		 z2 = XMVectorGetZ(v2) - (compare_tree->m_p_asset->box->GetBreadth(compare_tree->GetZScale()) / 2);

		 l2 = compare_tree->m_p_asset->box->GetLength(compare_tree->GetXScale());
		 h2 = compare_tree->m_p_asset->box->GetHeight(compare_tree->GetYScale());
		 b2 = compare_tree->m_p_asset->box->GetBreadth(compare_tree->GetZScale());*/

		CalculateDimensions2(v2, compare_tree->m_p_asset);

	}

	else if (compare_tree->m_e_asset)
	{
		 /*x2 = XMVectorGetX(v2) - (compare_tree->m_e_asset->box->GetLength(compare_tree->GetXScale()) / 2);
		 y2 = XMVectorGetY(v2) + (compare_tree->m_e_asset->box->GetHeight(compare_tree->GetYScale()) / 2);
		 z2 = XMVectorGetZ(v2) - (compare_tree->m_e_asset->box->GetBreadth(compare_tree->GetZScale()) / 2);

		 l2 = compare_tree->m_e_asset->box->GetLength(compare_tree->GetXScale());
		 h2 = compare_tree->m_e_asset->box->GetHeight(compare_tree->GetYScale());
		 b2 = compare_tree->m_e_asset->box->GetBreadth(compare_tree->GetZScale());*/
		CalculateDimensions2(v2, compare_tree->m_e_asset);

	}
	else if (compare_tree->m_w_asset)
	{
		/* x2 = XMVectorGetX(v2) - (compare_tree->m_w_asset->box->GetLength(compare_tree->GetXScale()) / 2);
		 y2 = XMVectorGetY(v2) + (compare_tree->m_w_asset->box->GetHeight(compare_tree->GetYScale()) / 2);
		 z2 = XMVectorGetZ(v2) - (compare_tree->m_w_asset->box->GetBreadth(compare_tree->GetZScale()) / 2);

		 l2 = compare_tree->m_w_asset->box->GetLength(compare_tree->GetXScale());
		 h2 = compare_tree->m_w_asset->box->GetHeight(compare_tree->GetYScale());
		 b2 = compare_tree->m_w_asset->box->GetBreadth(compare_tree->GetZScale());*/

		CalculateDimensions2(v2, compare_tree->m_w_asset);
	}
	else if (compare_tree->m_s_asset)
	{
		CalculateDimensions2(v2, compare_tree->m_s_asset);
	}
	else if (compare_tree->m_d_asset)
	{
		CalculateDimensions2(v2, compare_tree->m_d_asset);
	}
	

	float dist_x = x2 - x1;
	float dist_y = y2 - y1;
	float dist_z = z2 - z1;

	float main_dist = (dist_x * dist_x) + (dist_y * dist_y) + (dist_z * dist_z);

	float sum_radius = r1 + r2;

	if (main_dist < sum_radius * sum_radius)
	{
		m_isColliding = true;
	}
	else
	{
		m_isColliding = false;
	}

	//if (x1 < x2 + l2 && x1 + l1 > x2)
	//{
	//	if (y1 > y2 - h2 && y1 - h1 < y2)
	//	{
	//		if (z1 < z2 + b2 && z1 + b1 > z2)
	//		{
	//			m_isColliding = true;
	//		}
	//		else
	//		{
	//			//m_isColliding = false;
	//		}
	//	}
	//	else
	//	{
	//		m_isColliding = false;
	//	}
	//}
	//else
	//{
	//	m_isColliding = false;
	//}

	
		/*for (int i = 0; i < compare_tree->m_children.size(); i++)
		{
			if (CheckCollision(compare_tree->m_children[i], object_tree_root));
		}

		for (int i = 0; i < m_children.size(); i++)
		{
			if (m_children[i]->CheckCollision(compare_tree, object_tree_root));
		}*/
	

}

void SceneNode::CalculateDimensions1(XMVECTOR v, Asset* obj)
{
	x1 = XMVectorGetX(v) /*- (obj->box->GetLength(obj->GetXScale())  / 2)*/;
	y1 = XMVectorGetY(v)/* + (obj->box->GetHeight(obj->GetYScale())  / 2)*/;
	z1 = XMVectorGetZ(v) /*- (obj->box->GetBreadth(obj->GetZScale()) / 2)*/;

	r1 = obj->box->GetColliderRadius();

	/*l1 = obj->box->GetLength(obj->GetXScale());
	h1 = obj->box->GetHeight(obj->GetYScale());
	b1 = obj->box->GetBreadth(obj->GetZScale());*/
}

void SceneNode::CalculateDimensions2(XMVECTOR v, Asset* obj)
{
	x2 = XMVectorGetX(v)/* - (obj->box->GetLength(obj->GetXScale()) / 2)*/;
	y2 = XMVectorGetY(v)/* + (obj->box->GetHeight(obj->GetYScale()) / 2)*/;
	z2 = XMVectorGetZ(v)/* - (obj->box->GetBreadth(obj->GetZScale()) / 2)*/;

	r2 = obj->box->GetColliderRadius();

	/*l2 = obj->box->GetLength(obj->GetXScale());
	h2 = obj->box->GetHeight(obj->GetYScale());
	b2 = obj->box->GetBreadth(obj->GetZScale());*/

}


void SceneNode::MoveAsset(float x_dist, float y_dist, float z_dist)
{
	m_x += x_dist;
	m_y += y_dist;
	m_z += z_dist;
}

void SceneNode::RotateAsset(float pitch_degrees, float yaw_degrees, float roll_degrees)
{
	m_xangle += XMConvertToRadians(pitch_degrees);
	m_yangle += XMConvertToRadians(yaw_degrees);
	m_zangle += XMConvertToRadians(roll_degrees);

}

void SceneNode::RestrictPos(bool isColliding)
{
	if (isColliding == true)
	{
		m_x = m_prev_x;
		m_y = m_prev_y;
		m_z = m_prev_z;
	}
}

void SceneNode::UpdatePos(bool isColliding)
{
	if (isColliding == false)
	{
		m_prev_x = m_x;
		m_prev_y = m_y;
		m_prev_z = m_z;
	}

}

void SceneNode::SetXPos(float x)
{
	m_x = x;
}

void SceneNode::SetYPos(float y)
{
	m_y = y;
}

void SceneNode::SetZPos(float z)
{
	m_z = z;
}

void SceneNode::SetCurZPos()
{
	m_cur_pos_z = m_z;
}

void SceneNode::SetCollideState(bool state)
{
	m_isColliding = state;
}

bool SceneNode::IsColliding()
{
	return m_isColliding;
}

int SceneNode::GetChildrenSize()
{
	return m_children.size();
}

vector<SceneNode*> SceneNode::GetChildren()
{
	return m_children;
}

XMVECTOR SceneNode::GetWorldCentrePos()
{
	return XMVectorSet(m_collider_centre_x, m_collider_centre_y, m_collider_centre_z, 0.0f);
}

SceneNode* SceneNode::GetEquippedWeaponNode()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i]->m_w_asset->GetWeaponEquipState() == true)
		{
			return m_children[i];
		}
	}
}



float SceneNode::GetXPos()
{
	return m_x;
}

float SceneNode::GetYPos()
{
	return m_y;
}

float SceneNode::GetZPos()
{
	return m_z;
}

float SceneNode::GetCurZPos()
{
	return m_cur_pos_z;
}

float SceneNode::GetXAngle()
{
	return m_xangle;
}

float SceneNode::GetYAngle()
{
	return m_yangle;
}

float SceneNode::GetZAngle()
{
	return m_zangle;
}

float SceneNode::GetXScale()
{
	return m_scale_x;
}

float SceneNode::GetYScale()
{
	return m_scale_y;
}

float SceneNode::GetZScale()
{
	return m_scale_z;
}

