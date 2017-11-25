#include "Statik.h"

Statik::Statik(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x, float y, float z)
{
	InitialiseAsset(D3DDevice, ImmediateContext, x, y, z);

	LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");
}