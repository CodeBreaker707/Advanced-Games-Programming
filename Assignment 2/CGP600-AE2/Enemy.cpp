#include "Enemy.h"

Enemy::Enemy(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x_pos, float y_pos, float z_pos, float x_scale, float y_scale, float z_scale)
{
	InitialiseAsset(D3DDevice, ImmediateContext, x_pos, y_pos, z_pos, x_scale, y_scale, z_scale);

	LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");

	m_health = 1000.0f;

}

Enemy::~Enemy()
{

}

void Enemy::SetEnemyHealth(float amount)
{
	m_health = amount;
}

float Enemy::GetEnemyHealth()
{
	return m_health;
}