/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _MATRIX_4_X_4_H_
#define _MATRIX_4_X_4_H_

#ifdef _LINUX
#include <cstring>
#endif // _LINUX

#include <cassert>
#include <iostream>
#include "Math/MathCore.h"
#include "Platform/DataTypes.h"

namespace Math
{
	class Vector2;
	class Vector3;
	class Vector4;

	class Matrix4x4
	{
	public:
		/** Sets the matrix to the identity transformation:\n
		1 0 0 0 \n
		0 1 0 0 \n
		0 0 1 0 \n
		0 0 0 1 \n */
		inline Matrix4x4();

		/** Copy constructor.
		@param rhs The new matrix will exactly look like rhs. */
		Matrix4x4(const Matrix4x4 &rhs);

		//Matrix4x4(const Real *values); too dangerous since you forget to think about weather the values are in row or column major order
		Matrix4x4(	Real v11, Real v12, Real v13, Real v14,
					Real v21, Real v22, Real v23, Real v24,
					Real v31, Real v32, Real v33, Real v34,
					Real v41, Real v42, Real v43, Real v44);

		static Matrix4x4 createFromMajorRowArray(const Real * data);
		static Matrix4x4 createFromMajorColumnArray(const Real * data);
		static Matrix4x4 createInverse(const Matrix4x4 &matrix);
		static Matrix4x4 createInverseCameraTransformation(const Vector3 &cx, const Vector3 &cy, const Vector3 &cz, const Vector3 &position);

		/** DirectX convention, function maps z to [0, 1]. */
		static Matrix4x4 createOrthoLHDirectX(const Real width, const Real height, const Real zNear, const Real zFar);

		/** DirectX convention, function maps z to [0, 1]. */
		static Matrix4x4 createOrthoRHDirectX(const Real width, const Real height, const Real zNear, const Real zFar);

		/** DirectX convention, function maps z to [0, 1]. */
		static Matrix4x4 createProjectionLHDirectX(const Real width, const Real height, const Real zNear, const Real zFar);

		/** DirectX convention, function maps z to [0, 1]. */
		static Matrix4x4 createProjectionRHDirectX(const Real width, const Real height, const Real zNear, const Real zFar);

		/** DirectX convention, function maps [n, f] to [0, 1].
		@param aspectRatio Set this to the ratio of the image plane width to its height. Must be positive.*/
		static Matrix4x4 createProjectionFovLHDirectX(const Real fovY, const Real aspectRatio, const Real zNear, const Real zFar);

		/** DirectX convention, function maps [-n, -f] to [0, 1].
		@param aspectRatio Set this to the ratio of the image plane width to its height. Must be positive.*/
		static Matrix4x4 createProjectionFovRHDirectX(const Real fovY, const Real aspectRatio, const Real zNear, const Real zFar);

		/** OpenGL convention, function maps [-n, -f] to [-1, 1].
		@param aspectRatio Set this to the ratio of the image plane width to its height. Must be positive.*/
		static Matrix4x4 createProjectionFovRHOpenGL(const Real fovY, const Real aspectRatio, const Real zNear, const Real zFar);

		/** Computes a 4x4 projection matrix for a real world camera (pinhole model)
			which maps view space camera coordinates to homogenous (no z-division) device coordinates (non normalized device coordinates).
			If you also apply the z-division and transformation to your wanted viewport then you get pixel coordinates, e.g., for an undistorted image.
		@param focalLength Set this to the focalLength with which the target image was captured.
		@param aspectRatio Set this to the ratio of the image width to image height. Must be positive. */
		static Matrix4x4 createProjectionRealWorld(const Real focalLength, const Real aspectRatio);

		static Matrix4x4 createRotationX(const Real radian);
		static Matrix4x4 createRotationY(const Real radian);
		static Matrix4x4 createRotationZ(const Real radian);
		static Matrix4x4 createTranslation(const Vector3 &translation);

