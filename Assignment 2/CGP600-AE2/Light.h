#pragma once

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3dx11.h>
#include <dxerr.h>
#include <xnamath.h>

class Light
{
private:

	XMVECTOR m_directional_light_shines_from;
	XMVECTOR m_directional_light_colour;
	XMVECTOR m_ambient_light_colour;

public:

	Light();
	~Light();

	void SetDirectionalLightPos(float x, float y, float z, float w);
	void SetDirectionalLightColour(float r, float g, float b, float a);
	void SetAmbientLightColour(float r, float g, float b, float a);

	XMVECTOR GetDirectionalLightPos();
	XMVECTOR GetDirectionalLightColour();
	XMVECTOR GetAmbientLightColour();

};
