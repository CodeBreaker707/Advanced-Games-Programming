#pragma once

#include "Asset.h"

class Weapon : public Asset
{
private:


	bool m_pickedUp;
	bool m_weaponEquipped;
	bool m_Attacked;
	bool m_attackComplete;
	bool m_weaponHit;

	float m_cur_z;

public:

	Weapon(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale);
	~Weapon();

	void SetWeaponDurability(int value);
	int GetWeaponDurability();

	void SetCurPos();
	float GetCurPos();

	void SetWeaponEquipState(bool state);
	bool GetWeaponEquipState();

	void SetWeaponHitState(bool state);
	bool GetWeaponHitState();

	void SetWeaponAttackedState(bool state);
	bool GetWeaponAttackedState();

	void SetWeaponAttackCompleteState(bool state);
	bool GetWeaponAttackCompleteState();
	

};