		/** Compares two matrices to check for equality.
		@param rhs The left-hand side matrix is compared to rhs.
		@param epsilon Set this to define the threshold until which two floating point numbers are considered to be equal.
		@returns The operator returns true if all entries of this matrix are approximately equal to corresponding the elements of rhs.  (see Math::EPSILON)*/
		bool equals(const Matrix4x4 &rhs, const Real epsilon = EPSILON) const;

		inline void addTranslation(const Vector3 &translation);
		inline void addTranslation(Real x, Real y, Real z);

		/** Computes the Frobenius inner product of the two matrices which is analogous to the inner product of two vectors. (= sum of pairwise component products)
		@param rhs Computes the Frobenius inner product of rhs and the left hand side matrix. The Frobenius inner product is commutative.
		@return The Frobenius inner product (~= skalar product of both matrices) is returned. */
		Real frobeniusProduct(const Matrix4x4 &rhs) const;

		inline const Real * getData() const;

		Real getDeterminant() const;

		/** Returns a specific row vector of this matrix. For example, 0th row vector = (x = m00, y = m01, z = m02, w = m03) for rowVectorIdx = 0.
		@param target Is filled with the specified matrix row entries. For example, row vector 2 (x = m20, y = m21, z = m22, w = m23) for rowVectorIdx = 2.
		@param rowVectorIdx Specifies which row vector is returned. For this Matrix4x4 object only values in { 0, 1, 2, 3 } make sense. */
		void getRowVector(Vector4 &target, uint32 rowVectorIdx) const;

		/** Returns the trace of the matrix. The trace of a matrix is the sum of its diagonal elements.
		@return Returns the trace of the matrix. The trace of a matrix is the sum of its diagonal elements.*/
		inline Real getTrace() const;

		/** Sets the values of this matrix to exactly the values of rhs.
		@param rhs The left hand side matrix elements are set to the elements of the right hand set matrix called rhs.
		@return Returns the reference to this matrix after it was set to rhs. */
		Matrix4x4 &operator =(const Matrix4x4 &rhs);

		/** Compares two matrices to check for equality.
		@param rhs The left-hand side matrix is compared to rhs.
		@returns The operator returns true if all entries of this matrix are approximately equal to corresponding the elements of rhs.  (see Math::EPSILON)*/
		inline bool operator ==(const Matrix4x4 &rhs) const;
		
		inline bool operator !=(const Matrix4x4 &rhs) const;
		
		/** Adds two matrices and returns the result. The addition is done componentwise: result(i, j) = this(i, j) + rhs(i, j)
		@param rhs The result is rhs + this.
		@returns The returned matrix is the result of the componentwise addition.*/
		Matrix4x4 operator +(const Matrix4x4 &rhs) const;

		/** Adds matrix rhs to this matrix componentwise and returns the new matrix. The addition is done componentwise: this(i, j) += + rhs(i, j).
		@param rhs This matrix is changed to rhs + this and returned.
		@returns The returned matrix is the result of the componentwise addition.*/
		Matrix4x4 &operator +=(const Matrix4x4 &rhs);

		/** Substracts two matrices and returns the result. The substraction is carried for each element, e.g. result(i, j) = this(i, j) - rhs(i, j)
		@param rhs The result is this - rhs.
		@returns The matrix returned is the componentwise difference: this - rhs.*/
		Matrix4x4 operator -(const Matrix4x4 &rhs) const;

		Matrix4x4 operator *(Real scalar) const;
		Matrix4x4 operator /(Real scalar) const;

		Matrix4x4 operator *(const Matrix4x4 &rhs) const;
		
		inline Real &operator()(unsigned short int row, unsigned short int column);

		inline Real operator()(unsigned short int row, unsigned short int column) const;

		inline void setColumn(const Math::Vector4 &newColumn, const uint32 columnIdx);
		inline void setColumn(const Real &v0, const Real &v1, const Real &v2, const Real &v3, const uint32 columnIdx);
		
