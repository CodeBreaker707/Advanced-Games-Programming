#pragma once

#include "Collider3D.h"
#include "Light.h"

class Asset
{
private:

	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;

	ObjFileModel* m_pObject;

	Light* m_light;
	
	bool m_isColliding = false;

	ID3D11VertexShader* m_pVShader;
	ID3D11PixelShader*  m_pPShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11ShaderResourceView* m_pTexture0;
	//ID3D11ShaderResourceView* m_pTexture1;
	ID3D11SamplerState* m_pSampler0;

	float m_x, m_y, m_z;
	float m_prev_x, m_prev_y, m_prev_z;
	float m_xangle, m_zangle, m_yangle;
	float m_scale_x, m_scale_y, m_scale_z;

public:

	Collider3D* collider;

	Asset();
	~Asset();

	void InitialiseAsset(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x_scale, float y_scale, float z_scale);

	int LoadObjModel(char* fileName, char* textureFile);

	/*void RotateAsset(float pitch_degrees, float yaw_degrees, float roll_degrees);
	
	void ScaleAsset(float x_scale, float y_scale, float z_scale);

	void MoveAsset(float x_dist, float y_dist, float z_dist);

	void SetXPos(float x);
	void SetYPos(float y);
	void SetZPos(float z);

	float GetXPos();
	float GetYPos();
	float GetZPos();

	float GetXAngle();
	float GetYAngle();
	float GetZAngle();

	

	void RestrictPos(bool isCollding);
	void UpdatePos(bool isColliding);

	bool CheckCollision(Asset* obj);
*/
	//void SetCollideState(bool state);
	//bool IsColliding();

	//XMMATRIX GetWorldMatrix();

	//XMVECTOR GetColliderWorldSpacePos();

	float GetXScale();
	float GetYScale();
	float GetZScale();

	void Draw(XMMATRIX* world, XMMATRIX* view, XMMATRIX* projection);

};
