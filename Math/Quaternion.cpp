/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Math/MathHelper.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"
#include "Math/Quaternion.h"

using namespace Math;
using namespace std;

Quaternion::Quaternion(const Vector3 &axis, Real angle)
{
	assert(fabsr(axis.getLength() - 1.0f) < EPSILON);

	angle *= 0.5f;
	Real sinA = sinr(angle);
	x = sinA * axis.x;
	y = sinA * axis.y;
	z = sinA * axis.z;
	w = cosr(angle);
}

Quaternion Quaternion::createFromExtrinsicAngles(Real radianX, Real radianY, Real radianZ)
{
	// qx * qy * qz
	Real hX = radianX * 0.5f;
	Real hY = radianY * 0.5f;
	Real hZ = radianZ * 0.5f;
	Real cosX = cosr(hX); Real sinX = sinr(hX);
	Real cosY = cosr(hY); Real sinY = sinr(hY);
	Real cosZ = cosr(hZ); Real sinZ = sinr(hZ);

	 return Quaternion(	sinX * cosY * cosZ - cosX * sinY * sinZ,
						cosX * sinY * cosZ + sinX * cosY * sinZ,
						cosX * cosY * sinZ - sinX * sinY * cosZ,
						cosX * cosY * cosZ + sinX * sinY * sinZ);
}

Quaternion Quaternion::createFromIntrinsicAngles(Real radianX, Real radianY, Real radianZ)
{
	// qz * qy * qx
	Real hX = radianX * 0.5f;
	Real hY = radianY * 0.5f;
	Real hZ = radianZ * 0.5f;
	Real cosX = cosr(hX); Real sinX = sinr(hX);
	Real cosY = cosr(hY); Real sinY = sinr(hY);
	Real cosZ = cosr(hZ); Real sinZ = sinr(hZ);

	return Quaternion(	sinX * cosY * cosZ + cosX * sinY * sinZ,
						cosX * sinY * cosZ - sinX * cosY * sinZ,
						cosX * cosY * sinZ + sinX * sinY * cosZ,
						cosX * cosY * cosZ - sinX * sinY * sinZ);
}

Quaternion Quaternion::createInverse(const Quaternion &q)
{
	Real one_lengthSquared = 1.0f / q.getLengthSquared();
	return Quaternion(	-q.x * one_lengthSquared,
						-q.y * one_lengthSquared,
						-q.z * one_lengthSquared,
						 q.w * one_lengthSquared);
}

Quaternion Quaternion::nLerp(const Quaternion &q0, const Quaternion &q1, Real t1)
{
	assert(fabsr(q0.getLength() - 1.0f) < EPSILON && fabsr(q1.getLength() - 1.0f) < EPSILON);

	Real t0 = 1.0f - t1;
	Quaternion q(	q0.x * t0 + q1.x * t1,
					q0.y * t0 + q1.y * t1,
					q0.z * t0 + q1.z * t1,
					q0.w * t0 + q1.w * t1);

	q.normalize();
	return q;
}

Quaternion Quaternion::nLerp(	const Quaternion &q00, const Quaternion &q01,
								const Quaternion &q10, const Quaternion &q11,
								Real s1, Real t1)
{
	assert(fabsr(q00.getLength() - 1.0f) < EPSILON && fabsr(q01.getLength() - 1.0f) < EPSILON);
	assert(fabsr(q10.getLength() - 1.0f) < EPSILON && fabsr(q11.getLength() - 1.0f) < EPSILON);

	Real s0 = 1.0f - s1;
	Real t0 = 1.0f - t1;
	// bilinear interpolation: s=0, t=0 -> q00; s=0, t=1 -> q01; s=1, t=0 -> q10; s=1,q=1 -> q11

	Real f00 = s0 * t0;
	Real f01 = s0 * t1;
	Real f10 = s1 * t0;
	Real f11 = s1 * t1;
	Quaternion interpolated(q00.x * f00 +
							q10.x * f10 +
							q01.x * f01 +
							q11.x * f11,

							q00.x * f00 +
							q10.y * f10 +
							q01.z * f01 +
							q11.w * f11,
							
							q00.x * f00 +
							q10.y * f10 +
							q01.z * f01 +
							q11.w * f11,
							
							q00.x * f00 +
							q10.y * f10 +
							q01.z * f01 +
							q11.w * f11);
	interpolated.normalize();
	return interpolated;
}

