#include "GlobalIllumination.hlsli"

RWStructuredBuffer<VOXEL> voxelGrid : register( u0 );

RWTexture2DArray<float4> voxelRedSHTexture : register (u1);
RWTexture2DArray<float4> voxelGreenSHTexture : register (u2);
RWTexture2DArray<float4> voxelBlueSHTexture : register (u3);

RWTexture2DArray<float4> propagateRedSHTexture : register (u4);
RWTexture2DArray<float4> propagateGreenSHTexture : register (u5);
RWTexture2DArray<float4> propagateBlueSHTexture : register (u6);

[numthreads(8, 8, 8)]
void voxel_grid_clearCS( uint3 threadId : SV_DispatchThreadID )
{
	VOXEL clean;

	clean.color = 0;
	clean.normalMasks = uint4(0,0,0,0);
	clean.occlusion = 0;

	int gridIndex = threadId.z * 4096 + threadId.y * 64 + threadId.x;
	voxelGrid[gridIndex] = clean;

	int3 elementPos = threadId.xyz;

	voxelRedSHTexture[elementPos] = float4(0.0f, 0.0f, 0.0f, 0.0f);
	voxelGreenSHTexture[elementPos] = float4(0.0f, 0.0f, 0.0f, 0.0f);
	voxelBlueSHTexture[elementPos] = float4(0.0f, 0.0f, 0.0f, 0.0f);

	propagateRedSHTexture[elementPos] = float4(0.0f, 0.0f, 0.0f, 0.0f);
	propagateGreenSHTexture[elementPos] = float4(0.0f, 0.0f, 0.0f, 0.0f);
	propagateBlueSHTexture[elementPos] = float4(0.0f, 0.0f, 0.0f, 0.0f);
}