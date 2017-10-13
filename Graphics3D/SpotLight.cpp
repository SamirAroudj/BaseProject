/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "SpotLight.h"

using namespace Graphics;
using namespace Math;

SpotLight::SpotLight(const Color &ambient, const Color &diffuse, const Color &specular,
			const Vector3 &position, const Vector3 &direction,
			const Math::Vector3 &attenuationFactors, Real range, Real lightConeFactor) :
	Light(ambient, diffuse, specular),
	mAttenuationFactors(attenuationFactors),
	mDirection(direction),
	mPosition(position),
	mLightConeFactor(lightConeFactor),
	mRange(range)
{
	assert(EPSILON > mDirection.getLengthSquared() - 1.0f);
	assert(mLightConeFactor >= 1.0f);
	assert(mRange > 0.0f);
	assert(mAttenuationFactors.x > 0.0f || mAttenuationFactors.y > 0.0f || mAttenuationFactors.z > 0.0f);
}

LightData SpotLight::getData() const
{
	return LightData(LightData::SPOT, mAmbient, mDiffuse, mSpecular,
		mPosition, mDirection, mAttenuationFactors, mRange, mLightConeFactor);
}

void SpotLight::setAttenuationFactors(const Vector3 &attenuationFactors)
{
	mAttenuationFactors = attenuationFactors;
	assert(mAttenuationFactors.x > 0.0f || mAttenuationFactors.y > 0.0f || mAttenuationFactors.z > 0.0f);
}

void SpotLight::setDirection(const Math::Vector3 &direction)
{
	assert(EPSILON > direction.getLengthSquared() - 1.0f);
	mDirection = direction;
}