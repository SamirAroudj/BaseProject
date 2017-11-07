/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <iostream>
#include "Math/MathHelper.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector2.h"

using namespace Math;
using namespace std;

Real Vector2::computeDistanceTo(Real &t, const Vector2 &p0, const Vector2 &p1) const
{
	const Vector2 segment = p1 - p0;
	const Real segmentLengthSq = segment.getLengthSquared();

	// initialize t
	t = REAL_MAX;

	// p0 == p1?
	if (segmentLengthSq <= EPSILON * EPSILON)
		return computeDistanceTo(p0);

	// project this point onto the straight line line(t) = p0 + t * (p1 - p0)
	t = (*this - p0).dotProduct(segment) / segmentLengthSq;
	
	// is this cloesest / beyond p0 or p1?
	if (t <= 0.0f)
		return computeDistanceTo(p0);
	else if (t >= 1.0f)
		return computeDistanceTo(p1);
	
	// projection(this) is closer to this than p0 and p1
	const Vector2 projection(p0 + segment * t);
	return computeDistanceTo(projection);
}
		
Real Vector2::getAngle(const Vector2 &rhs) const
{
	Real temp = this->dotProduct(rhs) / (this->getLength() * rhs.getLength());
	clamp<Real>(temp, 1.0f, -1.0f);
	return acosr(temp);
}

Real Vector2::getAngleWithNormalizedVectors(const Vector2 &rhs) const
{
	Real temp = this->dotProduct(rhs);
	clamp<Real>(temp, 1.0f, -1.0f);
	return acosr(temp);
}

bool Vector2::normalize(const Real safetyThreshold)
{
	const Real length = getLength();
	const Real temp = 1.0f / length;
	const bool safe = length > safetyThreshold;

	x *= temp;
	y *= temp;

	return safe;
}

void Vector2::rotate(Real angle)
{
	Real cosA	= cosr(angle);
	Real sinA	= sinr(angle);
	Real tx		= cosA * x - sinA * y;
		 y		= sinA * x + cosA * y;
		 x		= tx;
}

ostream &operator <<(ostream &os, const Vector2 &rhs)
{
	os << "(" << rhs.x << "/" << rhs.y << ")";
	return os;
}
