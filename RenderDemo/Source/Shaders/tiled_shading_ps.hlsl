struct LightParams
{
	float4 color;
	float4 pos;
	float3 dir;
	float  range;
	float4 spotAngles;
};

StructuredBuffer<LightParams> gLights : register(t0);
StructuredBuffer<float> LightIndexBuffer : register(t1);
StructuredBuffer<float> ptLowerBound : register(t2);
StructuredBuffer<float> prHigherBound : register(t3);
StructuredBuffer<int> nLights : register(t4);

Texture2D<float4> diffuseMap	:	register(t5);
Texture2D<float4> specularMap	:	register(t6);
Texture2D<float4> glossMap		:	register(t7);
Texture2D<float4> normalMap		:	register(t8);
Texture2D<float4> heightMap		:	register(t9);

Texture2D<float> shadowMap : register(t10);

SamplerState	anisotropicSampler : register(s0);


//uint nLights = 3;

cbuffer CameraBuffer	:	register(b0)
{
	//float4x4 inverseViewProj;
	float4 cameraPos;
};

cbuffer ShadowBuffer : register(b1)
{
	matrix shadowViewProj;
};

struct ps_input
{
	float4 pos : SV_POSITION;
	float4 posWS : POSITION_WS;
	float2 texCoords : TEXCOORDS;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

#define pixels_in_tile 16
#define num_tiles 3600

float ComputeShadow(in float4 positionWS, in float distance)
{
	float4 result = mul(shadowViewProj, positionWS);
		result.xyz /= result.w;
	float shadow = 1.0f;

	float shadowMapDistance = shadowMap.Load(int3(result.xy, 0));
	if (shadowMapDistance < distance)
		shadow = 0.0f;

	//shadow /= PCF_NUM_SAMPLES;
	return shadow;
}

uint GetTileIdx(in float2 screenPos)
{
	uint x = ceil(screenPos.x / pixels_in_tile) - 1;
	uint y = ceil(screenPos.y / pixels_in_tile) - 1;

	//return y;
	return 80 * y + x;
}

void GetTileOffsets(in uint tileIdx, out uint startIdx, out uint endIdx)
{
	startIdx = ptLowerBound[tileIdx];
	endIdx = prHigherBound[tileIdx];
}

float BlinnPhongSpecular(in float specPower, in float n_dot_h)
{
	float normalization = (specPower + 2.0f) / (8.0f);
	float blinn_phong = pow(n_dot_h, specPower);
	return normalization * blinn_phong;
}

float TrowbridgeReitzSpecular(in float smoothness, in float n_dot_h)
{
	const float Pi = 3.14159265f;

	float roughness = 1.0f - smoothness;

	return (roughness * roughness) / (Pi * pow((n_dot_h * n_dot_h * (roughness * roughness - 1) + 1), 2.0f));
}

float ShlickVisibility(in float specPower, in float n_dot_l, in float n_dot_v)
{
	const float Pi = 3.14159265f;
	float PI_OVER_TWO = Pi / 2.0f;
	float PI_OVER_FOUR = Pi / 4.0f;

	float alpha = 1 / sqrt(PI_OVER_FOUR * specPower + PI_OVER_TWO);
	return 0.5f / ((n_dot_l * (1.0f - alpha) + alpha) * (n_dot_v * (1.0f - alpha) + alpha));
}

float3 PhysicallyBasedLightning(in float3 position,
	in float3 normal,
	in float  depth,
	in float3 diffuse,
	in float3 specular,
	in float  specPower,
	in float smoothness,
	in LightParams light)
{
	const float Pi = 3.14159265f;

	//get inverse light direction
	//float3 l = light.pos - position;
	float3 l = float3(-0.2403, 0.926800013, -0.28859999);

		//distance to the light source
		float distance = length(l);

	//normalize
	l /= distance;

	//calculate view vector
	float3 v = cameraPos.xyz - position;
	v = normalize(v);

	//half vector
	float3 h = normalize(l + v);

		float n_dot_l = saturate(dot(normal, l));
	float n_dot_v = saturate(dot(normal, v));
	float h_dot_l = saturate(dot(h, l));
	float n_dot_h = saturate(dot(normal, h));

	//*****************************************
	//Light attenuation
	//*****************************************
	float intencity = distance / light.range.x;
	//float atten = max(0.0f, 1.0f - (distance / lightRange.x));
	float atten = pow(saturate(1.0f - pow(intencity, 4.0f)), 2.0f) / (pow(intencity, 2.0f) + 1.0f);
	float3 attenColor = light.color.xyz * atten;

		//return float3(intencity, intencity, intencity);

		//*****************************************
		//Specular Term
		//
		//specular = (Pi / 4.0f) * specular_term * cosine_term * fresnel_term * visibility_term * lightColor
		//*****************************************

		//calculate specular term
		//float specular_term = BlinnPhongSpecular(specPower, n_dot_h);
		float specular_term = TrowbridgeReitzSpecular(smoothness, n_dot_h);

	//cosine term
	float cosine_term = n_dot_l;

	//fresnel reflection (Shlick's approximation)
	float base = 1.0f - h_dot_l;
	float exp = pow(base, 5.0f);
	float3 fresnel_term = specular + (1.0f - specular) * exp;

		//visibility term (Smith shadowing function)
		//float visibility_term = 1.0f; 
		float visibility_term = ShlickVisibility(specPower, n_dot_l, n_dot_v);
	//float visibility_term = 1.0f / (h_dot_l * h_dot_l);

	//!!! - finally calculate specular reflectance
	float3 finalSpecular = specular_term * cosine_term * fresnel_term * visibility_term * attenColor;

		//*****************************************
		//Diffuse Term - Energy Conserving!!!!!
		//*****************************************
		float3 finalDiffuse = n_dot_l * (1.0f - fresnel_term) * attenColor * diffuse;
		//return float3(attenColor, attenColor, attenColor);
		//return attenColor;
		return (finalDiffuse + finalSpecular);
	//return finalDiffuse;
	//return finalSpecular;
}

float4 shading_ps(ps_input input) : SV_TARGET
{
	float4 lightPosition = float4(0, 1500, 0, 1.0f);

	//get all material data for fragment
	int3 sampleCoords = int3(input.pos.xy, 0);

	float3 position = input.posWS.rgb;
	//float3 normal	=	normalMap.Load(input.texCoords).xyz;

	float3 normalWS = normalize(input.normal);
	float3 tangentWS = normalize(input.tangent);
	float3 binormalWS = normalize(input.binormal);

	float3x3 TangentToWorldSpace = float3x3((float3)tangentWS,
	(float3)binormalWS,
	(float3)normalWS);

	float3 normalTS = normalMap.Sample(anisotropicSampler, input.texCoords).xyz;
		float3 normal = normalize(normalTS * 2.0f - 1.0f);
		normal = mul(normal, TangentToWorldSpace);
	normal = normalize(normal);

	//if (normalTS.x == 0 && normalTS.y == 0 && normalTS.z == 1.0f)
	//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
	//else
	//	return float4(0.0f, 0.0f, 0.0f, 1.0f);
	//normal = normalWS;

	//return float4(normal, 1.0f);

	//depth		=	depthTexture.Load(sampleCoords).x;
	//float3 diffuse	=	diffuseMap.Load(input.texCoords).xyz;
	float3 diffuse = diffuseMap.Sample(anisotropicSampler, input.texCoords).xyz;
		//return float4(diffuse, 1.0f);
		//float3 diffuse	= float3(1.0f, 1.0f, 1.0f);

		//float4 specular	=	specularMap.Load(input.texCoords);
		//float4 specular	=	specularMap.Sample(anisotropicSampler, input.texCoords);
		float4 specular = float4(0.5f, 0.5f, 0.5f, 1.0f);

		//convert smoothness to specular power (2-2048)
		//float smoothness = glossMap.Load(input.texCoords).x;
		float smoothness = glossMap.Sample(anisotropicSampler, input.texCoords).x;
	//float smoothness	= 0.8f;
	float specPower = pow(2, (10 * smoothness + 1));

	uint tileIdx = GetTileIdx(input.pos.xy);
	uint startIdx, endIdx;
	GetTileOffsets(tileIdx, startIdx, endIdx);

	uint numtiles = endIdx - startIdx;
	//if (numtiles < 80 && numtiles > 50) return float4(1.0f, 0.0f, 0.0f, 1.0f);
	//return float4((float)numtiles / 5.0, (float)numtiles / 5.0, (float)numtiles / 5.0, 1.0f);
	//if (numtiles == 2) return float4(1.0f, 0.0f, 0.0f, 1.0f);
	//return float4((float)numtiles/3.0, (float)numtiles/3.0, (float)numtiles/3.0, 1.0f);
	//else return float4(0.0f, 1.0f, 0.0f, 1.0f);
	//if (tileIdx == 3599) return float4(1.0f, 0.0f, 0.0f, 1.0f);
	//else return float4(0.0f, 1.0f, 0.0f, 1.0f);
	//float numtiles = 1;
	//return float4(numtiles / 256.0f, numtiles / 256.0f, numtiles / 256.0f, 1.0f);
	return float4(numtiles, numtiles, numtiles, 1.0f);

	//compute shadowing by using shadow map from direct illumination
	float3 vecToLight = position - lightPosition.xyz;
		float distanceToLight = length(vecToLight);
	float shadow = ComputeShadow(float4(position, 1.0f), distanceToLight);

	float3 finalColor = float3(0.0f, 0.0f, 0.0f);
		//for (uint lightListIdx = startIdx; lightListIdx <= endIdx; lightListIdx++)
		return float4(diffuse, 1.0f);
	for (uint lightListIdx = startIdx; lightListIdx < endIdx; lightListIdx++)
	{
		int lightIdx = LightIndexBuffer[lightListIdx];
		LightParams directLight = gLights[lightIdx];
		//LightParams indirectLight;

		//FetchLight(lightIdx, directLight);
		finalColor += PhysicallyBasedLightning(position, normal, 0.0f, diffuse, specular, specPower, smoothness, directLight);// *shadow;
		//finalColor += float3(0.3f, 0.3f, 0.3f);
	}
	return float4(finalColor, 1.0f);
}

/*#define LIGHT_LOOP_BEGIN
uint tileIdx = GetTileIdx(ScreenPos.xy);
uint startIdx, endIdx;
GetTileOffsets(tileIdx, startIdx, endIdx);

for (uint lightListIdx = startIdx; lightListIdx <= endIdx; lightListIdx++)
{
int lightIdx = LightIndexBuffer[lightListIdx];
LightParams directLight;
//LightParams indirectLight;

FetchLight(lightIdx, directLight);
}

#define LIGHT_LOOP_END */