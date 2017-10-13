/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Math/MathHelper.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector4.h"

using namespace Math;
using namespace std;

Real Vector4::getAngle(const Vector4 &rhs) const
{
	Real temp = this->dotProduct(rhs) / (this->getLength() * rhs.getLength());
	clamp(temp, 1.0f, -1.0f);
	return acosr(temp);
}

Real Vector4::getAngleWithNormalizedVectors(const Vector4 &rhs) const
{
	Real temp = this->dotProduct(rhs);
	clamp(temp, 1.0f, -1.0f);
	return acosr(temp);
}

bool Vector4::normalize(const Real safetyThreshold)
{
	const Real length = getLength();
	const Real temp = 1.0f / length;
	const bool safe = length > safetyThreshold;

	x *= temp;
	y *= temp;
	z *= temp;
	w *= temp;

	return safe;
}

ostream &operator <<(ostream &os, const Vector4 &rhs)
{
	cout << "(" << rhs.x << "/" << rhs.y << "/" << rhs.z << "/" << rhs.w << ")";
	return os;
}