#pragma once

#include "Asset.h"

class Enemy : public Asset
{
private:

	float m_health;

public:
	Enemy(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x_pos, float y_pos, float z_pos, float x_scale, float y_scale, float z_scale);
	~Enemy();

	void SetEnemyHealth(float amount);
	float GetEnemyHealth();


};
