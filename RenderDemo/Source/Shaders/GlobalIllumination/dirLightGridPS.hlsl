#include "GlobalIllumination.hlsli"

struct gs_output
{
	float4 position : SV_POSITION;
	uint rtIndex : SV_RenderTargetArrayIndex;
};

struct ps_output
{
	float4 red : SV_TARGET0; //red SH-coeff
	float4 green : SV_TARGET1; //green SH-coeff
	float4 blue : SV_TARGET2; //blue SH-coeff
};

StructuredBuffer<VOXEL> voxelGrid : register( t0 );
Texture2D<float> shadowMap : register(t1);

cbuffer LightParams : register ( b0 )
{
	//float4 lightColor;
	//float4 lightPosition;
	//float3 lightDir;
	//float  lightRange;
	float4 spotAngles;
};

cbuffer ShadowParams : register(b1)
{
	matrix shadowViewProj;
};

#define PCF_NUM_SAMPLES 16
#define SHADOW_FILTER_RADIUS 2.0f
#define SHADOW_BIAS 0.02f

float ComputeShadow(in float4 positionWS, in float distance)
{
	float4 result = mul(shadowViewProj, positionWS);
	result.xyz /= result.w;
	float shadow = 1.0f;

	float shadowMapDistance = shadowMap.Load(int3(result.xy, 0));
	if (shadowMapDistance < distance)
		shadow = 0.0f;

	//shadow /= PCF_NUM_SAMPLES;
	return shadow;
}

ps_output dir_light_gridPS(gs_output input) : SV_TARGET
{
	float4 lightColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//float4 lightPosition = float4(0, 700, -500, 1.0f);
	float4 lightPosition = float4(0, 1500, 0, 1.0f);
	float3 lightDir = float3(0.2403, -0.926800013, 0.288599998);
	float lightRange = 500;
	//float4 lightPosition;
	//float3 lightDir;
	//float  lightRange;

	ps_output output;

	int3 voxelPos = int3(input.position.xy, input.rtIndex);
	uint gridIndex = voxelPos.z * 4096 + voxelPos.y * 64 + voxelPos.x;

	//get voxel data and discard if there is no occlusion data
	VOXEL voxel = voxelGrid[gridIndex];
	if (voxel.occlusion == 0)
		discard;

	int3 offset = voxelPos - int3(32,32,32);
	float3 position = offset * 60.5f + int3(0.0f, 500.0f, 0.0f);

	float3 lightVec = -lightDir;
	float lightLength = length(lightVec);

	float3 lightVecN = lightVec / lightLength;

	//get color
	float3 albedo = DecodeColor(voxel.color);

	//get closest normal
	float nDotL;
	float3 normal = GetClosestNormal(voxel.normalMasks, lightVecN, nDotL);

	//compute shadowing by using shadow map from direct illumination
	float3 vecToLight = position - lightPosition;
	float distanceToLight = length(vecToLight);
	//float shadow = ComputeShadow(float4(position, 1.0f), distanceToLight);

	//calculate diffuse lighting
	float atten = saturate(1.0f - (1.0f / lightRange) * lightLength);
	float3 diffuse = lightColor * albedo * max(nDotL, 0.0f) * atten;// *shadow;

	//store VPL as SH coefficients of second order
	float4 coeffs = ClampedCosineCoeffs(normal);

	float4 redSHCoeffs = coeffs*diffuse.r;
	float4 greenSHCoeffs = coeffs*diffuse.g;
	float4 blueSHCoeffs = coeffs*diffuse.b;

	output.red = redSHCoeffs;
	output.green = greenSHCoeffs;
	output.blue = blueSHCoeffs;

	return output;
}
