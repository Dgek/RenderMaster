struct vs_input 
{
	float3 position : POSITION;
	float2 texCoords : TEXCOORDS;
	float3 normal : NORMAL;
};

struct vs_output 
{
	float4 position : SV_POSITION;
	float2 texCoords : TEXCOORDS;
	float3 normal : NORMAL;
};

cbuffer MatrixBuffer : register( b0 )
{
	matrix World;
}; 

vs_output voxelgrid_vs( vs_input input )
{
	vs_output output;

	//output.position = float4(input.position, 1.0f);
	output.position = mul(float4(input.position, 1.0f), World);
	output.texCoords = input.texCoords;
	output.normal = normalize(mul(input.normal, (float3x3)World));
	//output.normal = input.normal;
	//output.texCoords = float2(0.0f, 0.0f);
	//output.normal = float3(0.0f, 1.0f, 0.0f);

	return output;
}