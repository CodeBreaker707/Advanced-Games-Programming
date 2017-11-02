Texture2D texture0;
SamplerState sampler0;

cbuffer Cbuffer0
{
	matrix WVPMatrix; // 64 bytes
	float4 point_light_position; // 16 bytes
	float4 point_light_colour; // 16 bytes
	float4 ambient_light_colour;
}; // TOTAL SIZE = 96 bytes;

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 normal: NORMAL)
{
	VOut output;

	//color.rb *= red_fraction;
	//position.xy *= scale;
	
	output.position = mul(WVPMatrix, position);
	
	float lightVector = point_light_position - position;
	float point_amount = dot(normalize(lightVector), normal);

	point_amount = saturate(point_amount);

	output.color = ambient_light_colour + (point_light_colour * point_amount);
	
	output.texcoord = texcoord;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	return color * texture0.Sample(sampler0, texcoord);
}