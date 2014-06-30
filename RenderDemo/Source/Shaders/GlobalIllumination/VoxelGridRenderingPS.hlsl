#include "GlobalIllumination.hlsli"

struct vs_output
{
	float4 pos : SV_POSITION;
	float3 posWS : POSITION_WS;
};

StructuredBuffer<VOXEL> voxelGrid : register( t0 );

float4 showgrid_ps(vs_output input) : SV_TARGET
{
	float3 gridCenter = float3(0.0f, 500.0f, 0.0f);
	float  invCellSize = 1.0f / 60.5f;

	float3 offset = (input.posWS - gridCenter) * invCellSize;
	offset = round(offset);

	int3 gridPos = int3(32,32,32) + int3(offset);
	uint gridIndex = gridPos.z * 4096 + gridPos.y * 64 + gridPos.x;

	float3 color = float3(0.5f, 0.5f, 0.5f);

	if (gridPos.x < 0 || gridPos.x >= 64)
		color = float3(1.0f, 0.0f, 0.0f);
	else if(gridPos.y < 0 || gridPos.y >= 64)
		color = float3(0.0f, 1.0f, 0.0f);
	else if(gridPos.z < 0 || gridPos.z >= 64)
		color = float3(0.0f, 0.0f, 1.0f);
	else
		color = DecodeColor(voxelGrid[gridIndex].color);
	
	return float4(color, 1.0f);
}