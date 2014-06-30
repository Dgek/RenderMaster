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

cbuffer LightParams : register ( cb0 )
{
	float4 lightColor;
	float4 lightPosition;
	float3 lightDir;
	float  lightRange;
	float4 spotAngles;
};

ps_output point_light_gridPS(gs_output input) : SV_TARGET
{
	ps_output output;

	int3 voxelPos = int3(input.position.xy, input.rtIndex);
	uint gridIndex = voxelPos.z * 4096 + voxelPos.y * 64 + voxelPos.x;

	//get voxel data and discard if there is no occlusion data
	VOXEL voxel = voxelGrid[gridIndex];
	if (voxel.occlusion == 0)
		discard;

	int3 offset = voxelPos - int3(32,32,32);
	float3 position = offset * 60.5f + int3(0.0f, 500.0f, 0.0f);


	float3 lightVec = lightPosition.xyz - position;
	float lightLength = length(lightVec);
	if (lightLength > lightRange)
		discard;

	float3 lightVecN = lightVec / lightLength;

	//get color
	float3 albedo = DecodeColor(voxel.color);

	//get closest normal
	float nDotL;
	float3 normal = GetClosestNormal(voxel.normalMasks, lightVecN, nDotL);

	//calculate diffuse lighting
	float atten = saturate(1.0f - (1.0f / lightRange) * lightLength);
	float3 diffuse = lightColor * albedo * max(nDotL, 0.0f) * atten;

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
