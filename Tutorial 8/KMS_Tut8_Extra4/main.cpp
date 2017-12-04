#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <string>
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <xnamath.h>
#include "text2D.h"
#include "Camera.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////
//	Global Variables
//////////////////////////////////////////////////////////////////////////////////////
HINSTANCE	g_hInst = NULL;
HWND		g_hWnd = NULL;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pD3DDevice = NULL;
ID3D11DeviceContext*    g_pImmediateContext = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pBackBufferRTView = NULL;
ID3D11Buffer*			g_pVertexBuffer;
ID3D11VertexShader*		g_pVertexShader;
ID3D11PixelShader*		g_pPixelShader;
ID3D11InputLayout*		g_pInputLayout;
ID3D11Buffer*			g_pConstantBuffer0;
ID3D11DepthStencilView* g_pZBuffer;
ID3D11ShaderResourceView* g_pTexture0;
ID3D11ShaderResourceView* g_pTexture1;
ID3D11SamplerState* g_pSampler0;
ID3D11BlendState* g_pAlphaBlendEnable; // 14
ID3D11BlendState* g_pAlphaBlendDisable; // 14
Text2D* g_2DText0;
Text2D* g_2DText1;
Camera* eye;

FLOAT z_position2 = 5.0f;

FLOAT x_rotation1 = 0.0f;
FLOAT y_rotation1 = 0.0f;
FLOAT z_rotation2 = 0.0f;
FLOAT y_rotation2 = 0.0f;

bool isIncreasing = true;

float yaw_degrees = 0.0f;
float pitch_degrees = 0.0f;
float g_pDistance = 0.0f;

float mouse_x = 0.0f;
float prev_mouse_x = 0.0f;
float mouse_y = 0.0f;
float prev_mouse_y = 0.0f;

// Const buffer structs. Pack to 16 bytes. 
// Don't let any single element cross a 18 byte boundary

struct CONSTANT_BUFFER0
{
	XMMATRIX WorldViewProjection; // 64 bytes (4 x 4 = 16 floats x 4 bytes)
	float RedAmount; // 4 bytes
	float ScaleAmount; // 4 bytes
	XMFLOAT2 packing_bytes; // 2x4 bytes = 8 bytes
}; // TOTAL BYTES = 80 Bytes


// Define Vertex structure

struct POS_COL_TEX_VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT4 Col;
	XMFLOAT2 Texture0;
};

// Rename for each tutorial
char		g_Tutorial_06_Exercise_02[100] = "Tutorial 06 Exercise 02\0";


//////////////////////////////////////////////////////////////////////////////////////
//	Forward declarations
//////////////////////////////////////////////////////////////////////////////////////
HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitialiseD3D();
void ShutdownD3D();
void RenderFrame(void);
HRESULT InitialiseGraphics(void);


//////////////////////////////////////////////////////////////////////////////////////
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//////////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Main message loop
	MSG msg = { 0 };

	if (FAILED(InitialiseWindow(hInstance, nCmdShow)))
	{
		DXTRACE_MSG("Failed to create Window");
		return 0;
	}

	if (FAILED(InitialiseD3D()))
	{
		DXTRACE_MSG("Failed to create Device");
		return 0;
	}

	if (FAILED(InitialiseGraphics())) // 03-01
	{
		DXTRACE_MSG("Failed to initialise graphics");
		return 0;
	}

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_pImmediateContext->OMSetBlendState(g_pAlphaBlendEnable, 0, 0xffffffff);
			RenderFrame();
		}
	}

	ShutdownD3D();

	return (int)msg.wParam;
}


