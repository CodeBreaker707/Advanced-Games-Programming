#pragma once

// Including the Asset class
// for loading in the model and to draw it
#include "Asset.h"

class Dynamic : public Asset
{
public:

	// Class Constructor and Destructor
	Dynamic(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale);
	~Dynamic();

};
