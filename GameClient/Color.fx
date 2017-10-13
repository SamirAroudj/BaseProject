/**
	Author: Samir Aroudj
*/
cbuffer cbPerObject
{
	Real4x4 gWVP;
};

void VS(Real3 positionModel : POSITION,
		Real4 iColor : COLOR,
		out Real4 positionDevice : SV_POSITION,
		out Real4 oColor : COLOR)
{
	positionDevice = mul(Real4(positionModel, 1.0f), gWVP);
	oColor = iColor;
}

Real4 PS(Real4 positionDevice : SV_POSITION,
		Real4 color : COLOR) : SV_Target
{
	return color;
}

technique10 DefaultTechnique
{
	pass Pass0
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}

