cbuffer Matrix : register(b0)
{
	matrix World;
	matrix WorldView;
	matrix WorldViewProj;
};

struct vs_input
{
	float3 pos : POSITION;
	float2 texCoords : TEXCOORDS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

struct ps_input
{
	float4 posCS : SV_POSITION;
	float2 texCoords : TEXCOORDS;
	float4 posVS : POSITION_VS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

ps_input prepass_vs(vs_input input)
{
	ps_input output;

	output.posCS = mul(float4(input.pos, 1.0f), WorldViewProj);
	output.posVS = mul(float4(input.pos, 1.0f), WorldView);
	output.texCoords = input.texCoords;
	output.normal = mul(input.normal, (float3x3)World);
	output.tangent = mul(input.tangent, (float3x3)World);
	output.binormal = cross(input.normal, input.tangent);

	return output;
}