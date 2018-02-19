/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cassert>
#include <cmath>
#include "Platform/DataTypes.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"
#include "Math/Quaternion.h"
#include "Math/MathHelper.h"
#include "Math/Vector2.h"

using namespace Math;

const uint32 X_INTERVAL = 0x1 << 0;
const uint32 Y_INTERVAL = 0x1 << 1;
const uint32 Z_INTERVAL = 0x1 << 2;

Real Math::computeHalfSolidAngle(const Vector3 &p, const Vector3 &a, const Vector3 &b, const Vector3 &c)
{
	// compute atan2(det([x y z]) / (||x||*||y||*||z|| + <x, y>*||z|| + <y, x>*||z|| + <z, x>*||y||)
	const Vector3 x = a - p;
	const Vector3 y = b - p;
	const Vector3 z = c - p;
	Real lengths[3] = { x.getLength(), y.getLength(), z.getLength() };

	Real det = 	 x.x * (y.y * z.z - z.y * y.z)
				-y.x * (x.y * z.z - z.y * x.z)
				+z.x * (x.y * y.z - y.y * x.z);

	Real denominator = lengths[0] * lengths[1] * lengths[2];
	denominator		+= y.dotProduct(z) * lengths[0];
	denominator		+= z.dotProduct(x) * lengths[1];
	denominator		+= x.dotProduct(y) * lengths[2];

	return atan2r(det, denominator); 
}

Real Math::computeSolidAngle(const Vector2 &p, const Vector2 &a, const Vector2 &b)
{
	// compute atan2(x.x*y.y - x.y*y.x, x.x * y.x + x.y * y.y)
	Real x[2] = { a.x - p.x, a.y - p.y };
	Real y[2] = { b.x - p.x, b.y - p.y };

	return atan2r(x[0] * y[1] - x[1] * y[0], x[0] * y[0] + x[1] * y[1]);
}

Matrix4x4 Math::createMatrix4x4FromMatrix3x3(const Matrix3x3 &m)
{
	return Matrix4x4(	m.m00, m.m01, m.m02, 0.0f,
						m.m10, m.m11, m.m12, 0.0f,
						m.m20, m.m21, m.m22, 0.0f,
						 0.0f,	0.0f,  0.0f, 1.0f	);
}

Matrix4x4 Math::createMatrix4x4FromQuaternion(const Quaternion &q)
{
	const Matrix3x3 m = createMatrix3x3FromQuaternion(q);
	return Matrix4x4(m.m00, m.m01, m.m02, 0.0f,
					 m.m10, m.m11, m.m12, 0.0f,
					 m.m20, m.m21, m.m22, 0.0f,
					 0.0f,  0.0f,  0.0f,  1.0f);
};

Matrix3x3 Math::createMatrix3x3FromQuaternion(const Quaternion &q)
{ 
	// IF YOU CHANGE THIS - THEN ALSO CHANGE createMatrix4x4FromQuaternion
	assert(fabsr(q.getLengthSquared() - 1.0f) < EPSILON);
	Real tx = 2.0f * q.x;
	Real ty = 2.0f * q.y;
	Real tz = 2.0f * q.z;
	Real txSq = tx * q.x;
	Real tySq = ty * q.y;
	Real tzSq = tz * q.z;
	Real twx = q.w * tx;
	Real twy = q.w * ty;
	Real twz = q.w * tz;
	Real txy = tx * q.y;
	Real txz = tx * q.z;
	Real tyz = ty * q.z;

	return Matrix3x3(	1.0f - tySq - tzSq,	txy + twz,			txz - twy,
						txy - twz,			1.0f - txSq - tzSq,	tyz + twx,
						txz + twy,			tyz - twx,			1.0f - txSq - tySq);
}

Quaternion Math::createQuaternionFromMatrix(const Matrix3x3 &m)
{
	return createQuaternionFromAxes(Vector3(m.m00, m.m01, m.m02),
									Vector3(m.m10, m.m11, m.m12),
									Vector3(m.m20, m.m21, m.m22));
}

Quaternion Math::createQuaternionFromMatrix(const Matrix4x4 &m)
{
	return createQuaternionFromAxes(Vector3(m.m00, m.m01, m.m02),
									Vector3(m.m10, m.m11, m.m12),
									Vector3(m.m20, m.m21, m.m22));
}

