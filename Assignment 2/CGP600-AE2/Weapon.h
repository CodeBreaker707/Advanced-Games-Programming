#pragma once

#include "Asset.h"

class Weapon : public Asset
{
private:

	// A boolean to check if the weapon
	// is equipped by a node
	bool m_weaponEquipped;
	
	// A boolean to check if it's in
	// attack state
	bool m_Attacked;

	// A boolean to check if the attack
	// is completed
	bool m_attackComplete;

	// A boolean to check if the weapon had
	// hit something
	bool m_weaponHit;


public:

	// Constructor and Destructor
	Weapon(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale);
	~Weapon();

	// SET AND GET FUNCTIONS FOR ALL BOOLEANS

	void SetWeaponEquipState(bool state);
	bool GetWeaponEquipState();

	void SetWeaponHitState(bool state);
	bool GetWeaponHitState();

	void SetWeaponAttackedState(bool state);
	bool GetWeaponAttackedState();

	void SetWeaponAttackCompleteState(bool state);
	bool GetWeaponAttackCompleteState();
	

};
