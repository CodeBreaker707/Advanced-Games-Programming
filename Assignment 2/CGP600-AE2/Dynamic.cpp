#include "Dynamic.h"

Dynamic::Dynamic(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext)
{
	InitialiseAsset(D3DDevice, ImmediateContext);

	LoadObjModel("Assets/cube2.obj", "Assets/tile.bmp");

	m_isPushing = false;
}

Dynamic::~Dynamic()
{

}

void Dynamic::SetPushState(bool state)
{
	m_isPushing = state;
}

bool Dynamic::GetPushState()
{
	return m_isPushing;
}