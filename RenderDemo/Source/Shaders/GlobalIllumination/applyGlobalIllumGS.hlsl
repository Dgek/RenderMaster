struct vs_output
{
	float4 pos : SV_POSITION;
	float2 texCoords : TEXCOORDS;
};

struct gs_output
{
	float4 pos : SV_POSITION;
	float2 texCoords : TEXCOORDS;
	float3 frustumRay : FRUSTUM_RAY;
};

cbuffer CameraBuffer : register(c0)
{
	float4 frustumRays[4];
};

[maxvertexcount(4)]
void globalIllumGS( line vs_output input[2], inout TriangleStream< gs_output > outputStream)
{
	//generate quad from line (triangle_strip)
	gs_output output[4];

	//lower-left
	output[0].pos = float4(input[0].pos.x, input[0].pos.y, input[0].pos.z, 1.0f);
	output[0].texCoords = float2(input[0].texCoords.x, input[0].texCoords.y);

	//lower-right
	output[1].pos = float4(input[1].pos.x, input[0].pos.y, input[0].pos.z, 1.0f);
	output[1].texCoords = float2(input[1].texCoords.x, input[0].texCoords.y);

	//upper-left
	output[2].pos = float4(input[0].pos.x, input[1].pos.y, input[0].pos.z, 1.0f);
	output[2].texCoords = float2(input[0].texCoords.x, input[1].texCoords.y);

	//upper-right
	output[3].pos = float4(input[1].pos.x, input[1].pos.y, input[0].pos.z, 1.0f);
	output[3].texCoords = float2(input[1].texCoords.x, input[1].texCoords.y);

	for (uint i = 0; i < 4; i++)
	{
		output[i].frustumRay = frustumRays[i].xyz;
		outputStream.Append(output[i]);
	}

	outputStream.RestartStrip();
}