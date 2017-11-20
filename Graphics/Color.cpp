/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Graphics/Color.h"
#include "Math/MathHelper.h"
#include "Platform/DataTypes.h"

using namespace Graphics;
using namespace Math;


Color Color::interpolateLinearly(const Color &c0, const Color &c1, const Real temp0)
{
	// interpolation factors
	const float f0 = Math::clamp((float) temp0, 1.0f, 0.0f);
	const float f1 = 1.0f - f0;

	Color c;
	c.mComponents[0] = c0.mComponents[0] * f0 + c1.mComponents[0] * f1;
	c.mComponents[1] = c0.mComponents[1] * f0 + c1.mComponents[1] * f1;
	c.mComponents[2] = c0.mComponents[2] * f0 + c1.mComponents[2] * f1;
	c.mComponents[3] = c0.mComponents[3] * f0 + c1.mComponents[3] * f1;

	return c;
}

Color::Color()
{
	mComponents[0] = 0.0f;
	mComponents[1] = 0.0f;
	mComponents[2] = 0.0f;
	mComponents[3] = 0.0f;
}

Color::Color(float red, float green, float blue) :
	Color(red, green, blue, 1.0f)
{

}

Color::Color(float red, float green, float blue, float alpha)
{
	assert(checkComponents(red, green, blue, alpha));
	mComponents[0] = Math::clamp<float>(red,   1.0f, 0.0f);
	mComponents[1] = Math::clamp<float>(green, 1.0f, 0.0f);
	mComponents[2] = Math::clamp<float>(blue,  1.0f, 0.0f);
	mComponents[3] = Math::clamp<float>(alpha, 1.0f, 0.0f);
}

void Color::convertToHSL(float &H, float &S, float &L) const
{
	// minimum of r, g, b
	float	min	= (mComponents[0] <= mComponents[1] ? mComponents[0] : mComponents[1]);
			min	= (mComponents[2] <= min ? mComponents[2] : min);
	
	// maximum of r, g, b & selection index
	uint32	maxRGB	= 0;
	float	max		= mComponents[0];
	if (mComponents[1] > mComponents[2])
	{
		if (mComponents[1] > max)
		{
			max		= mComponents[0];
			maxRGB	= 1;
		}
	}
	else if (mComponents[2] > mComponents[0])
	{
		max		= mComponents[2];
		maxRGB	= 2;
	}
	
	H = 0.0f;
	S = 0.0f;
	L = (max + min) * 0.5f;

	// achromatic / shade of gray?
	if (fabsr(max - min) < Math::EPSILON)
		return;

	float d = max - min;
	S = (L > 0.5f ? d / (2.0f - max - min) : d / (max + min));
        
	switch (maxRGB)
	{
		case 0:
			H  = (mComponents[1] - mComponents[2]) / d;
			if (mComponents[1] < mComponents[2])
				H +=  6.0f;
			break;

		case 1:
			H = (mComponents[2] - mComponents[0]) / d + 2.0f;
			break;

		case 2:
			H = (mComponents[0] - mComponents[1]) / d + 4.0f;
			break;
	}

	H = (float) Math::TWO_PI * H / 6.0f;
}

Color Color::createFromHSL(float H, float S, float L, float alpha)
{
	assert(H >= 0.0f);
	assert(H < Math::TWO_PI);
	assert(S >= 0.0f);
	assert(S <= 1.0f);
	assert(L >= 0.0f);
	assert(L <= 1.0f);
	assert(alpha >= 0.0f);
	assert(alpha <= 1.0f);
	
	// achromatic or shade of gray
	if (S <= Math::EPSILON)
		return Color(L, L, L, alpha);
	
	// helper variables
	const float q = (L < 0.5f) ? L * (1.0f + S) : L + S - L * S;
	const float p = 2 * L - q;
	H = H / (float) Math::TWO_PI;

	// clampling due to possible numerical errors
	const float r = Math::clamp<float>(helperVariablesToRGB(p, q, H + (1.0f / 3.0f)), 1.0f, 0.0f); 
	const float g = Math::clamp<float>(helperVariablesToRGB(p, q, H), 1.0f, 0.0f);
	const float b = Math::clamp<float>(helperVariablesToRGB(p, q, H - (1.0f / 3.0f)), 1.0f, 0.0f); 

	return Color(r, g, b, alpha);
}

float Color::helperVariablesToRGB(float p, float q, float t)
{
	if (t < 0.0f)
		t += 1.0f;
	if (t > 1.0f)
		t -= 1.0f;
	if (t < (1.0f / 6.0f))
		return p + (q - p) * 6.0f * t;
	if (t < 0.5f)
		return q;
	if (t < (2.0f / 3.0f))
		return p + (q - p) * ((2.0f / 3.0f) - t) * 6.0f;
	return p;
}

Color Color::createFromNormal(const Vector3 &normal, const Real saturation, const Real alpha)
{
	// spherical coordinates
	Real azimuth;
	Real polar;
	Math::transformCartesianToSpherical(azimuth, polar, normal, 1.0f);

	// hue, lightness from spherical coordinates
	const float	hue			= (float) azimuth;
	const float	lightness	= (float) (1.0f - polar / Math::PI);

	return createFromHSL(hue, (float) saturation, lightness, (float) alpha);
}

void Color::set(float red, float green, float blue, float alpha)
{
	assert(checkComponents(red, green, blue, alpha));
	mComponents[0] = red;
	mComponents[1] = green;
	mComponents[2] = blue;
	mComponents[3] = alpha;
}		

bool Color::checkComponents(float red, float green, float blue, float alpha)
{
	return checkComponent(red) && checkComponent(green) && checkComponent(blue) && checkComponent(alpha);
}

bool Color::checkComponent(float component)
{
	return (0.0f <= component) && (component <= 1.0f);
}
