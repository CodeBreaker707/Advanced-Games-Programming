#include "SceneNode.h"



SceneNode::SceneNode(char c, ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x_pos, float y_pos, float z_pos, float x_scale, float y_scale, float z_scale)
{
	if (c == 'P')
	{
		m_p_asset = new Player(D3DDevice, ImmediateContext, x_scale,  y_scale, z_scale);
	}
	else if (c == 'E')
	{
		m_e_asset = new Enemy(D3DDevice, ImmediateContext,  x_scale, y_scale,  z_scale);
	}
	else if (c == 'W')
	{
		m_w_asset = new Weapon(D3DDevice, ImmediateContext, x_scale,  y_scale, z_scale);
	}
	else if (c == 'S')
	{
		m_s_asset = new Statik(D3DDevice, ImmediateContext, x_scale,  y_scale, z_scale);
	}
	else if (c == 'D')
	{
		m_d_asset = new Dynamic(D3DDevice, ImmediateContext, x_scale,  y_scale, z_scale);
	}

	srand(time(NULL));
	
		m_x = x_pos;
		m_y = y_pos;
		m_z = z_pos;

		m_xangle = 0.0f;
		m_yangle = 0.0f;
		m_zangle = 0.0f;

		m_scale_x = x_scale;
		m_scale_y = y_scale;
		m_scale_z = z_scale;

		m_collider_centre_x = x_pos;
		m_collider_centre_y = y_pos;
		m_collider_centre_z = z_pos;

		lookAt_dist_x = 0.0f;
		lookAt_dist_z = 0.0f;

		moveSpots[0] = XMVectorSet(m_x        , m_y, m_z + 20.0, 0.0f);
		moveSpots[1] = XMVectorSet(m_x + 20.0f, m_y, m_z, 0.0f);
		moveSpots[2] = XMVectorSet(m_x        , m_y, m_z - 20.0, 0.0f);
		moveSpots[3] = XMVectorSet(m_x - 20.0f, m_y, m_z, 0.0f);

		/*moveSpots[0].x =  m_x + 10.0;
		moveSpots[0].y = m_y;
		moveSpots[0].z = m_z;*/

		//moveSpots[1] = { m_x + 10.0 , m_y , m_z };
		//moveSpots[2] = { m_x , m_y , m_z - 10.0 };
		//moveSpots[3] = { m_x - 10.0 , m_y , m_z };

		spotNum = rand() % 4;

		m_isColliding = false;
		m_isInteracting = false;
		m_inRange = false;
		m_haltMovement = false;
	

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

	
	local_world *= XMMatrixScaling(m_scale_x, m_scale_y, m_scale_z);
	local_world *= XMMatrixRotationRollPitchYaw(m_xangle, m_yangle, m_zangle);
	local_world *= XMMatrixTranslation(m_x, m_y, m_z);
	
	local_world *= *world;
	
	if (m_p_asset) m_p_asset->Draw(&local_world, view, projection);
	if (m_e_asset) m_e_asset->Draw(&local_world, view, projection);
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
	}
	else if (m_w_asset)
	{
		v = XMVectorSet(XMVectorGetX(m_w_asset->box->GetColliderPos()), XMVectorGetY(m_w_asset->box->GetColliderPos()), XMVectorGetZ(m_w_asset->box->GetColliderPos()), 0.0f);	
	}
	else if (m_e_asset)
	{
		v = XMVectorSet(XMVectorGetX(m_e_asset->box->GetColliderPos()), XMVectorGetY(m_e_asset->box->GetColliderPos()), XMVectorGetZ(m_e_asset->box->GetColliderPos()), 0.0f);
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

//bool SceneNode::CheckCollision(SceneNode* compare_tree)
//{
//	return CheckCollision(compare_tree, this);
//}

bool SceneNode::CheckCollision(SceneNode* compare_tree)
{
	if (this == compare_tree)
	{
		return false;
	}

	CalculateBoxCollisionDetails(compare_tree);

	if (x1 < x2 + l2 && x1 + l1 > x2)
	{
		if (y1 > y2 - h2 && y1 - h1 < y2)
		{
			if (z1 < z2 + b2 && z1 + b1 > z2)
			{
				m_isColliding = true;
			}
			else
			{
				m_isColliding = false;
			}
		}
		else
		{
			m_isColliding = false;
		}
	}
	else
	{
		m_isColliding = false;
	}

	
		/*for (int i = 0; i < compare_tree->m_children.size(); i++)
		{
			if (CheckCollision(compare_tree->m_children[i], object_tree_root));
		}

		for (int i = 0; i < m_children.size(); i++)
		{
			if (m_children[i]->CheckCollision(compare_tree, object_tree_root));
		}*/
	

}

//bool SceneNode::CheckActionCollision(SceneNode* compare_tree)
//{
//	return CheckActionCollision(compare_tree, this);
//}

bool SceneNode::CheckActionCollision(SceneNode* compare_tree)
{
	if (this == compare_tree)
	{
		return false;
	}

	CalculateSphereCollisionDetails(compare_tree);

	if (main_dist - 1.0 < sum_radius /** sum_radius*/)
	{
		m_isInteracting = true;
	}
	else
	{
		m_isInteracting = false;
	}

	/*for (int i = 0; i < compare_tree->m_children.size(); i++)
	{
	if (CheckCollision(compare_tree->m_children[i], object_tree_root));
	}

	for (int i = 0; i < m_children.size(); i++)
	{
	if (m_children[i]->CheckCollision(compare_tree, object_tree_root));
	}*/


}

bool SceneNode::CheckNodeBottomCollision(SceneNode* compare_tree)
{
	if (this == compare_tree)
	{
		return false;
	}

	CalculateBoxCollisionDetails(compare_tree);

	if (x1 < x2 + l2 && x1 + l1 > x2)
	{
		if (y1 - h1 > y2 && y1 - h1 < y2 + 0.005f)
		{
			if (z1 < z2 + b2 && z1 + b1 > z2)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	/*for (int i = 0; i < compare_tree->m_children.size(); i++)
	{
	if (CheckCollision(compare_tree->m_children[i], object_tree_root));
	}

	for (int i = 0; i < m_children.size(); i++)
	{
	if (m_children[i]->CheckCollision(compare_tree, object_tree_root));
	}*/


}

void SceneNode::CalculateBoxCollisionDetails(SceneNode* compare_tree)
{
	XMVECTOR v1 = GetWorldColliderCentrePos();
	XMVECTOR v2 = compare_tree->GetWorldColliderCentrePos();

	if (m_p_asset)
	{
		CalculateBoxDimensions1(v1, m_p_asset);
	}

	else if (m_e_asset)
	{
		CalculateBoxDimensions1(v1, m_e_asset);
	}
	else if (m_w_asset)
	{
		CalculateBoxDimensions1(v1, m_w_asset);
	}
	else if (m_s_asset)
	{
		CalculateBoxDimensions1(v1, m_s_asset);
	}
	else if (m_d_asset)
	{
		CalculateBoxDimensions1(v1, m_d_asset);
	}


	if (compare_tree->m_p_asset)
	{
		CalculateBoxDimensions2(v2, compare_tree->m_p_asset);
	}

	else if (compare_tree->m_e_asset)
	{
		CalculateBoxDimensions2(v2, compare_tree->m_e_asset);

	}
	else if (compare_tree->m_w_asset)
	{
		CalculateBoxDimensions2(v2, compare_tree->m_w_asset);
	}
	else if (compare_tree->m_s_asset)
	{
		CalculateBoxDimensions2(v2, compare_tree->m_s_asset);
	}
	else if (compare_tree->m_d_asset)
	{
		CalculateBoxDimensions2(v2, compare_tree->m_d_asset);
	}


	/*dist_x = x2 - x1;
	dist_y = y2 - y1;
	dist_z = z2 - z1;

	main_dist = sqrt((dist_x * dist_x) + (dist_y * dist_y) + (dist_z * dist_z));

	sum_radius = r1 + r2;*/


}

void SceneNode::CalculateSphereCollisionDetails(SceneNode* compare_tree)
{
	XMVECTOR v1 = GetWorldColliderCentrePos();
	XMVECTOR v2 = compare_tree->GetWorldColliderCentrePos();

	if (m_p_asset)
	{
		CalculateSphereDimensions1(v1, m_p_asset);
	}

	else if (m_e_asset)
	{
		CalculateSphereDimensions1(v1, m_e_asset);
	}
	else if (m_w_asset)
	{
		CalculateSphereDimensions1(v1, m_w_asset);
	}
	else if (m_s_asset)
	{
		CalculateSphereDimensions1(v1, m_s_asset);
	}
	else if (m_d_asset)
	{
		CalculateSphereDimensions1(v1, m_d_asset);
	}


	if (compare_tree->m_p_asset)
	{
		CalculateSphereDimensions2(v2, compare_tree->m_p_asset);
	}

	else if (compare_tree->m_e_asset)
	{
		CalculateSphereDimensions2(v2, compare_tree->m_e_asset);

	}
	else if (compare_tree->m_w_asset)
	{
		CalculateSphereDimensions2(v2, compare_tree->m_w_asset);
	}
	else if (compare_tree->m_s_asset)
	{
		CalculateSphereDimensions2(v2, compare_tree->m_s_asset);
	}
	else if (compare_tree->m_d_asset)
	{
		CalculateSphereDimensions2(v2, compare_tree->m_d_asset);
	}


	dist_x = x2 - x1;
	dist_y = y2 - y1;
	dist_z = z2 - z1;

	main_dist = sqrt((dist_x * dist_x) + (dist_y * dist_y) + (dist_z * dist_z));

	sum_radius = r1 + r2;

	
}

void SceneNode::CalculateBoxDimensions1(XMVECTOR v, Asset* obj)
{
	x1 = XMVectorGetX(v) - (obj->box->GetLength(obj->GetXScale()) / 2);
	y1 = XMVectorGetY(v) + (obj->box->GetHeight(obj->GetYScale()) / 2);
	z1 = XMVectorGetZ(v) - (obj->box->GetBreadth(obj->GetZScale()) / 2);

	l1 = obj->box->GetLength(obj->GetXScale());
	h1 = obj->box->GetHeight(obj->GetYScale());
	b1 = obj->box->GetBreadth(obj->GetZScale());
}

void SceneNode::CalculateSphereDimensions1(XMVECTOR v, Asset* obj)
{
	x1 = XMVectorGetX(v);
	y1 = XMVectorGetY(v);
	z1 = XMVectorGetZ(v);

	r1 = obj->box->GetColliderRadius();
}

void SceneNode::CalculateBoxDimensions2(XMVECTOR v, Asset* obj)
{
	x2 = XMVectorGetX(v) - (obj->box->GetLength(obj->GetXScale()) / 2);
	y2 = XMVectorGetY(v) + (obj->box->GetHeight(obj->GetYScale()) / 2);
	z2 = XMVectorGetZ(v) - (obj->box->GetBreadth(obj->GetZScale()) / 2);

	l2 = obj->box->GetLength(obj->GetXScale());
	h2 = obj->box->GetHeight(obj->GetYScale());
	b2 = obj->box->GetBreadth(obj->GetZScale());
}

void SceneNode::CalculateSphereDimensions2(XMVECTOR v, Asset* obj)
{
	x2 = XMVectorGetX(v);
	y2 = XMVectorGetY(v);
	z2 = XMVectorGetZ(v);

	r2 = obj->box->GetColliderRadius();
}


void SceneNode::MoveAsset(float x_dist, float y_dist, float z_dist)
{
	if (m_haltMovement == false)
	{
		m_x += x_dist;
		m_y += y_dist;
		m_z += z_dist;
	}
	
}

void SceneNode::LookAt()
{
	if (m_inRange == false)
	{
		lookAt_dist_x = m_x - XMVectorGetX(moveSpots[spotNum]);
		lookAt_dist_z = m_z - XMVectorGetZ(moveSpots[spotNum]);

		if (fabs(lookAt_dist_x) <= 0.2f && fabs(lookAt_dist_z) <= 0.2f)
		{
			float prevSpotNum = spotNum;
			spotNum = rand() % 4;

			if (spotNum == prevSpotNum)
			{
				spotNum += 1;

				if (spotNum > 3)
				{
					spotNum = 0;
				}
			}


			lookAt_dist_x = m_x - XMVectorGetX(moveSpots[spotNum]);
			lookAt_dist_z = m_z - XMVectorGetZ(moveSpots[spotNum]);
		}
	}

	m_yangle = -atan2(lookAt_dist_x, -lookAt_dist_z);

}

void SceneNode::CheckInRange(XMVECTOR other_pos)
{
	float dist_x = m_x - XMVectorGetX(other_pos);
	float dist_z = m_z - XMVectorGetZ(other_pos);

	if (fabs(dist_x) <= 10.0f && fabs(dist_z) <= 10.0f)
	{
		m_inRange = true;

		if (fabs(dist_x) <= 3.0f && fabs(dist_z) <= 3.0f)
		{
			m_haltMovement = true;
		}
		else
		{
			m_haltMovement = false;
		}
		

		lookAt_dist_x = dist_x;
		lookAt_dist_z = dist_z;
	}
	else
	{
		m_inRange = false;
	}


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

void SceneNode::SetYAngle(float angle)
{
	m_yangle = angle;
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

void SceneNode::SetInteractState(bool state)
{
	m_isInteracting = state;
}

bool SceneNode::IsInteracting()
{
	return m_isInteracting;
}

void SceneNode::SetHaltState(bool state)
{
	m_haltMovement = state;
}

bool SceneNode::isHalted()
{
	return m_haltMovement;
}

int SceneNode::GetChildrenSize()
{
	return m_children.size();
}

vector<SceneNode*> SceneNode::GetChildren()
{
	return m_children;
}

XMVECTOR SceneNode::GetWorldColliderCentrePos()
{
	return XMVectorSet(m_collider_centre_x, m_collider_centre_y, m_collider_centre_z, 0.0f);
}

XMVECTOR SceneNode::GetWorldPos()
{
	return XMVectorSet(m_x, m_y, m_z, 0.0f);
}

SceneNode* SceneNode::GetEquippedWeaponNode()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i]->m_w_asset)
		{
			return m_children[i];
		}
	}
}

SceneNode* SceneNode::GetPushingCrate()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i]->m_d_asset)
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