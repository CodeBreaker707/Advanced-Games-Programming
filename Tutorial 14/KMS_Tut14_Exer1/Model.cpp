#include "Model.h"

struct MODEL_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProjection; // 64 bytes
	XMVECTOR directional_light_vector; // 16 bytes
	XMVECTOR directional_light_colour; // 16 bytes
	XMVECTOR ambient_light_colour; // 16 bytes
}; // 112 bytes

Model::Model(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, float x, float y, float z)
{
	m_pD3DDevice = D3DDevice;
	m_pImmediateContext = ImmediateContext;

	m_x = x;
	m_y = y;
	m_z = z;
	m_xangle = 0.0f;
	m_zangle = 0.0f;
	m_yangle = 0.0f;

	m_scale = 1.0f;
}

Model::~Model()
{
	delete m_pObject;

	m_pInputLayout->Release();
	m_pVShader->Release();
	m_pPShader->Release();
	m_pConstantBuffer->Release();
}

int Model::LoadObjModel(char* fileName)
{
	m_pObject = new ObjFileModel(fileName, m_pD3DDevice, m_pImmediateContext);

	if (m_pObject->filename == "FILE NOT LOADED") return S_FALSE;

	HRESULT hr = S_OK;

	// Create Constant Buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; // Can use UpdateSubresource() to update
	constant_buffer_desc.ByteWidth = 112; // MUST be a multiple of 16, calculate from CB struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Use as a constant buffer

	hr = m_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer);

	ID3DBlob *VS, *PS, *error;
	hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelVS", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	if (error != 0) // Check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))
		{
			return hr;
		};
	}

	hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelPS", "ps_4_0", 0, 0, 0, &PS, &error, 0);

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

	D3DX11CreateShaderResourceViewFromFile(m_pD3DDevice, "Assets/tile.bmp", NULL, NULL, &m_pTexture0, NULL);
	//D3DX11CreateShaderResourceViewFromFile(m_pD3DDevice, "Assets/hogwarts.bmp", NULL, NULL, &m_pTexture1, NULL);

	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sampler_desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	m_pD3DDevice->CreateSamplerState(&sampler_desc, &m_pSampler0);

	CalculateModelCentrePoint();
	CalculateBoundingSphereRadius();

}

void Model::CalculateModelCentrePoint()
{

	float min_x = 0;
	float min_y = 0;
	float min_z = 0;
	float max_x = 0;
	float max_y = 0;
	float max_z = 0;

	for (int i = 0; i < m_pObject->numverts; i++)
	{
		if (m_pObject->vertices[i].Pos.x < min_x)
		{
			min_x = m_pObject->vertices[i].Pos.x;
		}
		else if (m_pObject->vertices[i].Pos.x > max_x)
		{
			max_x = m_pObject->vertices[i].Pos.x;
		}

		if (m_pObject->vertices[i].Pos.y < min_y)
		{
			min_y = m_pObject->vertices[i].Pos.y;
		}
		else if (m_pObject->vertices[i].Pos.y > max_y)
		{
			max_y = m_pObject->vertices[i].Pos.y;
		}

		if (m_pObject->vertices[i].Pos.z < min_z)
		{
			min_z = m_pObject->vertices[i].Pos.z;
		}
		else if (m_pObject->vertices[i].Pos.z > max_z)
		{
			max_z = m_pObject->vertices[i].Pos.z;
		}
	}

	m_bounding_sphere_centre_x = min_x + ((max_x - min_x) / 2);
	m_bounding_sphere_centre_y = min_y + ((max_y - min_y) / 2);
	m_bounding_sphere_centre_z = min_z + ((max_z - min_z) / 2);

}