		inline void setDiagonal(const Math::Vector4 &newDiagonal);
		inline void setDiagonal(const Real &v0, const Real &v1, const Real &v2, const Real &v3);

		inline void setRow(const Math::Vector4 &newRow, const uint32 rowIdx);
		inline void setRow(const Real &v0, const Real &v1, const Real &v2, const Real &v3, const uint32 rowIdx);

		/** Sets every diagonal entry to one and all others to zero. */
		inline void setToIdentity();

		/** Sets every matrix component to zero. */
		inline void setToZero();

		inline void setTranslation(const Math::Vector3 &translation);
		inline void setTranslation(const Real &x, const Real &y, const Real &z);

		/** Swaps rows and columns, every row becomes a column with the same number.
			(= Every column becomes a row, for i != j swaps mij and mji.) */
		void transpose();
			
	public:
		union
		{
			Real values[4][4];
			struct
			{
				Real m00, m01, m02, m03;
				Real m10, m11, m12, m13;
				Real m20, m21, m22, m23;
				Real m30, m31, m32, m33;
			};
		};
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	inline Matrix4x4::Matrix4x4()
	{
		setToIdentity();
	}

	inline void Matrix4x4::addTranslation(Real x, Real y, Real z)
	{
		m30 += x;
		m31 += y;
		m32 += z;
	}

	inline const Real *Matrix4x4::getData() const
	{
		return *values;
	}

	inline Real Matrix4x4::getTrace() const
	{
		return m00 + m11 + m22 + m33;
	}

	inline bool Matrix4x4::operator !=(const Matrix4x4 &rhs) const
	{
		return !(*this == rhs);
	}
	
	inline bool Matrix4x4::operator ==(const Matrix4x4 &rhs) const
	{
		return this->equals(rhs);
	}
		
	inline Real &Matrix4x4::operator()(unsigned short int row, unsigned short int column)
	{
		assert(row < 4 && column < 4);
		return values[row][column];
	}

	inline Real Matrix4x4::operator()(unsigned short int row, unsigned short int column) const	
	{
		assert(row < 4 && column < 4);
		return values[row][column];
	}

	inline void Matrix4x4::setColumn(const Real &v0, const Real &v1, const Real &v2, const Real &v3, const uint32 columnIdx)
	{
		assert(columnIdx <= 3);
		values[0][columnIdx] = v0;
		values[1][columnIdx] = v1;
		values[2][columnIdx] = v2;
		values[3][columnIdx] = v3;
	}

	inline void Matrix4x4::setDiagonal(const Real &v0, const Real &v1, const Real &v2, const Real &v3)
	{
		m00 = v0;
		m11 = v1;
		m22 = v2;
		m33 = v3;
	}

	inline void Matrix4x4::setRow(const Real &v0, const Real &v1, const Real &v2, const Real &v3, const uint32 rowIdx)
	{
		assert(rowIdx <= 3);
		values[rowIdx][0] = v0;
		values[rowIdx][1] = v1;
		values[rowIdx][2] = v2;
		values[rowIdx][3] = v3;
	}
	
	void Matrix4x4::setToIdentity()
	{
		m01 = m02 = m03 = 0.0f;
		m10 = m12 = m13 = 0.0f;
		m20 = m21 = m23 = 0.0f;
		m30 = m31 = m32 = 0.0f;
		m00 = m11 = m22 = m33 = 1.0f;
	}

	inline void Matrix4x4::setToZero()
	{ 
		m00 = m01 = m02 = m03 = 0.0f;
		m10 = m11 = m12 = m13 = 0.0f;
		m20 = m21 = m22 = m23 = 0.0f;
		m30 = m31 = m32 = m33 = 0.0f;
	}

	inline void Matrix4x4::setTranslation(const Real &x, const Real &y, const Real &z)
	{
		m30 = x;
		m31 = y;
		m32 = z;
	}
}

std::ostream &operator <<(std::ostream &os, const Math::Matrix4x4 &m);


#endif // _MATRIX_4_X_4_H_
