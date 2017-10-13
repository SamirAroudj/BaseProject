/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _VECTOR_3_H_
#define _VECTOR_3_H_

#ifdef _LINUX
#include <cstdlib>
#endif // _LINUX

#include <cassert>
#include <iostream>
#include "Math/MathCore.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"

namespace Math
{
	class Vector3
	{
	public:
		inline Vector3();

		inline Vector3(const Vector3 &rhs);

		inline Vector3(Real x, Real y, Real z);
		
		inline bool areOrthogonal(const Vector3 &rhs) const;

		/** Creates an orthonormal basis starting from this vector.
			The basis consists of three vectors with unit length that are perpendicular to each other. (3 coordinate system axes).
			For example, you can use this to create a local coordinate system for an object surface based on a normal vector (= this vector).
			This vector is going to be the first vector (b0) of the returned basis.
			The other two vectors (b1, b2) are chosen to be perpendicular to it and each other.
			The rotation of b1, b2 around b0 is arbitrary. (It actually depends on the coordinate system of this vector.)
		@param b0 This is set to a unit length vector with the same direction as this vector . b0, b1 and b2 are the orthonormal basis vectors.
		@param b1 This is set to a unit length vector orthogonal to this vector, b0 and b2. b0, b1 and b2 are the orthonormal basis vectors.
		@param b2 This is set to a unit length vector orthogonal to this vector, b0 and b1. b0, b1 and b2 are the orthonormal basis vectors.
		@param normalized Set this to true if v is a vector of unit length. */
		void createBasis(Vector3 &b0, Vector3 &b1, Vector3 &b2, bool normalized = false) const;

		/** Creates an orthonormal basis starting from this vector which is assumed to have unit length otherwise this call does not make sense.
			The basis consists of three vectors with unit length (assuming this vector has unit length).
			They are perpendicular to each other.
			(3 coordinate system axes).
			For example, you can use this to create a local coordinate system for an object surface based on a normal vector (= this vector).
			This vector is going to be the first vector (b0) of the returned basis.
			The other two vectors (b1, b2) are chosen to be perpendicular to it and each other.
			The rotation of b1 and b2 around this vector is arbitrary. (It actually depends on the coordinate system of this vector.)
		@param b1 This is set to a unit length vector orthogonal to this vector, b0 and b2. b0, b1 and b2 are the orthonormal basis vectors.
		@param b2 This is set to a unit length vector orthogonal to this vector, b0 and b1. b0, b1 and b2 are the orthonormal basis vectors.
		@param normalized Set this to true if v is a vector of unit length. */
		inline void createBasis(Vector3 &b1, Vector3 &b2) const;

		inline Vector3 crossProduct(const Vector3 &rhs) const;

		inline Real dotProduct(const Vector3 &rhs) const;
		
		/** Computes a pseudo dot product with absolute values for products.\n
			pseudoDotProduct(v1, v0) = |v0.x * v1.x| + |v0.y * v1.y| + |v0.z * v1.z|
			This dot product is the maximum if you can freely choose the signs of the vector components.
			E.g., useful for efficient separating axes theorem (SAT) collision tests.
		@param rhs Computes the pseudo / absolute dot product between this vector and rhs. 
		@return Returns PDP = pseudo dot product = sum over i \in {x, y, z} : |this.i * rhs.i| */
		inline Real dotProductAbsolute(const Vector3 &rhs) const;

		Real getAngle(const Vector3 &rhs) const;

		Real getAngleWithNormalizedVectors(const Vector3 &rhs) const;

		inline Real *getData();
		inline const Real *getData() const;

		inline Real getLength() const;

		inline Real getLengthSquared() const;

		/** Computes and returns some vector v which is orthogonal/perpendicular to this vector. (this->dotProduct(returned vector) = 0)
		@param normalize Set this to true if you want to get an orthogonal vector which has a length of 1.0f.
		@return Computes and returns some vector v which is orthogonal/perpendicular to this vector. (this->dotProduct(returned vector) = 0)*/
		Vector3 getOrthogonalVector(bool normalize = false) const;

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
		inline Vector3 maximum(const Vector3 &rhs) const;

		/** This is a convenience function to get the minimum for each component.
		@param rhs The components of rhs are compared to their corresponding counterpart of this.
		@return The returned vector contains the minimum component of both vectors for each dimension. */
		inline Vector3 minimum(const Vector3 &rhs) const;

		/** Returns a vector the components of which are the products of the corresponding components of both vectors.
		@param rhs Each component of rhs is multiplied with its corrensponding component of this and written to the result component.
		@return The components of the returned vector are set to rhs.component * this.component. */
		inline Vector3 multiply(const Vector3 &rhs) const;
		
		/** Keeps the direction of the vector but changes its magnitude to 1. 
		@param safetyThreshold Defines what minimum length of the original vector is required to consider the normalization as safe.
		@return Returns false if the normalization result is unsafe due to the small magnitude of the original vector or true if you can use the result without being worried. */
		bool normalize(const Real safetyThreshold = Math::EPSILON);
		
