

RWStructuredBuffer<float> globalLightIdx : register(u0);
RWStructuredBuffer<float> ptLowerBound	: register(u1);
RWStructuredBuffer<float> ptHigherBound : register(u2);

RWStructuredBuffer<int> globalLightIdxCounter : register(u3);
//RWTexture1D<uint> globalLightIdxCounter : register( u2 );

//cbuffer Lights : register( b0 )
//{
//uint nLights = 1;
//};
cbuffer CameraData : register(cb0)
{
	matrix cameraView;
	matrix viewProj;

	float3 cameraPos;
	float  cameraFOV;

	float  viewWidth;
	float  viewHeight;
	float  cameraNear;
	float  cameraFar;

	float3 cameraDir;
	float  cameraPad;
}

struct LightData
{
	float3 pos;
	float  range;
};

Texture2DMS<float4> depthTexture : register(t0);
StructuredBuffer<LightData> LightGeometry : register(t1);

cbuffer CullingData : register (cb1)
{
	uint4 nLights;
};


#define pixels_in_tile 16
#define LIGHTS_MAX 512

//uint globalLightIdx[LIGHTS_MAX*100];
//uint ptLowerBound[3600];

//global shared uint globalLightIdxCounter = 0;

groupshared uint minDepthInt = 0xFFFFFFFF;
groupshared uint maxDepthInt = 0;

groupshared uint localLightIdx[LIGHTS_MAX];
groupshared uint localLightIdxCounter = 0;
groupshared uint localLightIdxStart;

void appendLight(uint i)
{
	uint dstIdx = 0;
	InterlockedAdd(localLightIdxCounter, 1, dstIdx);
	if (dstIdx < LIGHTS_MAX)
		localLightIdx[dstIdx] = i;
}

float4 projToView(float xCS, float yCS, float zCS)
{
	float3 position = float3(xCS, yCS, zCS);

	float fRatio = viewWidth / viewHeight;
	float fWorldHeightDiv2 = cameraNear * tan(cameraFOV * 0.5f);
	float fWorldWidthDiv2 = fWorldHeightDiv2 * fRatio;

	position.xy /= float2(viewWidth, viewHeight);
	position.xy *= float2(2, -2);
	position.xy -= float2(1, -1);

	position *= float3(fWorldWidthDiv2, fWorldHeightDiv2, cameraNear);
	return float4(position, 1.0f);
}

float4 createPlane(in float4 p1, in float4 p2, in float4 p3)
{
	float3 v1 = (p2 - p1).xyz;
	float3 v2 = (p3 - p1).xyz;

	//float3 normal = float3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
	float3 normal = cross(v1, v2);
	normal = normalize(normal);
	
	return float4(normal, -(normal.x*p1.x + normal.y*p1.y + normal.z*p1.z));
}

bool overlaps(in float4 frustum[4], in LightData data, in float minGroupDepth, in float maxGroupDepth)
{
	//float3 lightVS = data.pos - cameraPos;
	//float dist = dot(lightVS, float3(frustum[0], frustum[1], frustum[2])) - frustum[3];

	//return abs(dist) <= data.range;
	bool bOverlaps = false;
	//float3 lightVS = data.pos - cameraPos;
	float4 lightVS = mul(float4(data.pos, 1.0f), cameraView);

	//if (dot(lightVS.xyz, cameraDir) < 0) return false;
	float distance = length(lightVS);
	//return false;
	//float distance = lightVS.z;
	if ((distance - data.range) > maxGroupDepth || (distance + data.range) < minGroupDepth)
		return false;
	//return true;
	for (int i = 0; i < 4; i++)
	{
		float dist = dot(lightVS.xyz, float3(frustum[i].x, frustum[i].y, frustum[i].z)) - frustum[i].w;
		if ((abs(dist) < data.range))
			//if (frustum[i].w == 0 && frustum[i].z == 0 && frustum[i].y == 0 && frustum[i].x == 0)
			return true;
	}

	return false;
}

bool stupid_overlaps(in uint left, in uint right, in uint top, in uint bottom, in LightData data)
{
	//float3 lightVS = data.pos - cameraPos;
	//if (minDepth < length(lightVS)) return false;

	float4 lightCS = mul(float4(data.pos, 1.0f), viewProj);
	if (lightCS.x >= left && lightCS.x <= right && lightCS.y >= top && lightCS.y <= bottom)
		return true;

	return false;

	//return true;
}

