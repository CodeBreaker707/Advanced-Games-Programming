#include "Enemy.h"

Enemy::Enemy(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x_scale, float y_scale, float z_scale)
{
	InitialiseAsset(D3DDevice, ImmediateContext, x_scale,  y_scale, z_scale);

	LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");

	m_health = 500.0f;

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