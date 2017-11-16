#pragma once

#include "objfilemodel.h"

class Model
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
	ID3D11ShaderResourceView* m_pTexture1;
	ID3D11SamplerState* m_pSampler0;


	XMVECTOR m_directional_light_shines_from;
	XMVECTOR m_directional_light_colour;
	XMVECTOR m_ambient_light_colour;

	float m_x, m_y, m_z;
	float dx, dz;
	float m_xangle, m_zangle, m_yangle;
	float m_scale;

	float m_bounding_sphere_centre_x, m_bounding_sphere_centre_y, m_bounding_sphere_centre_z;
	float m_bounding_sphere_radius;

	void CalculateModelCentrePoint();
	void CalculateBoundingSphereRadius();

public:
	Model(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x, float y, float z);
	~Model();

	int LoadObjModel(char* fileName);

	void UpdateRot(float pitch_degrees, float yaw_degrees, float roll_degrees);

	void LookAt_XZ(float x, float z);

	void MoveForward(float distance);

	XMVECTOR GetBoundingSphereWorldSpacePosition();

	float GetBoundingSphereRadius();

	bool CheckCollision(Model* obj);

	float GetX();
	float GetZ();

	void SetLightColour(float r, float g, float b);

	void Draw(XMMATRIX* view, XMMATRIX* projection);

};
