#include "Asset.h"

struct ASSET_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProjection; // 64 bytes
	XMVECTOR directional_light_vector; // 16 bytes
	XMVECTOR directional_light_colour; // 16 bytes
	XMVECTOR ambient_light_colour; // 16 bytes
}; // 112 bytes

Asset::Asset()
{
	
}

void Asset::InitialiseAsset(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x, float y, float z)
{
	m_pD3DDevice = D3DDevice;
	m_pImmediateContext = ImmediateContext;

	m_x = x;
	m_y = y;
	m_z = z;

	m_prev_x = m_x;
	m_prev_y = m_y;
	m_prev_z = m_z;

	m_xangle = 0.0f;
	m_zangle = 0.0f;
	m_yangle = 0.0f;

	m_scale_x = 1.0f;
	m_scale_y = 1.0f;
	m_scale_z = 1.0f;

	box = new Collider3D();
}

Asset::~Asset()
{
	delete m_pObject;

	m_pTexture0->Release();
	m_pSampler0->Release();
	m_pInputLayout->Release();
	m_pVShader->Release();
	m_pPShader->Release();
	m_pConstantBuffer->Release();
}

int Asset::LoadObjModel(char* fileName, char* textureFile)
{
	m_pObject = new ObjFileModel(fileName, m_pD3DDevice, m_pImmediateContext);

	if (m_pObject->filename == "FILE NOT LOADED") return S_FALSE;

	HRESULT hr = S_OK;

	// Create Constant Buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; // Can use UpdateSubresource() to update
	constant_buffer_desc.ByteWidth = sizeof(ASSET_CONSTANT_BUFFER); // MUST be a multiple of 16, calculate from CB struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Use as a constant buffer

	hr = m_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer);

	ID3DBlob *VS, *PS, *error;
	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	if (error != 0) // Check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))
		{
			return hr;
		};
	}

	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))
		{
			return hr;
		};
	}

	hr = m_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVShader);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPShader);

	if (FAILED(hr))
	{
		return hr;
	}

	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);

	// Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = m_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);

	if (FAILED(hr))
	{
		return hr;
	}

	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	D3DX11CreateShaderResourceViewFromFile(m_pD3DDevice, textureFile, NULL, NULL, &m_pTexture0, NULL);

	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sampler_desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	m_pD3DDevice->CreateSamplerState(&sampler_desc, &m_pSampler0);

	box->CalculateColliderCentre(m_pObject);
	box->CalculateDimensions(m_pObject);

}

void Asset::RotateAsset(float pitch_degrees, float yaw_degrees, float roll_degrees)
{
	m_xangle += XMConvertToRadians(pitch_degrees);
	m_yangle += XMConvertToRadians(yaw_degrees);
	m_zangle += XMConvertToRadians(roll_degrees);
}

void Asset::ScaleAsset(float x_scale, float y_scale, float z_scale)
{
	m_scale_x = x_scale;
	m_scale_y = y_scale;
	m_scale_z = z_scale;
}

void Asset::MoveAsset(float x_dist, float y_dist, float z_dist)
{
	m_x += x_dist;
	m_y += y_dist;
	m_z += z_dist;

}


bool Asset::CheckCollision(Asset* obj)
{
	if (obj == this)
	{
		return false;
	}

	XMVECTOR cur_pos = GetColliderWorldSpacePos();
	XMVECTOR other_pos = obj->GetColliderWorldSpacePos();

	float x1 = XMVectorGetX(cur_pos) - (box->GetLength(m_scale_x) / 2  );
	float y1 = XMVectorGetY(cur_pos) + (box->GetHeight(m_scale_y) / 2  );
	float z1 = XMVectorGetZ(cur_pos) - (box->GetBreadth(m_scale_z) / 2 );

	float l1 = box->GetLength(m_scale_x);
	float h1 = box->GetHeight(m_scale_y);
	float b1 = box->GetBreadth(m_scale_z);

	float x2 = XMVectorGetX(other_pos) - (obj->box->GetLength(obj->GetXScale()) / 2 );
	float y2 = XMVectorGetY(other_pos) + (obj->box->GetHeight(obj->GetYScale()) / 2 );
	float z2 = XMVectorGetZ(other_pos) - (obj->box->GetBreadth(obj->GetZScale()) / 2 );

	float l2 = obj->box->GetLength(obj->GetXScale());
	float h2 = obj->box->GetHeight(obj->GetYScale());
	float b2 = obj->box->GetBreadth(obj->GetZScale());


	if (x1 < x2 + l2 && x1 + l1 > x2)
	{
		if (y1 > y2 - h2 && y1 - h1 < y2)
		{
			if (z1 < z2 + b2 && z1 + b1 > z2)
			{
				m_isColliding = true;
				
			}
			else
			{
				m_isColliding = false;
			}
		}
		else
		{
			m_isColliding = false;
		}
	}
	else
	{
		m_isColliding = false;
	}
}

