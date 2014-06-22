struct vs_input
{
	float3	pos			:	POSITION;
	float2	texCoords	:	TEXCOORDS;
	float3	normal		:	NORMAL;
	float3	tangent		:	TANGENT;
};
struct ps_input
{
	float4 pos : SV_POSITION;
	float4 posWS : POSITION_WS;
	float2 texCoords : TEXCOORDS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

cbuffer MatrixBuffer : register(cb0)
{
	matrix World;
	matrix WorldView;
	matrix WVP;
};

ps_input shading_vs(vs_input input)
{
	ps_input output;

	output.posWS = mul(float4(input.pos, 1.0f), World);
	output.pos = mul(float4(input.pos, 1.0f), WVP);

	output.texCoords = input.texCoords;

	output.normal = mul(input.normal, (float3x3)World);
	output.tangent = mul(input.tangent, (float3x3)World);
	output.binormal = cross(input.normal, input.tangent);

	return output;
}