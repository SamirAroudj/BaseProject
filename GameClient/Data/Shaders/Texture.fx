/**
	Author: Samir Aroudj
*/
Texture2D gDiffTex;

cbuffer perObject
{
	float4x4 gWVP;
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
	float2 localTexCoords : TEXCOORD;
};

struct VertexShaderOutput
{
	float4 homogenousPosition : SV_POSITION;
	float2 texCoords : TEXCOORD;
};

VertexShaderOutput VS(VertexShaderInput input)
{
	VertexShaderOutput output;
	output.homogenousPosition = mul(float4(input.localPosition, 1.0f), gWVP);
	output.texCoords = input.localTexCoords;
	
	return output;
}

float4 PS(VertexShaderOutput input) : SV_TARGET 
{
	float4 color = gDiffTex.Sample(gTriLinearSampler, input.texCoords);
	
	return color;
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