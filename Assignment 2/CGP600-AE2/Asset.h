#pragma once

#include "objfilemodel.h"

class Asset
{
private:

	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;

	ObjFileModel* m_pObject;
	ID3D11VertexShader* m_pVShader;
	ID3D11PixelShader* m_pPShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11ShaderResourceView* m_pTexture0;
	//ID3D11ShaderResourceView* m_pTexture1;
	ID3D11SamplerState* m_pSampler0;


	XMVECTOR m_directional_light_shines_from;
	XMVECTOR m_directional_light_colour;
	XMVECTOR m_ambient_light_colour;

	float m_x, m_y, m_z;
	float m_xangle, m_zangle, m_yangle;
	float m_scale;

public:
	Asset(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext);
	~Asset();

	int LoadObjModel(char* fileName, char* textureFile);

	void UpdateRot(float pitch_degrees, float yaw_degrees, float roll_degrees);

	void MoveForward(float z_dist);
	void MoveSideways(float x_dist);

	void Draw(XMMATRIX* view, XMMATRIX* projection);

};
