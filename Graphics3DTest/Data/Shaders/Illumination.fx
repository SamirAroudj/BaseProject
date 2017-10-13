/**
	Author: Samir Aroudj
*/

static const int LIGHT_DIRECTIONAL = 0, LIGHT_POINT = 1, LIGHT_SPOT = 2;

struct Surface
{
	float4 diffuse;
	float3 normal;
	float hardness;
	float3 position;
};

struct Light
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 position;
	float range;
	float3 direction;
	float spotLightFactor;
	float3 attenuationFactors;
	int type;
};

float3 processDirectionalLight(Surface surface, Light light, float3 eyePos)
{
	float3 finalColor = float3(0.0f, 0.0f, 0.0f);
	finalColor += light.ambient.rgb * surface.diffuse.rgb;	// ambient part
	
	float3 toLight = -light.direction;	// diffuse part
	float diffuseFactor = dot(toLight, surface.normal);
	[branch]
	if(diffuseFactor > 0.0f)
	{
		finalColor += diffuseFactor * (light.diffuse.rgb * surface.diffuse.rgb);
	
		float3 toEye = normalize(eyePos - surface.position);	// specular part
		float3 reflectedRay = reflect(light.direction, surface.normal);
		float specularFactor = max(dot(reflectedRay, toEye), 0.0f);
		[branch]
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, surface.hardness);
			finalColor += specularFactor * light.specular.rgb; // material specular color is white
		}
	}
	
	return finalColor;
}

float3 processPositionalLight(Surface surface, Light light, float3 eyePos, float3 toLight, float lightSurfaceDistance)
{
	float3 finalColor = light.ambient.rgb * surface.diffuse.rgb; // ambient part
	float diffuseFactor = dot(toLight, surface.normal);// diffuse part
	
	[branch]
	if(diffuseFactor > 0.0f)
	{
		finalColor += diffuseFactor * (light.diffuse.rgb * surface.diffuse.rgb);
		
		float3 toEye = normalize(eyePos - surface.position); // specular part
		float3 reflectedRay = reflect(-toLight, surface.normal);
		float specularFactor = max(dot(reflectedRay, toEye), 0.0f);
		[branch]
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, surface.hardness);
			finalColor += specularFactor * light.specular.rgb; // material specular color is white
		}
	}
	
	// light intensity / (f1 + f2 * d + f3 * d²)
	return finalColor / dot(light.attenuationFactors, float3(1.0f, lightSurfaceDistance, lightSurfaceDistance * lightSurfaceDistance)); 
}

float3 processPointLight(Surface surface, Light light, float3 eyePos)
{
	float3 toLight = light.position - surface.position;	// range check
	float lightSurfaceDistance = length(toLight);
	if(lightSurfaceDistance > light.range)
		return float3(0.0f, 0.0f, 0.0f);
	toLight /= lightSurfaceDistance;

	return processPositionalLight(surface, light, eyePos, toLight, lightSurfaceDistance);
}

float3 processSpotLight(Surface surface, Light light, float3 eyePos)
{
	float3 finalColor = float3(0.0f, 0.0f, 0.0f);
	float3 toLight = light.position - surface.position;	// range check
	float lightSurfaceDistance = length(toLight);
	if(lightSurfaceDistance > light.range)
		return finalColor;
	toLight /= lightSurfaceDistance;
		
	float lightConeFactor = max(0.0f, dot(-toLight, light.direction));	// compute light cone influence
	if(lightConeFactor > 0.0f)
	{
		finalColor = processPositionalLight(surface, light, eyePos, toLight, lightSurfaceDistance);
		return finalColor * pow(lightConeFactor, light.spotLightFactor);
	}
	else
	{
		return finalColor;
	}
}