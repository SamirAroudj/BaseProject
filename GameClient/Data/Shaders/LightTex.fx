/**
	Author: Samir Aroudj
*/

#include "Illumination.fx"
#define MAX_NUM_OF_LIGHTS 8

Texture2D gDiffTex;

cbuffer perFrame
{
	Light gLights[MAX_NUM_OF_LIGHTS];
	int gNumLights;
	float3 gEyePos;
}

cbuffer perObject
{
	float4x4 gWVP;
	float4x4 gWorld;
	float gHardness;
}

SamplerState gTriLinearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VertexShaderInput
{
	float3 localPosition : POSITION;
	float3 localNormal : NORMAL;
	float2 localTexCoords : TEXCOORD;
};

struct VertexShaderOutput
{
	float4 homogenousPosition : SV_POSITION;
	float3 worldPosition : POSITION;
	float3 worldNormal : NORMAL;
	float2 texCoords : TEXCOORD;
};

VertexShaderOutput VS(VertexShaderInput input)
{
	VertexShaderOutput output;
	output.homogenousPosition = mul(float4(input.localPosition, 1.0f), gWVP);
	output.worldPosition = mul(float4(input.localPosition, 1.0f), gWorld).xyz;
	output.worldNormal = mul(float4(input.localNormal, 0.0f), gWorld).xyz;
	output.texCoords = input.localTexCoords;
	
	return output;
}

float4 PS(VertexShaderOutput input) : SV_TARGET 
{
	Surface surface = { gDiffTex.Sample(gTriLinearSampler, input.texCoords),
						normalize(input.worldNormal),
						gHardness,
						input.worldPosition };

	float3 finalColor = float3(0.0f, 0.0f, 0.0f);
	if(0 == gNumLights)
		return surface.diffuse;
		
	for(int i = 0; i < gNumLights; ++i)
	{
		if(LIGHT_DIRECTIONAL == gLights[i].type)
		{
			finalColor += processDirectionalLight(surface, gLights[i], gEyePos);
		}
		else if(LIGHT_POINT == gLights[i].type)
		{
			finalColor += processPointLight(surface, gLights[i], gEyePos);
		}
		else //if(LIGHT_SPOT == gLight.type)
		{
			finalColor += processSpotLight(surface, gLights[i], gEyePos);
		}
	}
	
	return float4(finalColor, surface.diffuse.a);
}

technique10 TexTech
{
	pass defaultPass
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}
