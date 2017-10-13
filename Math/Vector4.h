/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _VECTOR_4_H_
#define _VECTOR_4_H_

#ifdef _LINUX
#include <cstdlib>
#endif // _LINUX

#include <cassert>
#include <iostream>
#include "Math/MathCore.h"
#include "Math/Matrix4x4.h"

namespace Math
{
	class Vector4
	{
	public:
		inline Vector4();

		inline Vector4(const Vector4 &rhs);

		inline Vector4(Real x, Real y, Real z, Real w);

		inline Real &operator [](Axis n);
		inline Real operator [](Axis n) const;

		inline Vector4 &operator =(const Vector4 &rhs);

		/** Checks whether this vector is equal to the other one.
		@param rhs Check is done between this and rhs: this vector == rhs (with some tolerance)?
		@return Returns true if all four vector components are equal. (with tolerance componentTolerance) */
		inline bool operator ==(const Vector4 &rhs) const;

		inline bool operator !=(const Vector4 &rhs) const;

		inline Vector4 operator -() const;
		
		/** Does scalar multiplication, multiplies every vector component by scalar.
		@param scalar Every vector component is multiplied by scalar.
		@return Returns this vector after componentwise multiplication by scalar. */
		inline Vector4 &operator *=(Real scalar);

		/** Does scalar multiplication with 1 / scalar, so it multiplies every vector component by (1 / scalar).
		@param scalar Every vector component is divided by scalar.
		@return Returns this vector after componentwise multiplication by (1 / scalar). */
		inline Vector4 &operator /=(Real scalar);

		inline Vector4 &operator +=(const Vector4 &rhs);

		inline Vector4 &operator -=(const Vector4 &rhs);

		/** Does scalar multiplication, multiplies every vector component by scalar.
		@param scalar Every vector component is multiplied by scalar. 
		@return The returned vector is scaled by scalar. */
		inline Vector4 operator *(Real scalar) const;

		/** Does scalar multiplication with 1 / scalar, so it multiplies every vector component by (1 / scalar).
		@param scalar Every vector component is divided by scalar.  
		@return The returned vector is scaled by (1 / scalar). */
		inline Vector4 operator /(Real scalar) const;

		inline Vector4 operator +(const Vector4 &rhs) const;

		inline Vector4 operator -(const Vector4 &rhs) const;

		inline Vector4 operator *(const Matrix4x4 &rhs) const;
				
		inline bool areOrthogonal(const Vector4 &rhs) const;

		inline Real dotProduct(const Vector4 &rhs) const;

		/** Computes a pseudo dot product with absolute values for products.\n
			pseudoDotProduct(v1, v0) = |v0.x * v1.x| + |v0.y * v1.y| + |v0.z * v1.z| + |v0.w * v1.w|
			This dot product is the maximum if you can freely choose the signs of the vector components.
			E.g., useful for efficient separating axes theorem (SAT) collision tests.
		@param rhs Computes the pseudo / absolute dot product between this vector and rhs. 
		@return Returns PDP = pseudo dot product = sum over i \in {x, y, z, w} : |this.i * rhs.i| */
		inline Real dotProductAbsolute(const Vector4 &rhs) const;

		/** Checks whether this vector is equal to the other one.
		@param rhs Check is done between this and rhs: this vector == rhs (with some tolerance)?
		@param epsilon Two vector component are considered to be equal if their absolute difference is smaller than or equal to componentTolerance.
		@return Returns true if all four vector components are equal. (with tolerance componentTolerance) */
		inline bool equals(const Vector4 &rhs, const Real epsilon = EPSILON) const;

		Real getAngle(const Vector4 &rhs) const;

		Real getAngleWithNormalizedVectors(const Vector4 &rhs) const;

		inline Real *getData();

		inline const Real *getData() const;

		inline Real getLength() const;

		inline Real getLengthSquared() const;
		
		/** Returns true if any of the vector components represents infinity. 
		@return Returns true if any of the vector components represents infinity.
		@see Math::isInfinity */
		inline bool hasInfiniteComponent() const;

		/** Returns true if any of the vector components it not a number. 
		@return Returns true if any of the vector components it not a number.
		@see Math::isNaN */
		inline bool hasNaNComponent() const;

		/** Returns true if this vector has a length of 1 (except some epsilon error). 
		@return Returns true if this vector is longer or shorter than 1 (except some epsilon error). */
		inline bool hasUnitLength() const;

