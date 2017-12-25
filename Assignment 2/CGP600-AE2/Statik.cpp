#include "Statik.h"

// Constructor
Statik::Statik(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale)
{
	// Initialising the asset
	InitialiseAsset(D3DDevice, ImmediateContext, assetFile, textureFile, x_scale, y_scale, z_scale);

}

// Destructor
Statik::~Statik()
{

}