void Model::CalculateBoundingSphereRadius()
{
	//float dist_x = 0;
	//float dist_y = 0;
	//float dist_z = 0;

	//for (int i = 0; i < m_pObject->numverts; i++)
	//{
	//	if (m_pObject->vertices[i].Pos.x - m_bounding_sphere_centre_x > dist_x)
	//	{
	//		dist_x = m_pObject->vertices[i].Pos.x - m_bounding_sphere_centre_x;
	//	}

	//	if (m_pObject->vertices[i].Pos.y - m_bounding_sphere_centre_y > dist_y)
	//	{
	//		dist_y = m_pObject->vertices[i].Pos.y - m_bounding_sphere_centre_y;
	//	}

	//	if (m_pObject->vertices[i].Pos.z - m_bounding_sphere_centre_z > dist_z)
	//	{
	//		dist_z = m_pObject->vertices[i].Pos.z - m_bounding_sphere_centre_z;
	//	}
	//}

	//m_bounding_sphere_radius = sqrt( (dist_x * dist_x) + (dist_y * dist_y) + (dist_z * dist_z) );

	float maxDistance = sqrt((pow(m_pObject->vertices[0].Pos.x - m_bounding_sphere_centre_x, 2)) +
		(pow(m_pObject->vertices[0].Pos.y - m_bounding_sphere_centre_y, 2)) +
		(pow(m_pObject->vertices[0].Pos.z - m_bounding_sphere_centre_z, 2)));

	float currentDistance;

	for (int i = 1; i < m_pObject->numverts; i++)
	{
		currentDistance = sqrt((pow(m_pObject->vertices[i].Pos.x - m_bounding_sphere_centre_x, 2)) +
			(pow(m_pObject->vertices[i].Pos.y - m_bounding_sphere_centre_y, 2)) +
			(pow(m_pObject->vertices[i].Pos.z - m_bounding_sphere_centre_z, 2)));

		if (currentDistance > maxDistance)
		{
			maxDistance = currentDistance;
		}
	}

	m_bounding_sphere_radius = maxDistance;
}

bool Model::CheckCollision(Model* obj)
{
	if (obj == this)
	{
		return false;
	}

	XMVECTOR cur_pos = GetBoundingSphereWorldSpacePosition();
	XMVECTOR other_pos = obj->GetBoundingSphereWorldSpacePosition();

	float x1 = XMVectorGetX(cur_pos);
	float y1 = XMVectorGetY(cur_pos);
	float z1 = XMVectorGetZ(cur_pos);

	float x2 = XMVectorGetX(other_pos);
	float y2 = XMVectorGetY(other_pos);
	float z2 = XMVectorGetZ(other_pos);

	float dist_x = x2 - x1;
	float dist_y = y2 - y1;
	float dist_z = z2 - z1;

	float sqr_dist = (dist_x * dist_x) + (dist_y * dist_y) + (dist_z * dist_z);

	float r1 = GetBoundingSphereRadius();
	float r2 = obj->GetBoundingSphereRadius();

	float sum_radius = r1 + r2;

	if (sqr_dist <= sum_radius * sum_radius)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void Model::UpdateRot(float pitch_degrees, float yaw_degrees, float roll_degrees)
{
	m_xangle += pitch_degrees;
	m_yangle += yaw_degrees;
	m_zangle += roll_degrees;
}

void Model::LookAt_XZ(float x, float z)
{
	dx = m_x - x;
	dz = m_z - z;

	m_yangle = -atan2(dx, -dz);	
}

void Model::MoveForward(float distance)
{
	m_x += sin(m_yangle) * distance;
	m_z += cos(m_yangle) * distance;
}

float Model::GetX()
{
	return m_x;
}

float Model::GetZ()
{
	return m_z;
}

XMVECTOR Model::GetBoundingSphereWorldSpacePosition()
{
	XMMATRIX world;

	world = XMMatrixRotationRollPitchYaw(m_xangle, m_yangle, m_zangle);
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	XMVECTOR offset;

	offset = XMVectorSet(m_bounding_sphere_centre_x, m_bounding_sphere_centre_y, m_bounding_sphere_centre_z, 0.0f);
	
	XMVECTOR new_pos = XMVector3Transform(offset, world);

	return new_pos;

}

float Model::GetBoundingSphereRadius()
{
	return m_bounding_sphere_radius;
}

void Model::Draw(XMMATRIX* view, XMMATRIX* projection)
{
	//UpdateRot(0.0, 0.0001, 0.0);

	XMMATRIX world;

	world = XMMatrixRotationRollPitchYaw(m_xangle, m_yangle, m_zangle);
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	m_directional_light_shines_from = XMVectorSet(-1.0f, 1.0f, -1.0f, 0.0f);
	m_ambient_light_colour = XMVectorSet(0.3f, 0.3f, 0.3f, 1.0f);

	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX transpose;
	MODEL_CONSTANT_BUFFER model_cb_values;
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

void Model::SetLightColour(float r, float g, float b)
{
	m_directional_light_colour = XMVectorSet(r, g, b, 1.0f);
}