void Asset::RestrictPos(bool isColliding)
{
	if (isColliding == true)
	{
		m_x = m_prev_x;
		m_y = m_prev_y;
		m_z = m_prev_z;
	}
}

void Asset::UpdatePos(bool isColliding)
{
	if (isColliding == false)
	{
		m_prev_x = m_x;
		m_prev_y = m_y;
		m_prev_z = m_z;
	}

}

XMVECTOR Asset::GetColliderWorldSpacePos()
{
	XMMATRIX world;

	world = XMMatrixScaling(m_scale_x, m_scale_y, m_scale_z);
	world *= XMMatrixRotationRollPitchYaw(m_xangle, m_yangle, m_zangle);
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	XMVECTOR offset;

	offset = box->GetColliderPos();

	XMVECTOR new_pos = XMVector3Transform(offset, world);

	return new_pos;
}

void Asset::Draw(XMMATRIX* view, XMMATRIX* projection)
{
	//UpdateRot(0.0f, 0.0001f, 0.0001f);

	XMMATRIX world;

	world = XMMatrixScaling(m_scale_x, m_scale_y, m_scale_z);
	world *= XMMatrixRotationRollPitchYaw(m_xangle, m_yangle, m_zangle);
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	m_directional_light_shines_from = XMVectorSet(-1.0f, 1.0f, -1.0f, 0.0f);
	m_directional_light_colour = XMVectorSet(1.8f, 1.5f, 0.0f, 1.0f);
	m_ambient_light_colour = XMVectorSet(0.3f, 0.3f, 0.3f, 1.0f);

	XMMATRIX transpose;
	ASSET_CONSTANT_BUFFER model_cb_values;
	model_cb_values.WorldViewProjection = world * (*view) * (*projection);

	transpose = XMMatrixTranspose(world);

	model_cb_values.directional_light_colour = m_directional_light_colour;
	model_cb_values.ambient_light_colour = m_ambient_light_colour;
	model_cb_values.directional_light_vector = XMVector3Transform(m_directional_light_shines_from, transpose);
	model_cb_values.directional_light_vector = XMVector3Normalize(model_cb_values.directional_light_vector);

	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &model_cb_values, 0, 0);

	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture0);
	//m_pImmediateContext->PSSetShaderResources(1, 1, &m_pTexture1);

	m_pObject->Draw();
}

void Asset::SetXPos(float x)
{
	m_x = x;
}

void Asset::SetYPos(float y)
{
	m_y = y;
}

void Asset::SetZPos(float z)
{
	m_z = z;
}

float Asset::GetXPos()
{
	return m_x;
}

float Asset::GetYPos()
{
	return m_y;
}

float Asset::GetZPos()
{
	return m_z;
}

float Asset::GetXAngle()
{
	return m_xangle;
}

float Asset::GetYAngle()
{
	return m_yangle;
}

float Asset::GetZAngle()
{
	return m_zangle;
}

float Asset::GetXScale()
{
	return m_scale_x;
}

float Asset::GetYScale()
{
	return m_scale_y;
}

float Asset::GetZScale()
{
	return m_scale_z;
}

void Asset::SetCollideState(bool state)
{
	m_isColliding = state;
}

bool Asset::IsColliding()
{
	return m_isColliding;
}

