#include "Player.h"

Player::Player(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x, float y, float z)
{
	InitialiseAsset(D3DDevice, ImmediateContext, x, y, z);

	LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");

	m_isJumping = false;
	m_onGround = false;

	m_move_speed = 0.001f;
	m_speed_multiplier = 1.0f;
	m_jump_speed = 0.0015f;
	m_gravity_speed = 0.0015f;
	m_jump_height = 4.5f;

}

void Player::OnAirBehaviour()
{
	if (m_isJumping == true)
	{
		MoveAsset(0.0f, m_jump_speed, 0.0f);

		if (GetYPos() > m_jump_height)
		{
			m_isJumping = false;
		}

	}
	/*else if(m_onGround == false)
	{
		MoveAsset(0.0f, -m_jump_speed, 0.0f);
	}*/
}

void Player::PullDown()
{
	//if (m_onGround == false)
	//{
		MoveAsset(0.0f, -m_jump_speed, 0.0f);
	//}
}

void Player::SetOnGroundState(bool state)
{
	m_onGround = state;
}

void Player::SetJumpState(bool state)
{
	m_isJumping = state;
}

void Player::SetPlayerMoveSpeed(float speed)
{
	m_move_speed = speed;
}

void Player::SetSpeedMultiplier(float amount)
{
	m_speed_multiplier = amount;
}

bool Player::GetOnGroundState()
{
	return m_onGround;
}

bool Player::GetJumpState()
{
	return m_isJumping;
}

float Player::GetPlayerMoveSpeed()
{
	return m_move_speed * m_speed_multiplier;
}