		/** Computes the inverse for each component so that for each dimension newComponent * oldComponent = 1.
			A component is set to REAL_MAX if it is approximately 0 at the beginning.*/
		inline void invert();
		
		/** This is a convenience function to get the maximum for each component.
		@param rhs The components of rhs are compared to their corresponding counterpart of this.
		@return The returned vector contains the maximum component of both vectors for each dimension. */
		inline Vector4 maximum(const Vector4 &rhs) const;

		/** This is a convenience function to get the minimum for each component.
		@param rhs The components of rhs are compared to their corresponding counterpart of this.
		@return The returned vector contains the minimum component of both vectors for each dimension. */
		inline Vector4 minimum(const Vector4 &rhs) const;

		/** Returns a vector the components of which are the products of the corresponding components of both vectors.
		@param rhs Each component of rhs is multiplied with its corrensponding component of this and written to the result component.
		@return The components of the returned vector are set to rhs.component * this.component. */
		inline Vector4 multiply(const Vector4 &rhs) const;
		
		/** Keeps the direction of the vector but changes its magnitude to 1. 
		@param safetyThreshold Defines what minimum length of the original vector is required to consider the normalization as safe.
		@return Returns false if the normalization result is unsafe due to the small magnitude of the original vector or true if you can use the result without being worried. */
		bool normalize(const Real safetyThreshold = Math::EPSILON);

		/** Computes the matrix M = this * rhs^t which is created by the outer product of this vector and rhs.
			Its components are mij = this[i] * rhs[j]
		@param rhs The outer product is computed between this and rhs: M = this * rhs^t.
		@return Returns the outer product matrix M = this * rhs^t with the entries mij = this[i] * rhs[j].*/
		inline Matrix4x4 outerProduct(const Vector4 &rhs) const;

		/** Overwrites all vector components.
		@param x This vectors x-component is set to x.
		@param y This vectors y-component is set to y.
		@param z This vectors z-component is set to z.
		@param w This vectors w-component is set to w. */
		inline void set(Real x, Real y, Real z, Real w);
		
	public:
		Real x; /// This is the first component of this 4D vector.
		Real y; /// This is the second component of this 4D vector.
		Real z; /// This is the third component of this 4D vector.
		Real w; /// This is the forth component of this 4D vector.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline Vector4::Vector4() : x(0), y(0), z(0), w(1.0f)
	{
	
	}

