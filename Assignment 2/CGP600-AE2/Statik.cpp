#include "Statik.h"

Statik::Statik(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x_pos, float y_pos, float z_pos, float x_scale, float y_scale, float z_scale)
{
	InitialiseAsset(D3DDevice, ImmediateContext, x_pos, y_pos, z_pos, x_scale, y_scale, z_scale);

	LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");
}