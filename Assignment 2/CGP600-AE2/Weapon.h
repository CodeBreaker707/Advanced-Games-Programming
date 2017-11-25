#pragma once

#include "Asset.h"

class Weapon : public Asset
{
private:

	int m_weapon_durability;

	bool pickedUp;

public:

	Weapon(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x, float y, float z);
	~Weapon();

	void SetWeaponDurability(int value);
	bool GetWeaponDurability();

};
