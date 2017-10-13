/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _VECTOR_2_H_
#define _VECTOR_2_H_

#ifdef _LINUX
#include <cstdlib>
#endif // _LINUX

#include <cassert>
#include <iostream>
#include "Math/MathCore.h"
#include "Math/Matrix2x2.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"

namespace Math
{
	class Vector2
	{
	public:
		inline Vector2();

		inline Vector2(const Vector2 &rhs);

		inline Vector2(Real x, Real y);

		inline Real &operator [](Axis n);
		inline Real operator [](Axis n) const;

		inline Vector2 &operator =(const Vector2 &rhs);

		/** Checks whether this vector is equal to the other one.
		@param rhs Check is done between this and rhs: this vector == rhs (with some tolerance)?
		@return Returns true if all four vector components are equal. (with tolerance componentTolerance) */
		inline bool operator ==(const Vector2 &rhs) const;

		inline bool operator !=(const Vector2 &rhs) const;

		inline Vector2 operator -() const;

		/** Does scalar multiplication, multiplies every vector component by scalar.
		@param scalar Every vector component is multiplied by scalar.  
		@return The returned vector is scaled by scalar.*/
		inline Vector2 &operator *=(Real scalar);

		/** Does scalar multiplication with 1 / scalar, so it multiplies every vector component by (1 / scalar).
		@param scalar Every vector component is divided by scalar. 
		@return The returned vector is scaled by (1 / scalar). */
		inline Vector2 &operator /=(Real scalar);

		inline Vector2 &operator +=(const Vector2 &rhs);

		inline Vector2 &operator -=(const Vector2 &rhs);

		/** Does scalar multiplication, multiplies every vector component by scalar.
		@param scalar Every vector component is multiplied by scalar. 
		@return The returned vector is scaled by scalar. */
		inline Vector2 operator *(Real scalar) const;

		/** Does scalar multiplication with 1 / scalar, so it multiplies every vector component by (1 / scalar).
		@param scalar Every vector component is divided by scalar.  
		@return The returned vector is scaled by (1 / scalar). */
		inline Vector2 operator /(Real scalar) const;

		inline Vector2 operator +(const Vector2 &rhs) const;

		inline Vector2 operator -(const Vector2 &rhs) const;

		inline Vector2 operator *(const Matrix2x2 &rhs) const;

		inline Vector2 operator *(const Matrix3x3 &rhs) const;

		inline Vector2 operator *(const Matrix4x4 &rhs) const;
		
		inline bool areOrthogonal(const Vector2 &rhs) const;

		/** Computes the Euclidean distance between two points in space.
		@param v The Euclidean distance between this vector and v is computed and returned.
		@return The Euclidean distance between this vector and v is computed and returned. */
		inline Real computeDistanceTo(const Vector2 &v) const;

		/** Computes the distance between a point and a line segment.
		@param t It is set to the r value of the straight line equation position(this) = lineSegmentP0 + r * (lineSegmentP1 - lineSegmentP0) or REAL_MAX if line segment length = 0.
		@param lineSegmentP0 Set this to the first (or second) end of the line segment which shall be tested.
		@param lineSegmentP1 Set this to the other (second (or first)) end of the line segment which shall be tested.
		@return The returned value is the distance between the point represented by this object and the straight line segment (lineSegmentP0, lineSegmentP1).*/
		Real computeDistanceTo(Real &t, const Vector2 &lineSegmentP0, const Vector2 &lineSegmentP1) const;

		/** Computes lhs.x * rhs.y - lhs.y * rhs.x = determinant(lhs, rhs).
		@param rhs The cross product lhs x rhs = determinant(lhs, rhs) is computed whereas this is lhs.
		@return Returns lhs.x * rhs.y - lhs.y * rhs.x = determinant(lhs, rhs). */
		inline Real crossProduct(const Vector2 &rhs) const;

