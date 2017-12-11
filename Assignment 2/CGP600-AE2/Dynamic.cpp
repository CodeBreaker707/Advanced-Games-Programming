#include "Dynamic.h"

Dynamic::Dynamic(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale)
{
	InitialiseAsset(D3DDevice, ImmediateContext, assetFile, textureFile, x_scale, y_scale, z_scale);

	//LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");

	m_isPushing = false;
}

Dynamic::~Dynamic()
{

}