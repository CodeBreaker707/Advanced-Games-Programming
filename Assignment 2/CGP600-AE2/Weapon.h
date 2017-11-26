#pragma once

#include "Asset.h"

class Weapon : public Asset
{
private:

	int m_weapon_durability;

	bool m_pickedUp;
	bool m_weaponEquipped;
	bool m_Attacked;

	float m_cur_z;

public:

	Weapon(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x, float y, float z);
	~Weapon();

	void SetWeaponDurability(int value);
	int GetWeaponDurability();

	void SetCurPos();
	float GetCurPos();

	void SetWeaponEquipState(bool state);
	bool GetWeaponEquipState();

	void SetWeaponAttackedState(bool state);
	bool GetWeaponAttackedState();
	

};
