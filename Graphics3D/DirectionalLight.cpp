/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "DirectionalLight.h"

using namespace Graphics;
using namespace Math;

DirectionalLight::DirectionalLight(const Color &ambient, const Color &diffuse, const Color &specular, const Vector3 &direction) :
	Light(ambient, diffuse, specular),
	mDirection(direction)
{
}

LightData DirectionalLight::getData() const
{
	return LightData(LightData::DIRECTIONAL, mAmbient, mDiffuse, mSpecular, Vector3(), mDirection);
}

void DirectionalLight::setDirection(const Math::Vector3 &direction)
{
	assert(EPSILON > direction.getLengthSquared() - 1.0f);
	mDirection = direction;
}
