#pragma once

#include "Asset.h"

class Player : public Asset
{
private:

	float m_pos_x, m_pos_y, m_pos_z;

	float m_move_speed;

public:

	Player(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x, float y, float z);
	~Player();


};