		/** Computes lhs.x * rhs.y - lhs.y * rhs.x = determinant(lhs, rhs) = lhs.crossProduct(rhs).
		@param rhs The cross product lhs x rhs = determinant(lhs, rhs) is computed whereas this is lhs.
		@return Returns lhs.x * rhs.y - lhs.y * rhs.x = determinant(lhs, rhs). */
		inline Real determinant(const Vector2 &rhs) const;

		/** Computes and returns the commutative inner product between this = lhs and rhs.
		@param Computes and returns the commutative inner product between this = lhs and rhs.
		@return Returns the sum of the componentwise products. */
		inline Real dotProduct(const Vector2 &rhs) const;
		
		/** Computes a pseudo dot product with absolute values for products.\n
			pseudoDotProduct(v1, v0) = |v0.x * v1.x| + |v0.y * v1.y|
			This dot product is the maximum if you can freely choose the signs of the vector components.
			E.g., useful for efficient separating axes theorem (SAT) collision tests.
		@param rhs Computes the pseudo / absolute dot product between this vector and rhs. 
		@return Returns PDP = pseudo dot product = sum over i \in {x, y} : |this.i * rhs.i| */
		inline Real dotProductAbsolute(const Vector2 &rhs) const;

		/** Checks whether this vector is equal to the other one.
		@param rhs Check is done between this and rhs: this vector == rhs (with some tolerance)?
		@param componentTolerance Two vector component are considered to be equal if their absolute difference is smaller than or equal to componentTolerance.
		@return Returns true if all four vector components are equal. (with tolerance componentTolerance) */
		inline bool equals(const Vector2 &rhs, Real componentTolerance = EPSILON) const;
		
		Real getAngle(const Vector2 &rhs) const;

		Real getAngleWithNormalizedVectors(const Vector2 &rhs) const;

		/** Returns the length of this vector: square root of x * x + y * y.
		@return Returns the length of this vector: square root of x * x + y * y. */
		inline Real getLength() const;

		/** Returns the squared length of this vector: x * x + y * y.
			Returns the squared length of this vector: x * x + y * y.*/
		inline Real getLengthSquared() const;
		
		/** Returns true if any of the vector components represents infinity. 
		@return Returns true if any of the vector components represents infinity.
		@see Math::isInfinity */
		inline bool hasInfiniteComponent() const;

		/** Returns true if any of the vector components is not a number. 
		@return Returns true if any of the vector components is not a number.
		@see Math::isNaN */
		inline bool hasNaNComponent() const;

		/** Returns true if this vector has a length of 1 (except some epsilon error). 
		@return Returns true if this vector is longer or shorter than 1 (except some epsilon error). */
		inline bool hasUnitLength() const;

		/** Computes the inverse for each component so that for each dimension newComponent * oldComponent = 1.
			A component is set to REAL_MAX if it is approximately 0 at the beginning.*/
		inline void invert();
		
		/** Linearly interpolates between a and b with the interpolation coefficients factors.
		@param a This is the first vector used for interpolation which is returned if factors = { 0, 0 }.
		@param b This is the second vector used for interpolation which is returned if factors = { 1, 1 }.
		@param factors Each component is the interpolation coefficient for a single axis. ([0] -> x-axis, ...)
			(0 -> completely a's component is returned, 0.5 -> average of both, 1.0 -> b's component is returned)*/
		inline static Vector2 lerp(const Vector2 &a, const Vector2 &b, const Real factors[2]);

		/** This is a convenience function to get the maximum for each component.
		@param rhs The components of rhs are compared to their corresponding counterpart of this.
		@return The returned vector contains the maximum component of both vectors for each dimension. */
		inline Vector2 maximum(const Vector2 &rhs) const;

		/** This is a convenience function to get the minimum for each component.
		@param rhs The components of rhs are compared to their corresponding counterpart of this.
		@return The returned vector contains the minimum component of both vectors for each dimension. */
		inline Vector2 minimum(const Vector2 &rhs) const;

