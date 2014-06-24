struct vs_output
{
	float4 pos : SV_POSITION;
	float3 posWS : POSITION_WS;
};

cbuffer MatrixBuffer : register ( b0 )
{
	matrix World;
	matrix WVP;
}

vs_output showgrid_vs( float3 pos : POSITION )
{
	vs_output output;

	float4 worldPos = mul(float4(pos, 1.0f), World);
	output.posWS = worldPos.xyz;
	output.pos = mul(float4(pos, 1.0f), WVP);

	return output;
}