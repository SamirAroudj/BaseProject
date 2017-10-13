/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "PointLight.h"

using namespace Graphics;
using namespace Math;

PointLight::PointLight(const Color &ambient, const Color &diffuse, const Color &specular,
					   const Vector3 &position, const Vector3 &attenuationFactors, Real range) :
	Light(ambient, diffuse, specular),
	mPosition(position),
	mAttenuationFactors(attenuationFactors),
	mRange(range)
{
	assert(mRange > 0.0f);
	assert(mAttenuationFactors.x > 0.0f || mAttenuationFactors.y > 0.0f || mAttenuationFactors.z > 0.0f);
}

PointLight::~PointLight()
{
}

LightData PointLight::getData() const
{
	return LightData(LightData::POINT, mAmbient, mDiffuse, mSpecular, mPosition,
		Vector3(), mAttenuationFactors, mRange);
}

void PointLight::setAttenuationFactors(const Vector3 &attenuationFactors)
{
	mAttenuationFactors = attenuationFactors;
	assert(mAttenuationFactors.x > 0.0f || mAttenuationFactors.y > 0.0f || mAttenuationFactors.z > 0.0f);
}