		/** Returns a vector the components of which are the products of the corresponding components of both vectors.
		@param rhs Each component of rhs is multiplied with its corrensponding component of this and written to the result component.
		@return The components of the returned vector are set to rhs.component * this.component. */
		inline Vector2 multiply(const Vector2 &rhs) const;

		/** Keeps the direction of the vector but changes its magnitude to 1. 
		@param safetyThreshold Defines what minimum length of the original vector is required to consider the normalization as safe.
		@return Returns false if the normalization result is unsafe due to the small magnitude of the original vector or true if you can use the result without being worried. */
		bool normalize(const Real safetyThreshold = Math::EPSILON);

		/** Rotate this vector counter clock wise. (right handed coordinate system with a y-axis going up and an x-axis going to the right)
		@param angle Specifies how far the vector is rotated. (unit: radians)*/
		void rotate(Real angle);

		/** Rotate this vector counter clock wise by means of precomputed sine and cosine values.
		(right handed coordinate system with a y-axis going up and an x-axis going to the right)
		@param cosAngle Contains cosr(angle) whereas angle defines how far the vector is rotated.
		@param sinAngle Contains sinr(angle) whereas angle defines how far the vector is rotated. */
		inline void rotate(Real cosAngle, Real sinAngle);

		/** Scales the vector by the specified value. (x *= value, y *= value)
		@param value The vector is multiplied by this value. */
		inline void scale(Real value);

		/** Overwrites all vector components.
		@param x This vectors x-component is set to x.
		@param y This vectors y-component is set to y.*/
		inline void set(Real x, Real y);
		
	public:
		Real x;	/// This is the first component of this 2D vector.
		Real y;	/// This is the second component of this 2D vector.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline Vector2::Vector2() : x(0), y(0)
	{
		
	}

	inline Vector2::Vector2(const Vector2 &rhs) : x(rhs.x), y(rhs.y)
	{
		
	}

	inline Vector2::Vector2(Real x, Real y) : x(x), y(y)
	{
		
	}

	inline Real &Vector2::operator [](Axis n)
	{
		switch (n)
		{
			case AXIS_X:
				return x;

			case AXIS_Y:
				return y;

			default:
				assert(false);
				return x;
		}
	}

	inline Real Vector2::operator [](Axis n) const
	{
		switch (n)
		{
			case AXIS_X:
				return x;

			case AXIS_Y:
				return y;

			default:
				assert(false);
				return x;
		}
	}

	inline Vector2 &Vector2::operator =(const Vector2 &rhs)
	{
		x = rhs.x;
		y = rhs.y;

		return *this;
	}

	inline bool Vector2::operator ==(const Vector2 &rhs) const
	{
		return this->equals(rhs);
	}

	inline bool Vector2::operator !=(const Vector2 &rhs) const
	{
		return !(*this == rhs);
	}

	inline Vector2 Vector2::operator -() const
	{
		return Vector2(-x, -y);
	}

