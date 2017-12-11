#pragma once

#include "Asset.h"

class Statik : public Asset
{
private:

public:

	Statik(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale);
	~Statik();


};
