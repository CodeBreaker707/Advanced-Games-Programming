#pragma once

#include "Asset.h"

class Dynamic : public Asset
{
private:
	
	bool m_isPushing;

public:

	Dynamic(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext);
	~Dynamic();

	void SetPushState(bool state);
	bool GetPushState();

};
