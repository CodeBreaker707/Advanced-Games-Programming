TextureCube cube0;
SamplerState sampler0;

cbuffer CB0
{
	matrix WVPMatrix; // 64 bytes

}; // 112 bytes

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float3 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};

VOut VS_Sky(float4 position : POSITION, float3 texcoord : TEXCOORD)
{
	VOut output;

	output.texcoord = position.xyz;
	output.position = mul(WVPMatrix, position);

	return output;
}

//float4 ModelPS(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
float4 PS_Sky(VOut input) : SV_TARGET
{
	return cube0.Sample(sampler0, input.texcoord);
}