		inline Real &operator [](Axis n);

		inline Real operator [](Axis n) const;

		inline Vector3 &operator =(const Vector3 &rhs);

		/** Checks whether this vector is equal to the other one.
		@param rhs Check is done between this and rhs: this vector == rhs (with some tolerance)?
		@return Returns true if all four vector components are equal. (with tolerance componentTolerance) */
		inline bool operator ==(const Vector3 &rhs) const;

		/** Checks whether this vector is equal to the other one.
		@param rhs Check is done between this and rhs: this vector == rhs (with some tolerance)?
		@param epsilon Two vector component are considered to be equal if their absolute difference is smaller than or equal to componentTolerance.
		@return Returns true if all four vector components are equal. (with tolerance componentTolerance) */
		inline bool equals(const Vector3 &rhs, const Real epsilon = EPSILON) const;

		inline bool operator !=(const Vector3 &rhs) const;

		inline Vector3 operator -() const;

		/** Does scalar multiplication, multiplies every vector component by scalar.
		@param scalar Every vector component is multiplied by scalar.
		@return Returns this vector after componentwise multiplication by scalar. */
		inline Vector3 &operator *=(Real scalar);

		/** Does scalar multiplication with 1 / scalar, so it multiplies every vector component by (1 / scalar).
		@param scalar Every vector component is divided by scalar.
		@return Returns this vector after componentwise multiplication by (1 / scalar). */
		inline Vector3 &operator /=(Real scalar);

		inline Vector3 &operator +=(const Vector3 &rhs);

		inline Vector3 &operator -=(const Vector3 &rhs);

		/** Does scalar multiplication, multiplies every vector component by scalar.
		@param scalar Every vector component is multiplied by scalar. 
		@return The returned vector is scaled by scalar. */
		inline Vector3 operator *(Real scalar) const;

		/** Does scalar multiplication with 1 / scalar, so it multiplies every vector component by (1 / scalar).
		@param scalar Every vector component is divided by scalar.  
		@return The returned vector is scaled by (1 / scalar). */
		inline Vector3 operator /(Real scalar) const;

		inline Vector3 operator +(const Vector3 &rhs) const;

		inline Vector3 operator -(const Vector3 &rhs) const;

		inline Vector3 operator *(const Matrix3x3 &rhs) const;

		/** Computes this * Matrix3x3(rhs) whereas Matrix3x3(rhs) is the upper left 3x3 block of rhs.
		@param rhs The upper left 3x3 block is applied to this vector.
		@return Returns (this vector) * (upper left 3x3 block(rhs)) as result. */
		inline Vector3 operator *(const Matrix4x4 &rhs) const;

		/** Computes the matrix M = this * rhs^t which is created by the outer product of this vector and rhs.
			Its components are mij = this[i] * rhs[j]
		@param rhs The outer product is computed between this and rhs: M = this * rhs^t.
		@return Returns the outer product matrix M = this * rhs^t with the entries mij = this[i] * rhs[j].*/
		inline Matrix3x3 outerProduct(const Vector3 &rhs) const;

		/** Overwrites all vector components.
		@param x This vectors x-component is set to x.
		@param y This vectors y-component is set to y.
		@param z This vectors z-component is set to z.*/
		inline void set(Real x, Real y, Real z);
		
	public:
		Real x; /// This is the first component of this 3D vector.
		Real y; /// This is the second component of this 3D vector.
		Real z; /// This is the third component of this 3D vector.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
	
	inline Vector3::Vector3() : x(0), y(0), z(0)
	{
	
	}

	inline Vector3::Vector3(const Vector3 &rhs) : x(rhs.x), y(rhs.y), z(rhs.z)
	{
		
	}

	inline Vector3::Vector3(Real x, Real y, Real z) : x(x), y(y), z(z)
	{

	}

	inline bool Vector3::areOrthogonal(const Vector3 &rhs) const
	{
		return fabsr(this->dotProduct(rhs)) < EPSILON;
	}

	inline void Vector3::createBasis(Vector3 &b1, Vector3 &b2) const
	{
		b1 = getOrthogonalVector(true);
		b2 = this->crossProduct(b1);
	}

	inline Vector3 Vector3::crossProduct(const Vector3 &rhs) const
	{
		return Vector3(
			y * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x);
	}