	inline Vector4::Vector4(const Vector4 &rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
	{
	
	}

	inline Vector4::Vector4(Real x, Real y, Real z, Real w) : x(x), y(y), z(z), w(w)
	{
	
	}

	inline Real &Vector4::operator [](Axis n)
	{
		switch (n)
		{
			case AXIS_X: return x;
			case AXIS_Y: return y;
			case AXIS_Z: return z;
			case AXIS_W: return w;
			default: assert(false); return x;
		}
	}

	inline Real Vector4::operator [](Axis n) const
	{
		switch (n)
		{
			case AXIS_X: return x;
			case AXIS_Y: return y;
			case AXIS_Z: return z;
			case AXIS_W: return w;
			default: assert(false); return x;
		}
	}

	inline Vector4 &Vector4::operator =(const Vector4 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;

		return *this;
	}

	inline bool Vector4::operator ==(const Vector4 &rhs) const
	{
		return this->equals(rhs);
	}

	inline bool Vector4::operator !=(const Vector4 &rhs) const
	{
		return !(*this == rhs);
	}

	inline Vector4 Vector4::operator -() const
	{
		return Vector4(-x, -y, -z, -w);
	}

	inline Vector4 &Vector4::operator *=(Real scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;

		return *this;
	}

	inline Vector4 &Vector4::operator /=(Real scalar)
	{
		Real one_scalar = 1 / scalar;
		x *= one_scalar;
		y *= one_scalar;
		z *= one_scalar;
		w *= one_scalar;

		return *this;
	}

	inline Vector4 &Vector4::operator +=(const Vector4 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;

		return *this;
	}

	inline Vector4 &Vector4::operator -=(const Vector4 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;

		return *this;
	}

	inline Vector4 Vector4::operator *(Real scalar) const
	{
		return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	inline Vector4 Vector4::operator /(Real scalar) const
	{
		Real one_scalar = 1 / scalar;
		return Vector4(x * one_scalar, y * one_scalar, z * one_scalar, w * one_scalar);
	}

	inline Vector4 Vector4::operator +(const Vector4 &rhs) const
	{
		return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	inline Vector4 Vector4::operator -(const Vector4 &rhs) const
	{
		return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	inline Vector4 Vector4::operator *(const Matrix4x4 &rhs) const
	{
		return Vector4(	x * rhs.m00 + y * rhs.m10 + z * rhs.m20 + w * rhs.m30,
						x * rhs.m01 + y * rhs.m11 + z * rhs.m21 + w * rhs.m31,
						x * rhs.m02 + y * rhs.m12 + z * rhs.m22 + w * rhs.m32,
						x * rhs.m03 + y * rhs.m13 + z * rhs.m23 + w * rhs.m33);
	}
				
	inline bool Vector4::areOrthogonal(const Vector4 &rhs) const
	{
		return fabsr(dotProduct(rhs)) < EPSILON;
	}

	inline Real Vector4::dotProduct(const Vector4 &rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	}

	inline Real Vector4::dotProductAbsolute(const Vector4 &rhs) const
	{
		return fabsr(x * rhs.x) + fabsr(y * rhs.y) + fabsr(z * rhs.z) + fabsr(w * rhs.w);
	}

	inline bool Vector4::equals(const Vector4 &rhs, const Real epsilon) const
	{
		return	(fabsr(x - rhs.x) < epsilon) &&
				(fabsr(y - rhs.y) < epsilon) &&
				(fabsr(z - rhs.z) < epsilon) &&
				(fabsr(w - rhs.w) < epsilon);
	}

	inline Real *Vector4::getData()
	{
		return reinterpret_cast<Real *>(this);
	}

	inline const Real *Vector4::getData() const
	{
		return reinterpret_cast<Real const *>(this);
	}

	inline Real Vector4::getLength() const
	{
		return sqrtr(x * x + y * y + z * z + w * w);
	}

	inline Real Vector4::getLengthSquared() const
	{
		return x * x + y * y + z * z + w * w;
	}

	inline bool Vector4::hasInfiniteComponent() const
	{
		return isInfinity(x) || isInfinity(y) || isInfinity(z) || isInfinity(w);
	}

	inline bool Vector4::hasNaNComponent() const
	{
		return isNaN(x) || isNaN(y) || isNaN(z) || isNaN(w);
	}
	
	inline bool Vector4::hasUnitLength() const
	{
		const Real length = getLength();
		const Real error = fabsr(1.0f - length);
		return (EPSILON > error);
	}

	inline void Vector4::invert()
	{
		x = (fabsr(x) > EPSILON ? 1.0f / x : REAL_MAX);
		y = (fabsr(y) > EPSILON ? 1.0f / y : REAL_MAX);
		z = (fabsr(z) > EPSILON ? 1.0f / z : REAL_MAX);
		w = (fabsr(w) > EPSILON ? 1.0f / w : REAL_MAX);
	}

	inline Vector4 Vector4::maximum(const Vector4 &rhs) const
	{
		return Vector4((x >= rhs.x) ? x : rhs.x,
						(y >= rhs.y) ? y : rhs.y,
						(z >= rhs.z) ? z : rhs.z,
						(w >= rhs.w) ? w : rhs.w);
	}

	inline Vector4 Vector4::minimum(const Vector4 &rhs) const
	{
		return Vector4((x <= rhs.x) ? x : rhs.x,
						(y <= rhs.y) ? y : rhs.y,
						(z <= rhs.z) ? z : rhs.z,
						(w <= rhs.w) ? w : rhs.w);
	}

	inline Vector4 Vector4::multiply(const Vector4 &rhs) const
	{
		return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
	}

	inline Matrix4x4 Vector4::outerProduct(const Vector4 &rhs) const
	{
		return Matrix4x4(x * rhs.x, x * rhs.y, x * rhs.z, x * rhs.w,
							y * rhs.x, y * rhs.y, y * rhs.z, y * rhs.w,
							z * rhs.x, z * rhs.y, z * rhs.z, z * rhs.w,
							w * rhs.x, w * rhs.y, w * rhs.z, w * rhs.w);
	}

	inline void Vector4::set(Real x, Real y, Real z, Real w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
}

std::ostream& operator <<(std::ostream &os, const Math::Vector4 &rhs);

#endif // _VECTOR_3_H_