Quaternion Quaternion::sLerp(const Quaternion &q0, const Quaternion &q1, Real t1)
{
	assert(fabsr(q0.getLength() - 1.0f) < EPSILON && fabsr(q1.getLength() - 1.0f) < EPSILON);

	// idea: q2 and q0 are the base vectors of a 4 dimensional coordinate system
	// rotate q0 in this coordinate system to create the inerpolated quaternion

	// angle between both vectors
	const Real dotProduct = q0.dotProduct(q1);
	const Real alpha0 = clamp<Real>(acosr(dotProduct), 1.0f, 0.0f);
	if (alpha0 < EPSILON)
		return q0;

	// Gram-Schmidt orthogonalization
	Quaternion q2(	q1.x - dotProduct * q0.x,
					q1.y - dotProduct * q0.y,
					q1.z - dotProduct * q0.z,
					q1.w - dotProduct * q0.w);
	q2  /= sinr(alpha0);

	// rotate q0 towards q2
	const Real alpha = t1 * alpha0;
	const Real cosA = cosr(alpha);
	const Real sinA = sinr(alpha);

	return Quaternion(	q0.x * cosA + q2.x * sinA,
						q0.y * cosA + q2.y * sinA,
						q0.z * cosA + q2.z * sinA,
						q0.w * cosA + q2.w * sinA);
}

Quaternion &Quaternion::operator =(const Quaternion &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;

	return *this;
}

Quaternion &Quaternion::operator *=(Real scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;

	return *this;
}


Quaternion &Quaternion::operator /=(Real scalar)
{
	Real one_scalar = 1.0f / scalar;
	x *= one_scalar;
	y *= one_scalar;
	z *= one_scalar;
	w *= one_scalar;

	return *this;
}

Quaternion Quaternion::operator *(const Quaternion &rhs) const
{
	// IF YOU CHANGE THIS - THEN ALSO CHANGE *= !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return Quaternion(
		 rhs.x * w + rhs.y * z - rhs.z * y + rhs.w * x,
		-rhs.x * z + rhs.y * w + rhs.z * x + rhs.w * y,
		 rhs.x * y - rhs.y * x + rhs.z * w + rhs.w * z,
		-rhs.x * x - rhs.y * y - rhs.z * z + rhs.w * w);
}

Quaternion &Quaternion::operator +=(const Quaternion &rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;

	return *this;
}

Quaternion &Quaternion::operator -=(const Quaternion &rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;

	return *this;
}

void Quaternion::getAxisAndAngle(Real &angle, Vector3 &axis) const
{
	Real one_sinHalfAngle = 1.0f / sqrtr(1.0f - w * w);

	angle = 2.0f * acosr(w);
	axis.x = x * one_sinHalfAngle;
	axis.y = y * one_sinHalfAngle;
	axis.z = z * one_sinHalfAngle;
}

void Quaternion::getExtrinsicRotationAngles(Real &x, Real &y, Real &z) const
{
	Matrix3x3 m = createMatrix3x3FromQuaternion(*this);
	m.getExtrinsicRotationAngles(x, y, z);
}

void Quaternion::getIntrinsicRotationAngles(Real &x, Real &y, Real &z) const
{
	Matrix3x3 m = createMatrix3x3FromQuaternion(*this);
	m.getIntrinsicRotationAngles(x, y, z);
}

void Quaternion::normalize()
{
	Real one_length = 1.0f / sqrtr(x * x + y * y + z * z + w * w);

	x *= one_length;
	y *= one_length;
	z *= one_length;
	w *= one_length;
}

void Quaternion::rotateVector(Real &resultX, Real &resultY, Real &resultZ, Real vX, Real vY, Real vZ) const
{
	assert(fabsr(getLength() - 1.0f) < EPSILON);
	// rotate a vector: q * v * conjugate of q
	// v * conjugate of q
	Quaternion qTemp(	 vX *  w + vY * -z - vZ * -y,
						-vX * -z + vY *  w + vZ * -x,
						 vX * -y - vY * -x + vZ *  w,
						-vX * -x - vY * -y - vZ * -z);

	Quaternion rotatedVector(qTemp * *this);
	resultX = rotatedVector.x;
	resultY = rotatedVector.y;
	resultZ = rotatedVector.z;
}

ostream &operator <<(ostream &os, const Quaternion &q)
{
	cout << "(x: " << q.x << ", y: " << q.y << ", z: " << q.z << ", w: " << q.w << ")";
	return os;
}
