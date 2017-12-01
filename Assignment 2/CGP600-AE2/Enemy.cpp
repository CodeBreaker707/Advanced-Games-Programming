#include "Enemy.h"

Enemy::Enemy(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext)
{
	InitialiseAsset(D3DDevice, ImmediateContext);

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