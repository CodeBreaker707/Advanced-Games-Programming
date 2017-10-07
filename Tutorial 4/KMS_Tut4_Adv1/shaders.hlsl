cbuffer Cbuffer0 : register(b0)
{
	float color_fraction; // 4 bytes
	float3 packing_01; // 3x4 bytes = 12 bytes
};

cbuffer Cbuffer1 : register(b1)
{
	float scale; // 4 bytes
	float3 packing_02; // 3x4 bytes = 12 bytes
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
	VOut output;

	color.rb *= color_fraction;
	position.xy *= scale;
	output.position = position;
	output.color = color;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color;
}