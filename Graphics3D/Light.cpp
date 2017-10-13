/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Platform/DataTypes.h"
#include "Light.h"

using namespace Graphics;
using namespace std;

vector<Light *> Light::msLights;

LightData::LightData(LightData::Type type, const Color &ambient, const Color &diffuse, const Color &specular,
			const Math::Vector3 &position, const Math::Vector3 &direction, const Math::Vector3 &attenuationFactors,
			Real range, Real spotLightFactor) :
	mType(type), mAmbient(ambient), mDiffuse(diffuse), mSpecular(specular),
	mPosition(position), mDirection(direction), mAttenuationFactors(attenuationFactors),
	mRange(range), mSpotLightFactor(spotLightFactor)
{

}

Light::Light(const Color &ambient, const Color &diffuse, const Color &specular) :
	mAmbient(ambient), mDiffuse(diffuse), mSpecular(specular)
{
	msLights.push_back(this);
}

Light::~Light()
{
	uint32 numOfLights = msLights.size();
	for(uint32 i = 0; i < numOfLights; ++i)
	{
		if (msLights[i] == this)
		{
			msLights[i] = msLights.back();
			msLights.pop_back();
			return;
		}
	}
	assert(false);
}
