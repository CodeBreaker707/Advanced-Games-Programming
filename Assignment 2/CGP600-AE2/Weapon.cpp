#include "Weapon.h"

// Constructor
Weapon::Weapon(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale)
{
	// Initialising the asset
	InitialiseAsset(D3DDevice, ImmediateContext, assetFile, textureFile, x_scale, y_scale, z_scale);
	
	// Initialising the booleans
	m_weaponEquipped = false;
	m_Attacked = false;
	m_attackComplete = false;
	m_weaponHit = false;

}

// Destructor
Weapon::~Weapon()
{

}

// SET AND GET FUNCTIONS OF ALL BOOLEANS

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