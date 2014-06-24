#include "GlobalIllumination.hlsli"

Texture2DArray inputRedSHTexture : register (t0);
Texture2DArray inputGreenSHTexture : register (t1);
Texture2DArray inputBlueSHTexture : register (t2);

StructuredBuffer<VOXEL> voxelGrid : register (t3);

RWTexture2DArray<float4> outputRedSHTexture : register (u0);
RWTexture2DArray<float4> outputGreenSHTexture : register (u1);
RWTexture2DArray<float4> outputBlueSHTexture : register (u2);

#define SOLID_ANGLE_A 0.0318842778f // (22.95668f/(4*180.0f)) 
#define SOLID_ANGLE_B 0.0336955972f // (24.26083f/(4*180.0f))

#define FLUX_AMPLIFIER 1.75f

//directions to six neighbor voxel cell centers
static float3 directions[6] =
{
	float3(0.0f, 0.0f, 1.0f), float3(1.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, -1.0f),
	float3(-1.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f), float3(0.0f, -1.0f, 0.0f)
};

//SH-coefficients for size faces (ClampedCosineSHCoeffs[directions0-5])
static float4 faceCoeffs[6] =
{
	float4(0.8862269521f, 0.0f, 1.0233267546f, 0.0f),  // ClampedCosineCoeffs(directions[0])
	float4(0.8862269521f, 0.0f, 0.0f, -1.0233267546f), // ClampedCosineCoeffs(directions[1])
	float4(0.8862269521f, 0.0f, -1.0233267546f, 0.0f), // ClampedCosineCoeffs(directions[2])
	float4(0.8862269521f, 0.0f, 0.0f, 1.0233267546f),  // ClampedCosineCoeffs(directions[3])
	float4(0.8862269521f, -1.0233267546f, 0.0f, 0.0f), // ClampedCosineCoeffs(directions[4])
	float4(0.8862269521f, 1.0233267546, 0.0f, 0.0f)	   // ClampedCosineCoeffs(directions[5])
};


//offsets to six neighbor voxel cell centers
static int3 offsets[6] =
{
	int3(0, 0, 1), int3(1, 0, 0), int3(0, 0, -1),
	int3(-1, 0, 0), int3(0, 1, 0), int3(0, -1, 0)
};

[numthreads(8, 8, 8)]
void propagate_cs(uint3 groupID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID,
	uint3 groupThreadID : SV_GroupThreadID, uint groupIndex : SV_GroupIndex)
{
	int3 elementPos = dispatchThreadID.xyz;

	//init SH-coeffs from the values of the current cell
	float4 sumRedSHCoeffs = inputRedSHTexture.Load(int4(elementPos, 0));
	float4 sumGreenSHCoeffs = inputGreenSHTexture.Load(int4(elementPos, 0));
	float4 sumBlueSHCoeffs = inputBlueSHTexture.Load(int4(elementPos, 0));

	[unroll]
	for (int i = 0; i < 6; i++)
	{

		int3 samplePos = elementPos + offsets[i];

			//cell is out of bounds
		if ((samplePos.x < 0) && (samplePos.x > 64) &&
			(samplePos.y < 0) && (samplePos.y > 64) &&
			(samplePos.z < 0) && (samplePos.z > 64))
			continue;

		//load SH-coeffs for neighbor cell
		float4 redSHCoeffs = inputRedSHTexture.Load(int4(samplePos, 0));
		float4 greenSHCoeffs = inputGreenSHTexture.Load(int4(samplePos, 0));
		float4 blueSHCoeffs = inputBlueSHTexture.Load(int4(samplePos, 0));

		float4 occlusionCoeffs = float4(0.0f, 0.0f, 0.0f, 0.0f);

		//get index of the voxel
		int gridIndex = samplePos.z * 4096 + samplePos.y * 64 + samplePos.x;
		VOXEL voxel = voxelGrid[gridIndex];

		if (voxel.occlusion > 0)
		{
			float dotProduct;
			float3 occlusionNormal = GetClosestNormal(voxel.normalMasks, -directions[i], dotProduct);
			occlusionCoeffs = ClampedCosineCoeffs(occlusionNormal);
		}

		[unroll]
		for (int j = 0; j < 6; j++)
		{
			//direction from neighbor center to current face center
			float3 neighborCellCenter = directions[i];
			float3 facePosition = directions[j] * 0.5f;
			float3 dir = facePosition - neighborCellCenter;
			float fLength = length(dir);
			dir /= fLength;

			//get corresponding solid angle 
			//TODO: calculate solid angles
			float solidAngle = 0.0f;
			if (fLength > 0.5f)
				solidAngle = (fLength >= 1.5f) ? SOLID_ANGLE_A : SOLID_ANGLE_B;

			//calculate SH-coeffs for direction
			float4 dirSH = SH(dir);

			//calculate flux from neighbor cell to face of the current cell
			float3 flux;
			flux.r = dot(redSHCoeffs, dirSH);
			flux.g = dot(greenSHCoeffs, dirSH);
			flux.b = dot(blueSHCoeffs, dirSH);
			flux = max(0.0f, flux) * solidAngle * FLUX_AMPLIFIER;

			//apply occlusion
			float occlusion = 1.0f - saturate(dot(occlusionCoeffs, dirSH));
			flux *= occlusion;

			//add to SH-sums
			float4 coeffs = faceCoeffs[j];
			sumRedSHCoeffs += coeffs * flux.r;
			sumGreenSHCoeffs += coeffs * flux.g;
			sumBlueSHCoeffs += coeffs * flux.b;
		}
	}

	outputRedSHTexture[elementPos] = sumRedSHCoeffs;
	outputGreenSHTexture[elementPos] = sumGreenSHCoeffs;
	outputBlueSHTexture[elementPos] = sumBlueSHCoeffs;
}