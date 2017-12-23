#pragma once

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <xnamath.h>
using namespace std;

struct Particle
{
	float gravity;
	XMFLOAT3 position;
	XMFLOAT3 velocity;
	XMFLOAT4 color;

};

class ParticleEngine
{
	private:

		// DirectX 11 components for rendering objects into the scene
		ID3D11Device* m_pD3DDevice;
		ID3D11DeviceContext* m_pImmediateContext;

		// DirectX 11 Shaders, Buffers, Texture and Sampler Objects
		ID3D11VertexShader* m_pVShader;
		ID3D11PixelShader*  m_pPShader;
		ID3D11InputLayout* m_pInputLayout;
		ID3D11Buffer* m_pVertexBuffer;
		ID3D11ShaderResourceView* m_pTexture0;
		ID3D11SamplerState* m_pSampler0;

	public:

		// Particle Engine class constructor and destructor
		ParticleEngine(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext);
		~ParticleEngine();

		int InitialiseParticle();

		// Function to draw the asset into the scene
		void Draw(XMMATRIX* view, XMMATRIX* projection, XMFLOAT3* position);
		void DrawOne(Particle* one, XMMATRIX* view, XMMATRIX* projection, XMFLOAT3* position);


};
