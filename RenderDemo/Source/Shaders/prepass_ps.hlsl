
struct ps_input
{
	float4 posCS : SV_POSITION;
	float2 texCoords : TEXCOORDS;
	float4 posVS : POSITION_VS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct ps_output
{
	float4 normalDepth;
	float4 albedoGloss;
};

Texture2D diffuseMap : register(t0);
texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);
//Texture2D normalMap : register(t0);
//Texture2D diffuseMap : register(t1);

SamplerState normalSampler : register(s0);

ps_output prepass_ps(ps_input input) : SV_Target
{
	ps_output output;

	float3x3 tangentMatrix;
	tangentMatrix[0] = normalize(input.tangent);
	tangentMatrix[1] = normalize(input.binormal);
	tangentMatrix[2] = normalize(input.normal);

	//float3 normal = normalMap.Sample(normalSampler, input.texCoords);
	//	output.normal = float4(normal, 1.0f);

	float3 bump = normalMap.Sample(normalSampler, input.texCoords).xyz;
		bump = normalize(bump);
	//bump = mul(bump, tangentMatrix);
	//bump = normalize(bump);
	//output.normal = float4(bump, 1.0f);
	//float3 geometricNormal = normalize(input.normal);
	//	output.normal = float4(geometricNormal, 1.0f);
	//output.normal = float4(0.0f, 0.0f, 1.0f, 1.0f);
	//output.normal = float4(normalize(input.tangent), 1.0f);

	float depth = length(input.posVS.xyz);
	//float depth = input.posVS.xyz;
	//output.depth = float4(depth, depth, depth, 1.0f);
	//return float4(depth, depth, depth, 1.0f);
	output.normalDepth = float4(bump, depth);

	float3 diffuse = diffuseMap.Sample(normalSampler, input.texCoords).xyz;
	float gloss = specularMap.Sample(normalSampler, input.texCoords).x;
	output.albedoGloss = float4(diffuse, gloss);

	return output;
}