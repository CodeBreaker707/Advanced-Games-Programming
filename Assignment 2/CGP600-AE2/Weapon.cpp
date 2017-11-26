#include "Weapon.h"

Weapon::Weapon(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x, float y, float z)
{
	InitialiseAsset(D3DDevice, ImmediateContext, x, y, z);

	LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");

	m_weapon_durability = 3;

	m_cur_z = 0.0f;
	
	m_weaponEquipped = false;
	m_Attacked = false;

}

Weapon::~Weapon()
{

}

void Weapon::SetCurPos()
{
	m_cur_z = GetZPos();
}

float Weapon::GetCurPos()
{
	return m_cur_z;
}

void Weapon::SetWeaponDurability(int value)
{
	m_weapon_durability = value;
}

void Weapon::SetWeaponEquipState(bool state)
{
	m_weaponEquipped = state;
}

void Weapon::SetWeaponAttackedState(bool state)
{
	m_Attacked = state;
}

int Weapon::GetWeaponDurability()
{
	return m_weapon_durability;
}

bool Weapon::GetWeaponEquipState()
{
	return m_weaponEquipped;
}

bool Weapon::GetWeaponAttackedState()
{
	return m_Attacked;
}