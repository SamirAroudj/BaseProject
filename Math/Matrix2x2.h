/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _MATRIX_2_X_2_H_
#define _MATRIX_2_X_2_H_

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

	/// A square matrix with two rows and two columns consistent with DirectX conventions which can store different kinds of transformations.
	/** This is a square matrix with four elements.
	Be aware that this implementation is consistent with DirectX conventions. That is, these matrices are transposed OpenGL (usually, math conventions consistent) matrices.
	For example, if v1 is a row 2D vector and m1 and m2 are two transformations stored in two objects of this class then
	v2 = v1 * m1 * m2 is equal to the vector v1 transformed by m1 first and the result of this operation is in turn transformed by m2.*/
	class Matrix2x2
	{
	public:

		/** Creates a matrix that does not change vectors by multiplication since the diagonal is filed with ones and all other values are 0. */
		Matrix2x2()
		{
			setToIdentity();
		}

		/** That is the copy constructor which creates a new matrix which is equal to the matrix which is used as a parameter for the constructor.
		@param rhs The values of this matrix are equally used in the newly created matrix. */
		Matrix2x2(const Matrix2x2 &rhs);

		//Matrix2x2(const Real *values); too dangerous since you forget to think about weather the values are in row or column major order

		/** Creates a matrix by specifying each value. Note that the first index corresponds to the row and the second value identifies the column of the element.
		@param v11 This is the entry in the first row and first column.
		@param v12 This is the entry in the first row and second column.
		@param v21 This is the entry in the second row and first column.
		@param v22 This is the entry in the second row and second column. */
		Matrix2x2(Real v11, Real v12,
				  Real v21, Real v22);

		/** Creates a matrix from two row vectors according to the Direct X matrix convention. (The rows of the matrix form its coordinate system.)
		@param v1 This becomes the first row of the matrix.
		@param v2 This becomes the second row of the matrix.*/
		Matrix2x2(const Vector2 &v1, const Vector2 &v2);

		/** Creates a matrix from an array of floating point values which are stored as rows one after another.
		This means that the first value in the second row will be set to data[2]. Likewise, the element v12 (first row, second column) is set to data[1].
		@param data This is a matrix consisting of an array of floating point values which are stored as rows one after another.*/
		static Matrix2x2 createFromMajorRowArray(const Real * data);

		/** Creates a matrix from an array of floating point values which are stored as columns one after another.
		This means that the first value in the second row will be set to data[1]. Likewise, the element v12 (first row, second column) is set to data[2].
		@param data This is a matrix consisting of an array of floating point values which are stored as columns one after another.*/
		static Matrix2x2 createFromMajorColumnArray(const Real * data);

		/** Creates the inverse matrix m^-1 of a matrix m so that m * m^-1 = m^-1 * m = E whereas E is the identity matrix.
		@param invertible Is set to false if the matrix cannot be inverted as it is singuler (determinant = 0). This is set to true on success.
			The zero matrix is returned on failed inversion.
		@param matrix	The inverse matrix of this parameter is computed.
		@return The function returns the inverse matrix of the input matrix.*/
		static Matrix2x2 createInverse(bool &invertible, const Matrix2x2 &matrix);

		/** Creates a transformation that revolves a vector around the z-axis of the coordinate system the vector is relative to by a certain angle. (counter clockwise rotation)
		@param radian The vector is rotated by the value of the radian parameter which is a defined in radian measure.
		@returns The returned matrix specifies a transformation that revolves a vector around the z-axis by the angle radian. (counter clockwise rotation)*/
		static Matrix2x2 createRotation(Real radian);

		/** Sets the value of this matrix to the values of rhs.
		@param rhs The values of rhs are copied into the left-hand side matrix.
		@returns The matrix the values are assigned to is returned. */
		Matrix2x2 &operator =(const Matrix2x2 &rhs);
		//Matrix2x2 &operator =(const Real *values); too dangerous since you forget to think about weather the values are in row or column major order

		/** Compares two matrices to check for equality.
		@param rhs The left-hand side matrix is compared to rhs.
		@returns The operator returns true if all entries of this matrix are approximately equal to corresponding the elements of rhs.  (see Math::EPSILON)*/
		inline bool operator ==(const Matrix2x2 &rhs) const
		{
			return this->equals(rhs);
		}

		/** Compares two matrices to check for equality.
		@param rhs The left-hand side matrix is compared to rhs.
		@param epsilon Set this to define the threshold until which two floating point numbers are considered to be equal.
		@returns The operator returns true if all entries of this matrix are approximately equal to corresponding the elements of rhs.  (see Math::EPSILON)*/
		bool equals(const Matrix2x2 &rhs, const Real epsilon = EPSILON) const;

		/** Compares two matrices to check for disparity.
		@param rhs The left-hand side matrix is compared to rhs.
		@returns The operator returns false if all entries of this matrix are approximately equal to the corresponding elements of rhs.  (see Math::EPSILON)*/
		bool operator !=(const Matrix2x2 &rhs) const
		{
			return !(*this == rhs);
		}

		/** Adds two matrices and returns the result. The addition is done componentwise: result(i, j) = this(i, j) + rhs(i, j)
		@param rhs The result is rhs + this.
		@returns The returned matrix is the result of the componentwise addition.*/
		Matrix2x2 operator +(const Matrix2x2 &rhs) const;

		/** Adds matrix rhs to this matrix componentwise and returns the new matrix. The addition is done componentwise: this(i, j) += + rhs(i, j).
		@param rhs This matrix is changed to rhs + this and returned.
		@returns The returned matrix is the result of the componentwise addition.*/
		Matrix2x2 &operator +=(const Matrix2x2 &rhs);

		/** Substracts two matrices and returns the result. The substraction is carried for each element, e.g. result(i, j) = this(i, j) - rhs(i, j)
		@param rhs The result is this - rhs.
		@returns The matrix returned is the componentwise difference: this - rhs.*/
		Matrix2x2 operator -(const Matrix2x2 &rhs) const;

		/** Multiplies each entry of the matrix by the value of scalar and returns the resulting matrix.
		@param scalar Each value is multiplied by scalar.
		@returns The returned matrix has entries which were multiplied by the value of scalar. */
		Matrix2x2 operator *(Real scalar) const;

		/** Multiplies each matrix entry by scalar so that each entry e[i][j] becomes scalar * e[i][j].
		@param scalar Each value is multiplied by scalar. */
		void operator *=(Real scalar);

		/** Divides each entry of the matrix by the value of scalar and returns the resulting matrix.
		@param scalar Each value is divided by scalar.
		@returns The returned matrix has entries which were divided by the value of scalar. */
		Matrix2x2 operator /(Real scalar) const;

		/** Creates a transformation which is the concatenation of this matrix and rhs whereas the transformation of this matrix is caried out first. (DirectX convention).
			Take care! This operation is not commutative!!!!!!!!!!!!!
		@param rhs The resulting matrix is the transformation of this matrix followed by the transformation of rhs. (DirectX convention)
		@returns The result is the transformation of this matrix followed by the transformation of rhs. (DirectX convention)*/
		Matrix2x2 operator *(const Matrix2x2 &rhs) const;

		/** Accesses an entry of the matrix (first value indicates the row and the second value indicates the column).
		@param row Determines the row of the element which is returned.
		@param column Denomniates the column of the element which is returned. */
		Real &operator()(unsigned short int row, unsigned short int column)
		{
			assert(row < 2 && column < 2);
			return values[row][column];
		}

		/** Accesses an entry of the matrix (first value indicates the row and the second value indicates the column).
		@param row Determines the row of the element which is returned.
		@param column Denomniates the column of the element which is returned. */
		Real operator()(unsigned short int row, unsigned short int column) const
		{
			assert(row < 2 && column < 2);
			return values[row][column];
		}

		/** Computes the Frobenius inner product of the two matrices which is analogous to the inner product of two vectors. (= sum of pairwise component products)
		@param rhs Computes the Frobenius inner product of rhs and the left hand side matrix. The Frobenius inner product is commutative.
		@return The Frobenius inner product (~= skalar product of both matrices) is returned. */
		Real frobeniusProduct(const Matrix2x2 &rhs) const;

		/** Computes and returns the determinant of the matrix.
		@returns The resulted value is the determinant of the matrix. (If it is 0 then the matrix is singular as there exist no inverse matrix. */
		Real getDeterminant() const;

		/** Returns the entries of the matrix which are stored in row major order (one row after another in the array, DirectX convention).
		@returns The returned value points to an array which contains the rows of the matrix (one after another). */
		const Real *getData() const { return *values; }

		/** Computes the two Eigenvalues and the two Eigenvectors of this matrix.
			Caution! The Eigenvectors fit to the DirectX convention like v * M = scalar * v.
		@param eigenvector0 Is set to the Eigenvector corresponding to the returned eigenvalue0.
		@param eigenvector1 Is set to the Eigenvector corresponding to the returned eigenvalue1.
		@param eigenvalue0 Is set to the maximum of both Eigenvalues of this matrix.
		@param eigenvalue1 Is set to the minimum of both Eigenvalues of this matrix.
		@see See getEigenvalues(...) and getEigenvectors(...). */
		void getEigendata(Vector2 &eigenvector0, Vector2 &eigenvector1, Real &eigenvalue0, Real &eigenvalue1) const;

		/** Computes the two Eigenvalues of this matrix.
		@param eigenvalue0 Is set to the maximum of both Eigenvalues of this matrix.
		@param eigenvalue1 Is set to the minimum of both Eigenvalues of this matrix. */
		void getEigenvalues(Real &eigenvalue0, Real &eigenvalue1) const;

		/** Computes the two Eigenvectors of this matrix given its two Eigenvalues.
			Caution! The Eigenvectors fit to the DirectX convention like v * M = scalar * v.
		@param eigenvector0 Is set to the Eigenvector of the provided Eigenvalue eigenvalue0.
		@param eigenvector1 Is set to the Eigenvector of the provided Eigenvalue eigenvalue1.
		@param eigenvalue0 Set this to one Eigenvalue of this matrix.
		@param eigenvalue1 Set this to the remaining Eigenvalue of this matrix.
		@see See getEigenvalues(...). */
		void getEigenvectors(Vector2 &eigenvector0, Vector2 &eigenvector1, const Real eigenvalue0, const Real eigenvalue1) const;

		/** Returns a specific row vector of this matrix. For example, 0th row vector = (m00, m01) for rowVectorIdx = 0.
		@param target Is filled with the specified matrix row entries. For example, row vector 1 (m10, m11) for rowVectorIdx = 2.
		@param rowVectorIdx Specifies which row vector is returned. For this Matrix2x2 object only values in { 0, 1 } make sense. */
		void getRowVector(Vector2 &target, uint32 rowVectorIdx) const;

		/** Returns the trace of the matrix. The trace of a matrix is the sum of its diagonal elements.
		@return Returns the trace of the matrix. The trace of a matrix is the sum of its diagonal elements.*/
		Real getTrace() const { return m00 + m11;  }

		/** Changes the matrix to the identity matrix.
			This means that the diagonal consists of ones and all other entries are set to 0.*/
		void setToIdentity()
		{
			m00 = m11 = 1.0f;
			m01 = m10 = 0.0f;
		}

		/** Sets every matrix component to zero. */
		void setToZero()
		{
			m00 = m01 = 0.0f;
			m10 = m11 = 0.0f;
		}

		/** Swaps rows and columns, every row becomes a column with the same number.
			(= Every column becomes a row, for i != j swaps mij and mji.) */
		void transpose()
		{
			// m00, m11 don't change
			Real	temp = m01;
					m01	 = m10;
					m10	 = temp;
		}

	public:
		/// The four entries of the 2x2 matrix
		union
		{
			/// The nine entries of the 2x2 matrix
			Real values[2][2];
			struct
			{
				Real m00, m01;
				Real m10, m11;
			};
		};

	};

	/** Write the values of a 2x2 matrix to an output stream.
	@param os The matrix information is written to this stream.
	@param m The entries of m are written to the output stream.
	@returns Operator returns the extended output stream. */
	std::ostream &operator <<(std::ostream &os, const Math::Matrix2x2 &m);
}

#endif // _MATRIX_2_X_2_H_

