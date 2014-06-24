struct vs_output
{
	float4 position : POSITION;
	uint instanceID : INSTANCEID;
};

struct gs_output
{
	float4 position : SV_POSITION;
	uint rtIndex : SV_RenderTargetArrayIndex;
};

[maxvertexcount(4)]
void point_light_gridGS(line vs_output input[2], inout TriangleStream< gs_output > outputStream)
{
	//generate quad
	gs_output output[4];

	//lower-left vertex
	output[0].position = float4(input[0].position.x, input[0].position.y, input[0].position.z, 1.0f);

	//lower-right vertex
	output[1].position = float4(input[1].position.x, input[0].position.y, input[0].position.z, 1.0f);

	//upper-left vertex
	output[2].position = float4(input[0].position.x, input[1].position.y, input[1].position.z, 1.0f);

	//upper-right vertex
	output[3].position = float4(input[1].position.x, input[1].position.y, input[1].position.z, 1.0f);

	for (uint i = 0; i < 4; i++)
	{
		output[i].rtIndex = input[0].instanceID;
		outputStream.Append(output[i]);
	}

	outputStream.RestartStrip();
}