//////////////////////////////////////////////////////////////////////////////////////
// Register class and create window
//////////////////////////////////////////////////////////////////////////////////////
HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Give your app window your own name
	char Manikandan[100] = "Hello World\0";

	// Register class
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	//   wcex.hbrBackground = (HBRUSH )( COLOR_WINDOW + 1); // Needed for non-D3D apps
	wcex.lpszClassName = Manikandan;

	if (!RegisterClassEx(&wcex)) return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 1280, 768};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(Manikandan, g_Tutorial_06_Exercise_02, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
		rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if (!g_hWnd)
		return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
// Called every time the application receives a message
//////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(g_hWnd);
		
		if (wParam == VK_UP)
		{
			g_pDistance += 1.0f;
			eye->Forward(g_pDistance);
		}
		if (wParam == VK_DOWN)
		{
			g_pDistance -= 1.0f;
			eye->Forward(g_pDistance);
		}
	/*case WM_MOUSEMOVE:
		{
		mouse_x = LOWORD(lParam);

		if (mouse_x < prev_mouse_x)
		{
			yaw_degrees -= 1.0f;
			eye->YawRotate(yaw_degrees);

			prev_mouse_x = mouse_x;
		}
		else if (mouse_x > prev_mouse_x)
		{
			yaw_degrees += 1.0f;
			eye->YawRotate(yaw_degrees);

			prev_mouse_x = mouse_x;
		}

		mouse_y = HIWORD(lParam);

		if (mouse_y > prev_mouse_y)
		{
			pitch_degrees -= 1.0f;
			eye->PitchRotate(pitch_degrees);

			prev_mouse_y = mouse_y;
		}
		else if (mouse_y < prev_mouse_y)
		{
			pitch_degrees += 1.0f;
			eye->PitchRotate(pitch_degrees);

			prev_mouse_y = mouse_y;
		}
		
		}*/
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// Create D3D device and swap chain
//////////////////////////////////////////////////////////////////////////////////////
HRESULT InitialiseD3D()
{

	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, // comment out this line if you need to test D3D 11.0 functionality on hardware that doesn't support it
		D3D_DRIVER_TYPE_WARP, // comment this out also to use reference device
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL,
			createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain,
			&g_pD3DDevice, &g_featureLevel, &g_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return hr;

	// Get pointer to back buffer texture
	ID3D11Texture2D *pBackBufferTexture;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBufferTexture);

	if (FAILED(hr)) 
		return hr;

	// Use the back buffer texture pointer to create the render target view
	hr = g_pD3DDevice->CreateRenderTargetView(pBackBufferTexture, NULL,
		&g_pBackBufferRTView);
	pBackBufferTexture->Release();

	if (FAILED(hr)) 
		return hr;
	
	// Create a Z buffer texture
	D3D11_TEXTURE2D_DESC tex2dDesc;
	ZeroMemory(&tex2dDesc, sizeof(tex2dDesc));

	tex2dDesc.Width = width;
	tex2dDesc.Height = height;
	tex2dDesc.ArraySize = 1;
	tex2dDesc.MipLevels = 1;
	tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex2dDesc.SampleDesc.Count = sd.SampleDesc.Count;
	tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2dDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Texture2D *pZBufferTexture;
	hr = g_pD3DDevice->CreateTexture2D(&tex2dDesc, NULL, &pZBufferTexture);

	if (FAILED(hr)) return hr;
	

	// Create the Z buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));

	dsvDesc.Format = tex2dDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	g_pD3DDevice->CreateDepthStencilView(pZBufferTexture, &dsvDesc, &g_pZBuffer);
	pZBufferTexture->Release();

	// Set the render target view
	g_pImmediateContext->OMSetRenderTargets(1, &g_pBackBufferRTView, g_pZBuffer);

	// Set the viewport
	D3D11_VIEWPORT viewport;

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	g_pImmediateContext->RSSetViewports(1, &viewport);



	D3D11_BLEND_DESC b;
	b.RenderTarget[0].BlendEnable = TRUE;
	b.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	b.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	b.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	b.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	b.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	b.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	b.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	b.IndependentBlendEnable = FALSE;
	b.AlphaToCoverageEnable = FALSE;

	g_pD3DDevice->CreateBlendState(&b, &g_pAlphaBlendEnable);

	g_2DText0 = new Text2D("Assets/font1.bmp", g_pD3DDevice, g_pImmediateContext);
	g_2DText1 = new Text2D("Assets/font3.png", g_pD3DDevice, g_pImmediateContext);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
// Clean up D3D objects
//////////////////////////////////////////////////////////////////////////////////////
void ShutdownD3D()
{
	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pInputLayout) g_pInputLayout->Release();
	if (g_pVertexShader) g_pVertexShader->Release();
	if (g_pPixelShader) g_pPixelShader->Release();
	if (g_pBackBufferRTView) g_pBackBufferRTView->Release();
	if (g_pZBuffer) g_pZBuffer->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pConstantBuffer0) g_pConstantBuffer0->Release();
	if (g_pTexture0) g_pTexture0->Release();
	if (g_pTexture1) g_pTexture1->Release();
	if (g_pSampler0) g_pSampler0->Release();
	if (g_2DText0) delete g_2DText0;
	if (g_2DText1) delete g_2DText1;
	if (g_pAlphaBlendEnable) g_pAlphaBlendEnable->Release();
	if (g_pAlphaBlendDisable) g_pAlphaBlendDisable->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pD3DDevice) g_pD3DDevice->Release();
}

