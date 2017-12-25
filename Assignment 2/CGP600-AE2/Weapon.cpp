#include "Weapon.h"

Weapon::Weapon(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale)
{
	InitialiseAsset(D3DDevice, ImmediateContext, assetFile, textureFile, x_scale, y_scale, z_scale);

	//LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");

	m_cur_z = 0.0f;
	
	m_weaponEquipped = false;
	m_Attacked = false;
	m_attackComplete = false;
	m_weaponHit = false;

}

Weapon::~Weapon()
{

}

void Weapon::SetCurPos()
{
	//m_cur_z = GetZPos();
}

float Weapon::GetCurPos()
{
	return m_cur_z;
}

void Weapon::SetWeaponEquipState(bool state)
{
	m_weaponEquipped = state;
}

void Weapon::SetWeaponAttackedState(bool state)
{
	m_Attacked = state;
}

void Weapon::SetWeaponAttackCompleteState(bool state)
{
	m_attackComplete = state;
}

void Weapon::SetWeaponHitState(bool state)
{
	m_weaponHit = state;
}

bool Weapon::GetWeaponEquipState()
{
	return m_weaponEquipped;
}

bool Weapon::GetWeaponAttackedState()
{
	return m_Attacked;
}

bool Weapon::GetWeaponAttackCompleteState()
{
	return m_attackComplete;
}

bool Weapon::GetWeaponHitState()
{
	return m_weaponHit;
}