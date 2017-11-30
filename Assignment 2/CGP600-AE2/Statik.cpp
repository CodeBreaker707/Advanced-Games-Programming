#include "Statik.h"

Statik::Statik(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext)
{
	InitialiseAsset(D3DDevice, ImmediateContext);

	LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");
}