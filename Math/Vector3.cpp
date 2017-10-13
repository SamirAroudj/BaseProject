/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <iostream>
#include "Math/MathHelper.h"
#include "Math/Vector3.h"

using namespace Math;
using namespace std;

void Vector3::createBasis(Vector3 &b0, Vector3 &b1, Vector3 &b2, bool normalized) const
{
	// compute normalized, orthogonal basis vectors via Gram-Schmidt process 
	b0 = *this;
	if (!normalized)
		b0.normalize();

	b1 = b0.getOrthogonalVector(true);
	b2 = b0.crossProduct(b1);
}

Real Vector3::getAngle(const Vector3 &rhs) const
{
	Real temp = this->dotProduct(rhs) / (this->getLength() * rhs.getLength());
	clamp(temp, 1.0f, -1.0f);
	return acosr(temp);
}

Real Vector3::getAngleWithNormalizedVectors(const Vector3 &rhs) const
{
	Real temp = this->dotProduct(rhs);
	clamp(temp, 1.0f, -1.0f);
	return acosr(temp);
}

Vector3 Vector3::getOrthogonalVector(bool normalize) const
{
	Real	temp;
	Vector3	v;

	// choose an axis as basis for the orthogonal vector
	const Real colinear = fabsr(x); // <x-axis, this>
	if (fabsr(colinear - 1.0f) < Math::EPSILON)
	{
		temp = y; // <y-axis, this>
		v.set(0.0f, 1.0f, 0.0f);
	}
	else
	{
		temp = x; // <x-axis, this>
		v.set(1.0f, 0.0f, 0.0f);
	}

	// make the axis orthogonal (Gram-Schmidt orthogonalization with the chosen axis
	v.x -= temp * x;
	v.y -= temp * y;
	v.z -= temp * z;

	// normalize v if requested
	if (normalize)
		v.normalize();

	return v;
}

bool Vector3::normalize(const Real safetyThreshold)
{
	const Real length = getLength();
	const Real temp = 1.0f / length;
	const bool safe = length >= safetyThreshold;

	x *= temp;
	y *= temp;
	z *= temp;

	return safe;
}

ostream &operator <<(ostream &os, const Vector3 &rhs)
{
	os << "(" << rhs.x << "/" << rhs.y << "/" << rhs.z << ")";
	return os;
}