[numthreads(pixels_in_tile, pixels_in_tile, 1)]
void light_culling_cs(uint3 groupId	: SV_GroupID,
	uint3 groupThreadId : SV_GroupThreadID,
	uint  groupIndex : SV_GroupIndex,
	uint3 dispatchThreadId : SV_DispatchThreadID)
{
	float index = dispatchThreadId.y * 1280 + dispatchThreadId.x;
	//uint nLights = 256;
	//	globalLightIdx[index] = index;
	//find min and max depth value per group

	//float depth = depthTexture.Load(dispatchThreadId).r;
	float minDepth = 0xFFFFFFFF;
	float maxDepth = 0;
	for (int i = 0; i < 8; i++)
	{
		float curDepth = depthTexture.Load(int2(dispatchThreadId.x, dispatchThreadId.y), i);
		if (curDepth < minDepth)
			minDepth = curDepth;
		if (curDepth > maxDepth)
			maxDepth = curDepth;
	}
	//minDepth -= 8.0f; if (minDepth < 0) minDepth = 0.5f;
	//maxDepth += 8.0f;
	//float minDepth = depthTexture.Load(dispatchThreadId.xy, 0).r;
	//float maxDepth = depthTexture.Load(dispatchThreadId.xy, 0).r;
	//uint depthInt = asuint(depth);
	uint minDepthIntThread = asuint(minDepth);
	uint maxDepthIntThread = asuint(maxDepth);

	//minDepthInt = 0xFFFFFFFF;
	//maxDepthInt = 0;

	//synchronize threads
	GroupMemoryBarrierWithGroupSync();

	//InterlockedMin(minDepthInt, depthInt);
	//InterlockedMax(maxDepthInt, depthInt);
	InterlockedMin(minDepthInt, minDepthIntThread);
	InterlockedMax(maxDepthInt, maxDepthIntThread);

	//again synchronize
	GroupMemoryBarrierWithGroupSync();

	float minGroupDepth = asfloat(minDepthInt);
	float maxGroupDepth = asfloat(maxDepthInt);

	//check which lights overlaps (pixels_in_tile * pixels_in_tile of lights in parallel)

	/*** create frustum ***/
	float4 frustum[4];
	{
		float4 v[4];
		v[0] = projToView(pixels_in_tile*groupId.x, pixels_in_tile*groupId.y, 1.0f);
		v[1] = projToView(pixels_in_tile*(groupId.x + 1), pixels_in_tile*groupId.y, 1.0f);
		v[2] = projToView(pixels_in_tile*(groupId.x + 1), pixels_in_tile*(groupId.y + 1), 1.0f);
		v[3] = projToView(pixels_in_tile*groupId.x, pixels_in_tile*(groupId.y + 1), 1.0f);

		float4 o = float4(0.0f, 0.0f, 0.0f, 0.0f);

		for (int i = 0; i < 4; i++)
			frustum[i] = createPlane(o, v[i], v[(i + 1) & 3]);
	}

	uint localIdx = groupThreadId.x + groupThreadId.y * pixels_in_tile;
	for (uint i = 0; i < nLights.x; i += (pixels_in_tile * pixels_in_tile))
	{
		uint il = localIdx + i;

		if (il < nLights.x)
		{
			if (overlaps(frustum, LightGeometry[il], minGroupDepth, maxGroupDepth))
			{
				appendLight(il);
			}
		}
	}

	//again synchronize
	GroupMemoryBarrierWithGroupSync();

	//add light indices to global list as well as keep offsets for each tile
	{
		uint startOffset = 0;
		uint tileIdx = groupId.x + groupId.y * 80;
		if (localIdx == 0 && groupThreadId.x == 0 && groupThreadId.y == 0)
		{
			if (localLightIdxCounter != 0)
			{
				InterlockedAdd(globalLightIdxCounter[0], localLightIdxCounter, startOffset);
			}
			ptLowerBound[tileIdx] = startOffset;
			ptHigherBound[tileIdx] = startOffset + localLightIdxCounter;
			localLightIdxStart = startOffset;
		}
		GroupMemoryBarrierWithGroupSync();

		startOffset = localLightIdxStart;

		for (uint i = localIdx; i < localLightIdxCounter; i += (pixels_in_tile * pixels_in_tile))
		{
			globalLightIdx[startOffset + i] = localLightIdx[i];
		}
	}
}