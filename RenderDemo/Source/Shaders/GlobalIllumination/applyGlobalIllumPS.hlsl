#include "GlobalIllumination.hlsli"

Texture2DMS<float4> depthNormalMap : register(t0);
Texture2DMS<float4> albedoGlossMap : register(t1);

Texture2DArray inputRedSHTexture : register(t2);
Texture2DArray inputGreenSHTexture : register(t3);
Texture2DArray inputBlueSHTexture : register(t4);

SamplerState linearSampler : register(s0);

cbuffer CameraBuffer : register(b0)
{
	float3 cameraPos : packoffset(c0.x);
	float pad : packoffset(c0.w);
};

struct gs_output
{
	float4 pos : SV_POSITION;
	float2 texCoords : TEXCOORDS;
	float3 frustumRay : FRUSTUM_RAY;
};

float4 DecodePosition(in float3 frustumRay, in float depth)
{
	float4 position;

	float3 frustumRayN = normalize(frustumRay);
	position.xyz = cameraPos + frustumRayN*depth;
	position.w = 1.0f;

	return position;
}

float4 globalIllumPS(gs_output input, uint sampleIndex : SV_SampleIndex) : SV_TARGET
{
	float3 gridCenter = float3(0.0f, 500.0f, 0.0f);
	float  invCellSize = 1.0f / 60.5f;

	float depth = depthNormalMap.Load(int2(input.pos.x, input.pos.y), sampleIndex).w;

	float3 frustumRay = normalize(input.frustumRay);

	float4 position = DecodePosition(input.frustumRay, depth);

	
	float3 albedo = albedoGlossMap.Load(int2(input.pos.x, input.pos.y), sampleIndex).xyz;

	float3 normal = depthNormalMap.Load(int2(input.pos.x, input.pos.y), sampleIndex).xyz;

	float3 offset = (position.xyz - gridCenter) * invCellSize;

	//calculate texture coordinates
	float3 texCoords = float3(32.5f, 32.5f, 32.0) + offset;
	texCoords.xy /= 64.0f;

	//calculate texture coordinates for trilinear filtering
	int lowZ = floor(texCoords.z);
	int highZ = min(lowZ + 1, 64 - 1);
	float highZWeight = texCoords.z - lowZ;
	float lowZWeight = 1.0f - highZWeight;
	float3 tcLow = float3(texCoords.xy, lowZ);
	float3 tcHigh = float3(texCoords.xy, highZ);

	//get SH-coefficients with trilinear filtering
	float4 redSHCoeffs =	lowZWeight * inputRedSHTexture.Sample(linearSampler, tcLow) +
							highZWeight * inputRedSHTexture.Sample(linearSampler, tcHigh);

	float4 greenSHCoeffs =	lowZWeight * inputGreenSHTexture.Sample(linearSampler, tcLow) +
							highZWeight * inputGreenSHTexture.Sample(linearSampler, tcHigh);

	float4 blueSHCoeffs =	lowZWeight * inputBlueSHTexture.Sample(linearSampler, tcLow) +
							highZWeight * inputBlueSHTexture.Sample(linearSampler, tcHigh);

	//calculate SH for surface normal
	float4 surfaceNormalLobe = ClampedCosineCoeffs(normal);

	//calculate SH-lighting
	float3 diffuseGlobalIllum;
	diffuseGlobalIllum.r = dot(redSHCoeffs, surfaceNormalLobe);
	diffuseGlobalIllum.g = dot(greenSHCoeffs, surfaceNormalLobe);
	diffuseGlobalIllum.b = dot(blueSHCoeffs, surfaceNormalLobe);
	diffuseGlobalIllum /= PI;

	return float4(diffuseGlobalIllum*albedo, 1.0f);
}