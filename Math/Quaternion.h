/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include <iostream>
#include "Math/MathCore.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

namespace Math
{
	class Quaternion
	{
	public:
		inline static Quaternion createConjugate(const Quaternion &q);
		static Quaternion createFromExtrinsicAngles(Real radianX, Real radianY, Real radianZ);
		static Quaternion createFromIntrinsicAngles(Real radianX, Real radianY, Real radianZ);
		static Quaternion createInverse(const Quaternion &q);
		inline static Quaternion createInverseFromUnitQuaternion(const Quaternion &q);

		// expects unit quaternions
		static Quaternion nLerp(const Quaternion &q0, const Quaternion &q1, Real t1);

		// expects unit quaternions
		static Quaternion nLerp(const Quaternion &q00, const Quaternion &q01,
								const Quaternion &q10, const Quaternion &q11,
								Real s1, Real t1);

		// expects unit quaternions
		static Quaternion sLerp(const Quaternion &q0, const Quaternion &q1, Real t1);

	public:
		// no rotation
		inline Quaternion();
		inline Quaternion(const Quaternion &copy);

		// requires a unit-length vector as axis
		Quaternion(const Vector3 &axis, Real angle);
		inline Quaternion(Real x, Real y, Real z, Real w);

		Quaternion &operator =(const Quaternion &rhs);

		inline bool operator ==(const Quaternion &rhs) const;

		inline bool operator !=(const Quaternion &rhs) const;

		inline Quaternion operator -() const;

		inline Quaternion operator *(Real scalar) const;

		inline Quaternion operator /(Real scalar) const;

		Quaternion &operator *=(Real scalar);
		Quaternion &operator /=(Real scalar);

		inline Quaternion operator +(const Quaternion &rhs) const;

		inline Quaternion operator -(const Quaternion &rhs) const;

		Quaternion operator *(const Quaternion &rhs) const;
		
		Quaternion &operator +=(const Quaternion &rhs);

		Quaternion &operator -=(const Quaternion &rhs);

		inline Quaternion &operator *=(const Quaternion &rhs);

		inline Real dotProduct(const Quaternion &rhs) const;

		inline Real getLength() const;

		inline Real getLengthSquared() const;

		void getAxisAndAngle(Real &angle, Vector3 &axis) const;
		void getExtrinsicRotationAngles(Real &x, Real &y, Real &z) const;
		void getIntrinsicRotationAngles(Real &x, Real &y, Real &z) const;

		inline void conjugate();

		void normalize();

		inline void rotateVector(Vector3 &result, const Vector3 &rhs) const;

		inline void rotateVector(Vector4 &result, const Vector4 &rhs) const;		
			
		void rotateVector(Real &resultX, Real &resultY, Real &resultZ, Real vX, Real vY, Real vZ) const;

	public:
		Real x, y, z, w;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline Quaternion Quaternion::createConjugate(const Quaternion &q)
	{
		return Quaternion(-q.x, -q.y, -q.z, q.w);
	}

	inline Quaternion Quaternion::createInverseFromUnitQuaternion(const Quaternion &q)
	{
		return Quaternion(-q.x, -q.y, -q.z, q.w);
	}

	inline Quaternion::Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
	{
	}

	inline Quaternion::Quaternion(const Quaternion &copy) : x(copy.x), y(copy.y), z(copy.z), w(copy.w)
	{
	}

	inline Quaternion::Quaternion(Real x, Real y, Real z, Real w) : x(x), y(y), z(z), w(w)
	{
	}

	inline bool Quaternion::operator ==(const Quaternion &rhs) const
	{
		return  fabsr(x - rhs.x) < EPSILON && fabsr(y - rhs.y) < EPSILON && fabsr(z -rhs.z) < EPSILON && fabsr(w - rhs.w) < EPSILON;
	}

	inline bool Quaternion::operator !=(const Quaternion &rhs) const
	{
		return !(*this == rhs);
	}

	inline Quaternion Quaternion::operator -() const
	{
		return Quaternion(-x, -y, -z, -w);
	}

	inline Quaternion Quaternion::operator *(Real scalar) const
	{
		return Quaternion(scalar * x, scalar * y, scalar * z, scalar * w);
	}

	inline Quaternion Quaternion::operator /(Real scalar) const
	{
		Real one_scalar = 1.0f / scalar;
		return Quaternion(one_scalar * x, one_scalar * y, one_scalar * z, one_scalar * w);
	}

	inline Quaternion Quaternion::operator +(const Quaternion &rhs) const
	{
		return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}
	 
	inline Quaternion &Quaternion::operator *=(const Quaternion &rhs)
	{
		return (*this = (*this * rhs));
	}

	inline Quaternion Quaternion::operator -(const Quaternion &rhs) const
	{
		return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	inline Real Quaternion::dotProduct(const Quaternion &rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	}

	inline Real Quaternion::getLength() const
	{
		return sqrtr(x * x + y * y + z * z + w * w);
	}

	inline Real Quaternion::getLengthSquared() const
	{
		return x * x + y * y + z * z + w * w;
	}

	inline void Quaternion::conjugate()
	{
		x *= -1.0f;
		y *= -1.0f;
		z *= -1.0f;
	}

	inline void Quaternion::rotateVector(Vector3 &result, const Vector3 &rhs) const
	{
		rotateVector(result.x, result.y, result.z, rhs.x, rhs.y, rhs.z);
	}

	inline void Quaternion::rotateVector(Vector4 &result, const Vector4 &rhs) const
	{
		rotateVector(result.x, result.y, result.z, rhs.x, rhs.y, rhs.z);
		result.w = rhs.w;
	}		
}

std::ostream& operator<<(std::ostream &os, const Math::Quaternion &q);

#endif // _QUATERNION_H_

