struct vs_input
{
	float3 position : POSITION;
};

struct vs_output
{
	float4 position : POSITION;
	uint instanceID : INSTANCEID;
};

vs_output point_light_gridVS( vs_input input, uint instanceID : SV_InstanceID)
{
	vs_output output;

	output.position = float4(input.position, 1.0f);
	output.instanceID = instanceID;

	return output;
}