/////////////////////////////////////////////////////////////////////////////////////
// Init Graphics
/////////////////////////////////////////////////////////////////////////////////////
HRESULT InitialiseGraphics()// 03-01
{
	HRESULT hr = S_OK;

	// Define vertices of a triangle - screen coordinates -1.0 to +1.0
	POS_COL_TEX_VERTEX vertices[] =
	{
		// back face
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),    XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),     XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f),     XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),     XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f),      XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f),     XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },

		// front face
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),    XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),     XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f) ,     XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),    XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f),      XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),     XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		// left face
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),    XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),     XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),     XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),     XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f),      XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),     XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },

		// right face
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),    XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),    XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),    XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),    XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),    XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),    XMFLOAT2(0.0f, 0.0f) },

		// bottom face
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),    XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),    XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),    XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),    XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),    XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),    XMFLOAT2(0.0f, 0.0f) },

		// top face
		{ XMFLOAT3(1.0f, 1.0f, 1.0f),   XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),     XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),     XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),     XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),     XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f),   XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),     XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),     XMFLOAT2(0.0f, 1.0f) },

	};

	// Set up and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // Used by CPU and GPU
	bufferDesc.ByteWidth = sizeof(vertices); // Total size of buffer, number of vertices
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Use as a vertex buffer
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allow CPU Access
	hr = g_pD3DDevice->CreateBuffer(&bufferDesc, NULL, &g_pVertexBuffer); // Create the buffer

	// Create Constant Buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; // Can use UpdateSubresource() to update
	constant_buffer_desc.ByteWidth = 80; // MUST be a multiple of 16, calculate from CB struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Use as a constant buffer

	hr = g_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &g_pConstantBuffer0);

	if (FAILED(hr))
	{
		return hr;
	}

	// Copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	// Lock the buffer to allow writing
	g_pImmediateContext->Map(g_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	// Copy the data
	memcpy(ms.pData, vertices, sizeof(vertices));

	// Unlock the buffer
	g_pImmediateContext->Unmap(g_pVertexBuffer, NULL);

	// Load and compile pixel and vertex shaders - use vs_5_0 to target DX11 hardware only
	ID3DBlob *VS, *PS, *error;
	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr)) // don't fail if error is just a warning
		{
			return hr;
		};
	}

	hr = D3DX11CompileFromFile("shaders.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	if (error != 0) // check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr)) // don't fail if error is just a warning
		{
			return hr;
		};
	}

	// Create shader objects
	hr = g_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &g_pVertexShader);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = g_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &g_pPixelShader);

	if (FAILED(hr))
	{
		return hr;
	}

	// Set the shader objects as active
	g_pImmediateContext->VSSetShader(g_pVertexShader, 0, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader, 0, 0);

	// Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	hr = g_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &g_pInputLayout);

	if (FAILED(hr))
	{
		return hr;
	}

	g_pImmediateContext->IASetInputLayout(g_pInputLayout);

	eye = new Camera(0.0, 0.0, -0.5, 0);

	D3DX11CreateShaderResourceViewFromFile(g_pD3DDevice, "Assets/tile.bmp", NULL, NULL, &g_pTexture0, NULL);
	D3DX11CreateShaderResourceViewFromFile(g_pD3DDevice, "Assets/hogwarts.bmp", NULL, NULL, &g_pTexture1, NULL);

	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	g_pD3DDevice->CreateSamplerState(&sampler_desc, &g_pSampler0);

	return S_OK;

}

