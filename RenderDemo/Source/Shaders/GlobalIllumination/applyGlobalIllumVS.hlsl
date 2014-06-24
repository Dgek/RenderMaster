struct vs_input
{
	float3 position : POSITION;
};

struct vs_output
{
	float4 pos : SV_POSITION;
	float2 texCoords : TEXCOORDS;
};


vs_output globalIllumVS(vs_input input, uint id : SV_VertexID)
{
	vs_output output;

	output.pos = float4(input.position, 1.0f);
	output.texCoords.x = id % 2;
	output.texCoords.y = (id+1) % 2;

	return output;
}