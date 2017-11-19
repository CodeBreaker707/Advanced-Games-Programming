#pragma once

#include "Asset.h"

class Player : public Asset
{
private:

	float m_move_speed;
	float m_speed_multiplier;

	bool m_isJumping;
	bool m_onGround;

	float m_gravity_speed;
	float m_jump_height;
	float m_jump_speed;

public:

	Player(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x, float y, float z);
	~Player();

	void OnAirBehaviour();
	void PullDown();

	void SetOnGroundState(bool state);
	bool GetOnGroundState();

	void SetJumpState(bool state);
	bool GetJumpState();

	void SetPlayerMoveSpeed(float speed);
	void SetSpeedMultiplier(float amount);

	float GetPlayerMoveSpeed();

	//float GetSpeedMultiplier();




};