	inline Real Vector3::dotProduct(const Vector3 &rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	inline Real Vector3::dotProductAbsolute(const Vector3 &rhs) const
	{
		return fabsr(x * rhs.x) + fabsr(y * rhs.y) + fabsr(z * rhs.z);
	}

	inline Real *Vector3::getData()
	{
		return reinterpret_cast<Real *>(this);
	}

	inline const Real *Vector3::getData() const
	{
		return reinterpret_cast<Real const *>(this);
	}

	inline Real Vector3::getLengthSquared() const
	{
		return x * x + y * y + z * z;
	}

	inline Real Vector3::getLength() const
	{
		return sqrtr(x * x + y * y + z * z);
	}

	inline bool Vector3::hasInfiniteComponent() const
	{
		return isInfinity(x) || isInfinity(y) || isInfinity(z);
	}

	inline bool Vector3::hasNaNComponent() const
	{
		return isNaN(x) || isNaN(y) || isNaN(z);
	}
	
	inline bool Vector3::hasUnitLength() const
	{
		const Real length = getLength();
		const Real error = fabsr(1.0f - length);
		return (EPSILON > error);
	}

	inline void Vector3::invert()
	{
		x = (fabsr(x) > EPSILON ? 1.0f / x : REAL_MAX);
		y = (fabsr(y) > EPSILON ? 1.0f / y : REAL_MAX);
		z = (fabsr(z) > EPSILON ? 1.0f / z : REAL_MAX);
	}
		
	inline Vector3 Vector3::maximum(const Vector3 &rhs) const
	{
		return Vector3((x >= rhs.x) ? x : rhs.x,
						(y >= rhs.y) ? y : rhs.y,
						(z >= rhs.z) ? z : rhs.z);
	}

	inline Vector3 Vector3::minimum(const Vector3 &rhs) const
	{
		return Vector3((x <= rhs.x) ? x : rhs.x,
						(y <= rhs.y) ? y : rhs.y,
						(z <= rhs.z) ? z : rhs.z);
	}

	inline Vector3 Vector3::multiply(const Vector3 &rhs) const
	{
		return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	inline Real &Vector3::operator [](Axis n)
	{
		switch (n)
		{
			case AXIS_X: return x;
			case AXIS_Y: return y;
			case AXIS_Z: return z;
			default: assert(false); return x;
		}
	}

	inline Real Vector3::operator [](Axis n) const
	{
		switch (n)
		{
			case AXIS_X: return x;
			case AXIS_Y: return y;
			case AXIS_Z: return z;
			default: assert(false); return x;
		}
	}

	inline Vector3 &Vector3::operator =(const Vector3 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;

		return *this;
	}

	inline bool Vector3::operator ==(const Vector3 &rhs) const
	{
		return this->equals(rhs);
	}

	inline bool Vector3::equals(const Vector3 &rhs, const Real epsilon) const
	{
		return	(fabsr(x - rhs.x) < epsilon) &&
				(fabsr(y - rhs.y) < epsilon) &&
				(fabsr(z - rhs.z) < epsilon);
	}

	inline bool Vector3::operator !=(const Vector3 &rhs) const
	{
		return !(*this == rhs);
	}

	inline Vector3 Vector3::operator -() const
	{
		return Vector3(-x, -y, -z);
	}

	inline Vector3 &Vector3::operator *=(Real scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	inline Vector3 &Vector3::operator /=(Real scalar)
	{
		Real one_scalar = 1 / scalar;
		x *= one_scalar;
		y *= one_scalar;
		z *= one_scalar;

		return *this;
	}

	inline Vector3 &Vector3::operator +=(const Vector3 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}

	inline Vector3 &Vector3::operator -=(const Vector3 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}

	inline Vector3 Vector3::operator *(Real scalar) const
	{
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	inline Vector3 Vector3::operator /(Real scalar) const
	{
		Real one_scalar = 1 / scalar;
		return Vector3(x * one_scalar, y * one_scalar, z * one_scalar);
	}

	inline Vector3 Vector3::operator +(const Vector3 &rhs) const
	{
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	inline Vector3 Vector3::operator -(const Vector3 &rhs) const
	{
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	inline Vector3 Vector3::operator *(const Matrix3x3 &rhs) const
	{
		return Vector3(	x * rhs.m00 + y * rhs.m10 + z * rhs.m20,
						x * rhs.m01 + y * rhs.m11 + z * rhs.m21,
						x * rhs.m02 + y * rhs.m12 + z * rhs.m22);
	}

	inline Vector3 Vector3::operator *(const Matrix4x4 &rhs) const
	{
		return Vector3(	x * rhs.m00 + y * rhs.m10 + z * rhs.m20,
						x * rhs.m01 + y * rhs.m11 + z * rhs.m21,
						x * rhs.m02 + y * rhs.m12 + z * rhs.m22);
	}

	inline Matrix3x3 Vector3::outerProduct(const Vector3 &rhs) const
	{
		return Matrix3x3(x * rhs.x, x * rhs.y, x * rhs.z,
							y * rhs.x, y * rhs.y, y * rhs.z, 
							z * rhs.x, z * rhs.y, z * rhs.z);
	}

	inline void Vector3::set(Real x, Real y, Real z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
}

std::ostream& operator <<(std::ostream &os, const Math::Vector3 &rhs);

#endif // _VECTOR_3_H_