// Render frame
void RenderFrame(void)
{
	// Clear the back buffer - choose a colour you like
	float rgba_clear_colour[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	g_pImmediateContext->ClearRenderTargetView(g_pBackBufferRTView, rgba_clear_colour);
	g_pImmediateContext->ClearDepthStencilView(g_pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// RENDERING

	// Set Vertex buffer // 03-01
	UINT stride = sizeof(POS_COL_TEX_VERTEX);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	CONSTANT_BUFFER0 cb0_values;
	//cb0_values.RedAmount = 0.47f; // 47% of vertex red value
	//cb0_values.ScaleAmount = 0.47f; // reduced to 47%

	// Select which primitive type to use // 03-01
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX projection, world1, world2, view;

	x_rotation1 += 0.0002f;
	y_rotation1 += 0.0001f;
	z_rotation2 += 0.0005f;
	y_rotation2 += 0.0006f;

	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0), 1280.0 / 768.0, 1.0, 100.0);
	view = eye->GetViewMatrix();

	//********************************//
	//***********FIRST CUBE***********//
	//********************************//

	//world1 = XMMatrixRotationRollPitchYaw(x_rotation1, y_rotation1, 0.0f);
	//world1 *= XMMatrixTranslation(3,0,15);

	//cb0_values.WorldViewProjection = world1 * view * projection;

	//// upload the new values for the constant buffer
	//g_pImmediateContext->UpdateSubresource(g_pConstantBuffer0, 0, 0, &cb0_values, 0, 0);

	//g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer0);

	////Draw the vertex buffer to the back buffer //  03-01
	//g_pImmediateContext->Draw(36, 0);
	
	//*********************************//
	//***********SECOND CUBE***********//
	//*********************************//

	world2 = XMMatrixRotationRollPitchYaw(0.0f, y_rotation2, z_rotation2);

	//if (z_position2 <= 100.0 && isIncreasing == true)
	//{
	//	z_position2 += 0.01f;
	//}
	//else
	//{
	//	isIncreasing = false;
	//}
	//
	//if (z_position2 >= 5.0f && isIncreasing == false)
	//{
	//	z_position2 -= 0.01f;
	//}
	//else
	//{
	//	isIncreasing = true;
	//}

	world2 *= XMMatrixTranslation(0, 0, z_position2);

	cb0_values.WorldViewProjection = world2 * view * projection;
	g_2DText0->AddText("Health:" + to_string(90) + '/' + to_string(100), -0.98, +0.95, 0.04);
	g_2DText1->AddText("GamerLord707", -0.98, +0.7, 0.04);
	// upload the new values for the constant buffer
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer0, 0, 0, &cb0_values, 0, 0);

	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer0);

	g_pImmediateContext->PSSetSamplers(0, 1, &g_pSampler0);
	g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTexture0);
	g_pImmediateContext->PSSetShaderResources(1, 1, &g_pTexture1);

	g_pImmediateContext->VSSetShader(g_pVertexShader, 0, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader, 0, 0);

	g_pImmediateContext->IASetInputLayout(g_pInputLayout);
	

	g_pImmediateContext->Draw(36, 0);

	g_2DText0->RenderText();
	g_2DText1->RenderText();

	g_pImmediateContext->OMSetBlendState(g_pAlphaBlendDisable, 0, 0xffffffff);
	
	// Display what has just been rendered
	g_pSwapChain->Present(0, 0);
}


