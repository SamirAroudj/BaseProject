/**
	Author: Samir Aroudj
*/

cbuffer perObject
{
	float4x4 gWVP;
}

struct VertexShaderInput
{
	float3 localPosition : POSITION;
	float4 color : COLOR;
};

struct VertexShaderOutput
{
	float4 homogenousPosition : SV_POSITION;
	float4 color : COLOR;
};

VertexShaderOutput VS(VertexShaderInput input)
{
	VertexShaderOutput output;
	output.homogenousPosition = mul(float4(input.localPosition, 1.0f), gWVP);
	output.color = input.color;
	
	return output;
}

float4 PS(VertexShaderOutput input) : SV_TARGET
{
	return input.color;
}

technique10 defaultTechnique
{
	pass defaultPass
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}