	inline Vector2 &Vector2::operator *=(Real scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	inline Vector2 &Vector2::operator /=(Real scalar)
	{
		Real one_scalar = 1 / scalar;
		x *= one_scalar;
		y *= one_scalar;

		return *this;
	}

	inline Vector2 &Vector2::operator +=(const Vector2 &rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	inline Vector2 &Vector2::operator -=(const Vector2 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	inline Vector2 Vector2::operator *(Real scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}

	inline Vector2 Vector2::operator /(Real scalar) const
	{
		Real one_scalar = 1 / scalar;
		return Vector2(x * one_scalar, y * one_scalar);
	}

	inline Vector2 Vector2::operator +(const Vector2 &rhs) const
	{
		return Vector2(x + rhs.x, y + rhs.y);
	}

	inline Vector2 Vector2::operator -(const Vector2 &rhs) const
	{
		return Vector2(x - rhs.x, y - rhs.y);
	}

	inline Vector2 Vector2::operator *(const Matrix2x2 &rhs) const
	{
		return Vector2(	x * rhs.m00 + y * rhs.m10,
						x * rhs.m01 + y * rhs.m11);
	}

	inline Vector2 Vector2::operator *(const Matrix3x3 &rhs) const
	{
		return Vector2(	x * rhs.m00 + y * rhs.m10,
						x * rhs.m01 + y * rhs.m11);
	}

	inline Vector2 Vector2::operator *(const Matrix4x4 &rhs) const
	{
		return Vector2(	x * rhs.m00 + y * rhs.m10,
						x * rhs.m01 + y * rhs.m11);
	}
		
	inline bool Vector2::areOrthogonal(const Vector2 &rhs) const
	{
		return fabsr(dotProduct(rhs)) < EPSILON;
	}

	inline Real Vector2::computeDistanceTo(const Vector2 &v) const
	{
		Real dX = x - v.x;
		Real dY = y - v.y;
		return sqrtr(dX * dX + dY * dY);
	}

	inline Real Vector2::crossProduct(const Vector2 &rhs) const
	{
		return x * rhs.y - y * rhs.x;
	}

	inline Real Vector2::determinant(const Vector2 &rhs) const
	{
		return crossProduct(rhs);
	}

	inline Real Vector2::dotProduct(const Vector2 &rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}

	inline Real Vector2::dotProductAbsolute(const Vector2 &rhs) const
	{
		return fabsr(x * rhs.x) + fabsr(y * rhs.y);
	}

	inline bool Vector2::equals(const Vector2 &rhs, Real componentTolerance) const
	{
		return	(fabsr(x - rhs.x) <= componentTolerance) &&
				(fabsr(y - rhs.y) <= componentTolerance);
	}

	inline Real Vector2::getLength() const
	{
		return sqrtr(x * x + y * y);
	}

	inline Real Vector2::getLengthSquared() const
	{
		return x * x + y * y;
	}

	inline bool Vector2::hasInfiniteComponent() const
	{
		return isInfinity(x) || isInfinity(y);
	}

	inline bool Vector2::hasNaNComponent() const
	{
		return isNaN(x) || isNaN(y);
	}
	
	inline bool Vector2::hasUnitLength() const
	{
		const Real length = getLength();
		const Real error = fabsr(1.0f - length);
		return (EPSILON > error);
	}

	inline void Vector2::invert()
	{
		x = (fabsr(x) > EPSILON ? 1.0f / x : REAL_MAX);
		y = (fabsr(y) > EPSILON ? 1.0f / y : REAL_MAX);
	}

	inline Vector2 Vector2::lerp(const Vector2 &a, const Vector2 &b, const Real factors[2])
	{
		return Vector2(	(1.0f - factors[0]) * a.x + factors[0] * b.x,
						(1.0f - factors[1]) * a.y + factors[1] * b.y);
	}

	inline Vector2 Vector2::maximum(const Vector2 &rhs) const
	{
		return Vector2((x >= rhs.x) ? x : rhs.x, (y >= rhs.y) ? y : rhs.y);
	}

	inline Vector2 Vector2::minimum(const Vector2 &rhs) const
	{
		return Vector2((x <= rhs.x) ? x : rhs.x, (y <= rhs.y) ? y : rhs.y);
	}

	inline Vector2 Vector2::multiply(const Vector2 &rhs) const
	{
		return Vector2(x * rhs.x, y * rhs.y);
	}

	inline void Vector2::rotate(Real cosAngle, Real sinAngle)
	{
		Real	tx = cosAngle * x - sinAngle * y;
				y  = sinAngle * x + cosAngle * y;
				x  = tx;
	}

	inline void Vector2::scale(Real value)
	{
		x *= value;
		y *= value;
	}

	inline void Vector2::set(Real x, Real y)
	{
		this->x = x;
		this->y = y;
	}
}

std::ostream &operator<<(std::ostream &os, const Math::Vector2 &rhs);

#endif // _VECTOR_2_H_

