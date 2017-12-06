#include "Player.h"

Player::Player(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x_scale, float y_scale, float z_scale)
{
	InitialiseAsset(D3DDevice, ImmediateContext, x_scale, y_scale, z_scale);

	LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");

	m_health = 500.0f;

	m_isJumping = false;
	m_onGround = false;
	m_carryingWeapon = false;
	m_isPushing = false;

	m_move_speed = 0.001f;
	m_speed_multiplier = 1.0f;
	m_jump_speed = 0.0010f;
	m_gravity_speed = 0.0015f;
	m_jump_height = 4.5f;

	//m_lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	//m_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	//m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//m_position = XMVectorSet(x, y, z, 0.0f);

}

//bool Player::CheckPlayerFeetonGround(Asset* obj)
//{
//	/*if (obj == this)
//	{
//		return false;
//	}
//
//	XMVECTOR cur_pos = GetColliderWorldSpacePos();
//	XMVECTOR other_pos = obj->GetColliderWorldSpacePos();*/
//
//	/*float x1 = XMVectorGetX(cur_pos) - (box->GetLength(GetXScale()) / 2);
//	float y1 = XMVectorGetY(cur_pos) + (box->GetHeight(GetYScale()) / 2);
//	float z1 = XMVectorGetZ(cur_pos) - (box->GetBreadth(GetZScale()) / 2);
//
//	float l1 = box->GetLength(GetXScale());
//	float h1 = box->GetHeight(GetYScale());
//	float b1 = box->GetBreadth(GetZScale());
//
//	float x2 = XMVectorGetX(other_pos) - (obj->box->GetLength(obj->GetXScale()) / 2);
//	float y2 = XMVectorGetY(other_pos) + (obj->box->GetHeight(obj->GetYScale()) / 2);
//	float z2 = XMVectorGetZ(other_pos) - (obj->box->GetBreadth(obj->GetZScale()) / 2);
//
//	float l2 = obj->box->GetLength(obj->GetXScale());
//	float h2 = obj->box->GetHeight(obj->GetYScale());
//	float b2 = obj->box->GetBreadth(obj->GetZScale());
//
//
//	if (x1 < x2 + l2 && x1 + l1 > x2)
//	{
//		if (y1 - h1 > y2 && y1 - h1 < y2 + 0.005f)
//		{
//			if (z1 < z2 + b2 && z1 + b1 > z2)
//			{
//				return true;
//
//			}
//			else
//			{
//				return false;
//			}
//		}
//		else
//		{
//			return false;
//		}
//	}
//	else
//	{
//		return false;
//	}*/
//		
//
//}
//
//void Player::JumpPlayer()
//{
//	if (m_isJumping == true)
//	{
//		MoveAsset(0.0f, m_jump_speed, 0.0f);
//
//		if (GetYPos() > m_jump_height)
//		{
//			m_isJumping = false;
//		}
//
//	}
//
//}
//
//void Player::UpdateLookAt()
//{
//	float x = 0;
//	float z = 0;
//	//float z = GetZPos();
//
//	//XMMATRIX rot = XMMatrixRotationRollPitchYaw(GetXAngle(), GetYAngle(), GetZAngle());
//
//	x = (XMVectorGetX(m_position) * cos(GetYAngle())) + (XMVectorGetZ(m_position) * sin(GetYAngle()));
//	z = (XMVectorGetX(m_position) * -sin(GetYAngle())) + (XMVectorGetZ(m_position) * cos(GetYAngle()));
//
//	m_lookAt = XMVectorSet(x, GetYPos(), z, 0.0f);
//	m_lookAt = XMVector3Normalize(m_lookAt);
//
//	/*m_lookAt = XMVector3Transform(m_lookAt, rot);
//	m_lookAt = XMVector3Normalize(m_lookAt);*/
//
//	//m_lookAt = XMVectorSet(GetYAngle() , 0.0f, GetYAngle(), 0.0f);
//
//}
//
//void Player::UpdatePosVector()
//{
//	float x = GetXPos();
//	float y = GetYPos();
//	float z = GetZPos();
//
//	m_position = XMVectorSet(x , y , z , 0.0f);
//
//}
//
//void Player::MovePlayer(float dist)
//{
//	MoveAsset(sin(GetYAngle()) * dist, 0.0f, cos(GetYAngle()) * dist);
//
//	//m_position += (dist * m_lookAt);
//
//	//float x = XMVectorGetX(m_position);
//	//float z = XMVectorGetZ(m_position);
//
//	//SetXPos(x);
//	//SetZPos(z);
//
//}
//
//void Player::StrafePlayer(float dist)
//{
//	//MoveAsset(cos(GetYAngle()) * dist, 0.0f, sin(GetYAngle()) * dist);
//	
//	m_right = XMVector3Cross(m_lookAt, m_up);
//
//	m_position += (-dist * m_right);
//
//	float x = XMVectorGetX(m_position);
//	float z = XMVectorGetZ(m_position);
//
//	SetXPos(x);
//	SetZPos(z);
//
//}
//
//void Player::PullDown()
//{
//	if (m_onGround == false && m_isJumping == false)
//	{
//		MoveAsset(0.0f, -m_jump_speed, 0.0f);
//	}
//}

void Player::SetOnGroundState(bool state)
{
	m_onGround = state;
}

void Player::SetJumpState(bool state)
{
	m_isJumping = state;
}

void Player::SetWeaponCarryingState(bool state)
{
	m_carryingWeapon = state;
}

void Player::SetPlayerHealth(float amount)
{
	m_health = amount;
}

void Player::SetPlayerMoveSpeed(float speed)
{
	m_move_speed = speed;
}

void Player::SetJumpHeight(float value)
{
	m_jump_height = value;
}

void Player::SetSpeedMultiplier(float amount)
{
	m_speed_multiplier = amount;
}

void Player::SetPushState(bool state)
{
	m_isPushing = state;
}

bool Player::GetPushState()
{
	return m_isPushing;
}

bool Player::GetOnGroundState()
{
	return m_onGround;
}

bool Player::GetJumpState()
{
	return m_isJumping;
}

bool Player::GetWeaponCarryingState()
{
	return m_carryingWeapon;
}

float Player::GetPlayerHealth()
{
	return m_health;
}

float Player::GetPlayerMoveSpeed()
{
	return m_move_speed * m_speed_multiplier;
}

float Player::GetSpeedMultiplier()
{
	return m_speed_multiplier;
}

float Player::GetJumpSpeed()
{
	return m_jump_speed;
}

float Player::GetJumpHeight()
{
	return m_jump_height;
}