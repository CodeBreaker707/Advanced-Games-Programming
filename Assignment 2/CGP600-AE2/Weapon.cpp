#include "Weapon.h"

Weapon::Weapon(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x, float y, float z)
{
	InitialiseAsset(D3DDevice, ImmediateContext, x, y, z);

	LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");

	m_weapon_durability = 3;
}

Weapon::~Weapon()
{

}

void Weapon::SetWeaponDurability(int value)
{
	m_weapon_durability = value;
}

bool Weapon::GetWeaponDurability()
{
	return m_weapon_durability;
}