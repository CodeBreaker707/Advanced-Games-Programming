#pragma once

#include "Asset.h"

class Dynamic : public Asset
{
private:
	
	bool m_isPushing;

public:

	Dynamic(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale);
	~Dynamic();

	//void SetPushState(bool state);
	//bool GetPushState();

};