Quaternion Math::createQuaternionFromAxes(const Vector3 x, const Vector3 y, const Vector3 z)
{
	Real trace = x.x + y.y + z.z;

	//Real trace = m.m00 + m.m11 + m.m22;
	if (trace > 0)
	{

		//Quaternion q(m.m12 - m.m21, m.m20 - m.m02, m.m01 - m.m10, trace + 1.0f);
		Quaternion q(y.z - z.y, z.x - x.z, x.y - y.x, trace + 1.0f);
		q.normalize();
		return q;
	}

	if (x.x > y.y && x.x > z.z)
	{
		//Quaternion q(	m.m00 - m.m11 - m.m22 + 1.0f,
		//				m.m10 + m.m01,
		//				m.m20 + m.m02,
		//				m.m12 - m.m21)
		Quaternion q(x.x - y.y - z.z + 1.0f,
						y.x + x.y,
						z.x + x.z,
						y.z - z.y);
		q.normalize();
		return q;
	}

	if (y.y > z.z)
	{
		//Quaternion	q(	m.m10 + m.m01,
		//				m.m11 - m.m00 - m.m22 + 1.0f,
		//				m.m21 + m.m12,
		//				m.m20 - m.m02);
		Quaternion	q(	y.x + x.y,
						y.y - x.x - z.z + 1.0f,
						z.y + y.z,
						z.x - x.z);
		q.normalize();
		return q;
	}

	// z.z >= x.x && z.z >= y.y
	//Quaternion q(	m.m20 + m.m02,
	//				m.m12 + m.m21,
	//				m.m22 - m.m00 - m.m11 + 1.0f,
	//				m.m01 - m.m10);
	Quaternion q(	z.x + x.z,
					y.z + z.y,
					z.z - x.x - y.y + 1.0f,
					x.y - y.x);
	q.normalize();
	return q;
}

Math::Vector3 Math::getAABBCorner(const Math::Vector3 &AABBMin, const Real AABBSize, const uint32 cornerIdx)
{	
	Vector3 corner(AABBMin);

	if (0 != cornerIdx % 2)
		corner.x = AABBSize;
	if (0 != (cornerIdx / 2) % 2)
		corner.y = AABBSize;
	if (0 == cornerIdx / 4)
		corner.z = AABBSize;

	return corner;
}

Math::Vector3 Math::getAABBCorner(const Math::Vector3 &AABBMin, const Math::Vector3 &AABBMax, const uint32 cornerIdx)
{
	Vector3 corner(AABBMin);

	if (0 != cornerIdx % 2)
		corner.x = AABBMax.x;
	if (0 != (cornerIdx / 2) % 2)
		corner.y = AABBMax.y;
	if (0 == cornerIdx / 4)
		corner.z = AABBMax.z;

	return corner;
}

void Math::getXYProjectionScaleFactors(Real &sX, Real &sY, const Real f, const Real aspectRatio)
{
	sX = f;
	sY = f;

	// always scale smaller side length coordinates according to aspect ratio
	const bool landscape = (aspectRatio >= 1.0f); // if width >= height: landscape | if height > width: portrait
	if (landscape) // stretch image along y-axis to make it square
		sY *= aspectRatio;
	else // stretch image along x-axis to make it square
		sX /= aspectRatio;
}

Real Math::normalizeAngle(Real radian)
{
	while(radian > TWO_PI)
		radian -= TWO_PI;

	while(radian < 0.0f)
		radian += TWO_PI;

	return radian;
}

uint32 Math::power2RoundUp(uint32 n)
{
    --n;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;

    return n + 1;
}

Real Math::transformCartesianToSpherical(Real &azimuthAngle, Real &polarAngle, const Vector3 &cartesian, Real length)
{
	if (length <= 0.0f)
		length = cartesian.getLength();

	const Real temp	= clamp(cartesian.y / length, (Real) 1.0f, (Real) -1.0f);
	polarAngle = acosr(temp);

	azimuthAngle	= atan2r(cartesian.z, cartesian.x);
	if (azimuthAngle >= Math::TWO_PI)
		azimuthAngle -= Math::TWO_PI;
	if (azimuthAngle < 0.0f)
		azimuthAngle += Math::TWO_PI;

	return length;
}

void Math::transformSphericalToCartesian(Vector3 &cartesian, const Real azimuthAngle, const Real polarAngle, const Real r)
{
	const Real cosAzi = cosr(azimuthAngle);
	const Real sinAzi = sinr(azimuthAngle);
	const Real cosPol = cosr(polarAngle);
	const Real sinPol = sinr(polarAngle);

	cartesian.x = sinPol * cosAzi;
	cartesian.z = sinPol * sinAzi;
	cartesian.y = cosPol;

	cartesian *= r;
}
