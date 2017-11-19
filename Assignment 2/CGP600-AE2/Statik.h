#pragma once

#include "Asset.h"

class Statik : public Asset
{
private:

public:

	Statik(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x, float y, float z);
	~Statik();


};
