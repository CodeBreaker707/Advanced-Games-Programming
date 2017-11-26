#pragma once

#include "Asset.h"

class Player : public Asset
{
private:

	float m_move_speed;
	float m_speed_multiplier;

	XMVECTOR m_lookAt, m_right, m_up, m_position;

	bool m_isJumping;
	bool m_onGround;

	float m_gravity_speed;
	float m_jump_height;
	float m_jump_speed;

public:

	Player(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x_pos, float y_pos, float z_pos, float x_scale, float y_scale, float z_scale);
	~Player();

	void JumpPlayer();
	void PullDown();

	void UpdatePosVector();
	void UpdateLookAt();

	void MovePlayer(float dist);
	void StrafePlayer(float dist);

	bool CheckPlayerFeetonGround(Asset* obj);

	void SetOnGroundState(bool state);
	bool GetOnGroundState();

	void SetJumpState(bool state);
	bool GetJumpState();

	void SetPlayerMoveSpeed(float speed);
	void SetSpeedMultiplier(float amount);

	float GetPlayerMoveSpeed();

	//float GetSpeedMultiplier();




};
