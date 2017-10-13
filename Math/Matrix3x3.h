/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _MATRIX_3_X_3_H_
#define _MATRIX_3_X_3_H_

#ifdef _LINUX
#include <cstring>
#endif // _LINUX

#include <cassert>
#include <iostream>
#include "Utilities/HelperFunctions.h"

namespace Math
{
	class Vector2;
	class Vector3;

	/// A square matrix with three rows and three columns consistent with DirectX conventions which can store different kinds of transformations.
	/** This is a square matrix with nine elements.
	Be aware that this implementation is consistent with DirectX conventions. That is, these matrices are transposed OpenGL (usual math conventions consistent) matrices.
	For example if v1 is a row 3D vector and m1 and m2 are two transformations stored in two objects of this class then is
	v2 = v1 * m1 * m2 the vector v1 transformed by m1 first and the result of this operation is in turn transformed by m2.*/
	class Matrix3x3
	{
	public:

		/** Creates a matrix that does not change vectors by multiplication since the diagonal is filed with ones and all other values are 0. */
		Matrix3x3()
		{
			setToIdentity();
		}

		/** That is the copy constructor which creates a new matrix which is equal to the matrix which is used as a parameter for the constructor.
		@param rhs The values of this matrix are equally used in the newly created matrix. */
		Matrix3x3(const Matrix3x3 &rhs);

		//Matrix3x3(const Real *values); too dangerous since you forget to think about weather the values are in row or column major order

		/** Creates a matrix by specifying each value. Note that the first index corresponds to the row and the second value identifies the column of the element.
		@param v11 This is the entry in the first row and first column.
		@param v12 This is the entry in the first row and second column. 
		@param v13 This is the entry in the first row and third column. 
		@param v21 This is the entry in the second row and first column. 
		@param v22 This is the entry in the second row and second column. 
		@param v23 This is the entry in the second row and third column.
		@param v31 This is the entry in the third row and first column. 
		@param v32 This is the entry in the third row and second column. 
		@param v33 This is the entry in the third row and third column.   */
		Matrix3x3(Real v11, Real v12, Real v13,
				  Real v21, Real v22, Real v23,
				  Real v31, Real v32, Real v33);

		/** Creates a matrix from three row vectors according to the Direct X matrix convention. (The rows of the matrix form its coordinate system.)
		@param v1 This becomes the first row of the matrix.
		@param v2 This becomes the second row of the matrix.
		@param v3 This becomes the third row of the matrix.*/
		Matrix3x3(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3);

		/** Creates an orthonormal matrix consisting of three vectors with unit length that are perpendicular to each other. (3 coordinate system axes).
			For example, you can use this to create a local coordinate system for an object surface based on a normal vector (= v).
		@param v Set this to the vector which is going to be the first vector of the returned basis. The other two vectors are chosen to be perpendicular to it and each other.
		@param normalized Set this to true if v is a vector of unit length.
		@return Returns a matrix in which each row is a basis vector of unit length. All basis vectors are perpendicular to each other.
			The first row vector (m00, m01, m02) has the same direction as the entered vector v. */
		static Matrix3x3 createBasisFromVector(const Vector3 &v, bool normalized = false);

		/** Computes an eigendecomposition of the symmetric matrix symmetricA.
			An eigenbasis V containing normalized and pairwise orthogonal row eigenvectors of symmetricA is computed.
			V * symmetricA = DiagonalMatrix(eigenvalues) * V;
		@param eigenvalues Is filled with the sorted eigenvalues of symmetricA whereas e[0] >= e[1] >= e[2].
		@param symmetricA The algorithm assumes that symmetricA is a symmetric, real 3x3 matrix.
		@param fast Set this to true if you want the version that is faster but also a bit more inaccurate due to numerical error issues.
		@param EQUAL_EIGENVECTORS_TOLERANCE Two eigenvalues are considered to be equal if their absolute difference is smaller than this value.
			   Eigenvector computation is different depending on the number of eigenvector multiplicity.
		@return Returns an orthonormal eigenbasis containing three normalized and pairwise orthogonal eigenvectors.
				First row eigenvector belongs to eigenvalues[0], second row eigenvector belongs to eigenvalues[1] and third row eigenvector belongs to eigenvalues[2].
		@note Function is based on:\n
			  http://en.wikipedia.org/wiki/Eigenvalue_algorithm#3.C3.973_matrices */
		static Matrix3x3 createEigenbasisFromSymmetric(Real *eigenvalues, const Matrix3x3 &symmetricA,
													   const bool fast, const Real EQUAL_EIGENVECTORS_TOLERANCE);

		/** Creates a matrix from an array of floating point values which are stored as rows one after another.
		This means that the first value in the second row will be set to data[3]. Likewise, the element v31 (third row, first column) is set to data[6].
		@param data This is a matrix consisting of an array of floating point values which are stored as rows one after another.*/
		static Matrix3x3 createFromMajorRowArray(const Real * data);

		/** Creates a matrix from an array of floating point values which are stored as columns one after another.
		This means that the first value in the second row will be set to data[1]. Likewise, the element v31 (third row, first column) is set to data[2].
		@param data This is a matrix consisting of an array of floating point values which are stored as columns one after another.*/
		static Matrix3x3 createFromMajorColumnArray(const Real * data);

		/** Creates the inverse matrix m^-1 of a matrix m so that m * m^-1 = m^-1 * m = E whereas E is the identity matrix.
		@param matrix	The inverse matrix of this parameter is computed.
		@return The function returns the inverse matrix of the input matrix.*/
		static Matrix3x3 createInverse(const Matrix3x3 &matrix);
		
		/** Computes a 3x3 projection matrix for a projection matrix from fov in y-direction (rendering camera model, right handed, looking along negative z-direction) which
			maps normalized homogenous device coordinates (x, y, z) with z = 1 (after viewport transformation - not in pixels) to view space camera ray directions.
			Actually, a projection matrix is not invertible and this matrix only computes the corresponding x, y and z coordinates of the ray directions.
			That means that the length of the returned ray direction is rather arbitrary.
		@param fovY Set this to the field of view angle in y-direction to define the opening of the camera.
		@param aspectRatio Set this to the ratio of the image plane width to its height. Must be positive.
		@see See createProjectionFovRHOpenGL, createProjectionFovRHDirectX*/
		static Matrix3x3 createInverseProjectionFovRH(const Real fovY, const Real aspectRatio);

		/** Computes a 3x3 projection matrix for a real world camera (pinhole model, right handed, looking along negative z-direction) which
			maps normalized homogenous device coordinates (x, y, z) with z = 1 (after viewport transformation - not in pixels) to view space camera ray directions.
			Actually, a real world (pinhole model) projection matrix is not invertible and this matrix only computes the corresponding x, y and z coordinates of the ray directions.
			That means that the length of the returned ray direction is rather arbitrary.
		@param focalLength Set this to the focalLength with which the target image was captured.
		@param aspectRatio Set this to the ratio of the image plane width to its height. Must be positive.
		@see See createProjectionRealWorld*/
		static Matrix3x3 createInverseProjectionRealWorld(const Real focalLength, const Real aspectRatio);

		/** Creates a transformation that revolves a vector around the x-axis of the coordinate system the vector is relative to by a certain angle.
		@param radian The vector is rotated by the value of the radian parameter which is a defined in radian measure.
		@returns The returned matrix specifies a transformation that revolves a vector around the x-axis by the angle radian.*/
		static Matrix3x3 createRotationX(Real radian);

		/** Creates a transformation that revolves a vector around the y-axis of the coordinate system the vector is relative to by a certain angle.
		@param radian The vector is rotated by the value of the radian parameter which is a defined in radian measure.
		@returns The returned matrix specifies a transformation that revolves a vector around the y-axis by the angle radian.*/
		static Matrix3x3 createRotationY(Real radian);
		
		/** Creates a transformation that revolves a vector around the z-axis of the coordinate system the vector is relative to by a certain angle.
		@param radian The vector is rotated by the value of the radian parameter which is a defined in radian measure.
		@returns The returned matrix specifies a transformation that revolves a vector around the z-axis by the angle radian.*/
		static Matrix3x3 createRotationZ(Real radian);

		/** Creates a transformation which encapsulates three rotations around the three axis of the coordinate system (result = vx * rotX * rotY * rotZ, vx is a row vector).
		Each rotation is relative to the original coordinate system. ( = "rotation around world axis")
		@param radianX Defines the angle (radian measure) which is used for the first rotation which rotates a vector around the x-axis (relative to the original coordinate system).
		@param radianY Specifies the angle (radian measure) which is necessary for the second rotation which rotates a vector around the y-axis (relative to the original coordinate system).
		@param radianZ Defines the angle (radian measure) which is required for the third rotation which rotates a vector around the z-axis (relative to the original coordinate system).*/
		static Matrix3x3 createRotationFromExtrinsicAngles(Real radianX, Real radianY, Real radianZ);

		
		/** Creates a transformation which encapsulates three rotations around the three axis of the coordinate system (result = vx * rotZ * rotY * rotX, vx is a row vector).
		Each rotation is relative to the rotated coordinate system which is the result of the previous rotation. ( = "rotation around object axis")
		@param radianX Defines the angle (radian measure) which is used for the first rotation which rotates a vector around the x-axis (relative to the world / rotated coordinate system).
		@param radianY Specifies the angle (radian measure) which is necessary for the second rotation which rotates a vector around the y-axis (relative to the rotated coordinate system).
		@param radianZ Defines the angle (radian measure) which is required for the third rotation which rotates a vector around the z-axis (relative to the rotated coordinate system).*/
		static Matrix3x3 createRotationFromIntrinsicAngles(Real radianX, Real radianY, Real radianZ);

		/** Sets the value of this matrix to the values of rhs.
		@param rhs The values of rhs are copied into the left-hand side matrix.
		@returns The matrix the values are assigned to is returned. */
		Matrix3x3 &operator =(const Matrix3x3 &rhs);
		//Matrix3x3 &operator =(const Real *values); too dangerous since you forget to think about weather the values are in row or column major order

		/** Compares two matrices to check for equality.
		@param rhs The left-hand side matrix is compared to rhs.
		@returns The operator returns true if all entries of this matrix are approximately equal to corresponding the elements of rhs.  (see Math::EPSILON)*/
		inline bool operator ==(const Matrix3x3 &rhs) const
		{
			return this->equals(rhs);
		}

		/** Compares two matrices to check for equality.
		@param rhs The left-hand side matrix is compared to rhs.
		@param epsilon Set this to define the threshold until which two floating point numbers are considered to be equal.
		@returns The operator returns true if all entries of this matrix are approximately equal to corresponding the elements of rhs.  (see Math::EPSILON)*/
		bool equals(const Matrix3x3 &rhs, const Real epsilon = EPSILON) const;

		/** Compares two matrices to check for disparity.
		@param rhs The left-hand side matrix is compared to rhs.
		@returns The operator returns false if all entries of this matrix are approximately equal to the corresponding elements of rhs.  (see Math::EPSILON)*/
		bool operator !=(const Matrix3x3 &rhs) const
		{
			return !(*this == rhs);
		}

		/** Adds two matrices and returns the result. The addition is done componentwise: result(i, j) = this(i, j) + rhs(i, j)
		@param rhs The result is rhs + this.
		@returns The returned matrix is the result of the componentwise addition.*/
		Matrix3x3 operator +(const Matrix3x3 &rhs) const;

		/** Adds matrix rhs to this matrix componentwise and returns the new matrix. The addition is done componentwise: this(i, j) += + rhs(i, j).
		@param rhs This matrix is changed to rhs + this and returned.
		@returns The returned matrix is the result of the componentwise addition.*/
		Matrix3x3 &operator +=(const Matrix3x3 &rhs);
		
		/** Substracts two matrices and returns the result. The substraction is carried for each element, e.g. result(i, j) = this(i, j) - rhs(i, j)
		@param rhs The result is this - rhs.
		@returns The matrix returned is the componentwise difference: this - rhs.*/
		Matrix3x3 operator -(const Matrix3x3 &rhs) const;

		/** Multiplies each entry of the matrix by the value of scalar and returns the resulting matrix.
		@param scalar Each value is multiplied by scalar.
		@returns The returned matrix has entries which were multiplied by the value of scalar. */
		Matrix3x3 operator *(Real scalar) const;

		/** Multiplies each matrix entry by scalar so that each entry e[i][j] becomes scalar * e[i][j].
		@param scalar Each value is multiplied by scalar. */
		void operator *=(Real scalar);

		/** Divides each entry of the matrix by the value of scalar and returns the resulting matrix.
		@param scalar Each value is divided by scalar.
		@returns The returned matrix has entries which were divided by the value of scalar. */
		Matrix3x3 operator /(Real scalar) const;

		/** Creates a transformation which is the concatenation of this matrix and rhs whereas the transformation of this matrix is caried out first. (DirectX convention).
		Take care as this operation is not commutative!!!!!!!!!!!!!
		@param rhs The resulting matrix is the transformation of this matrix followed by the transformation of rhs. (DirectX convention)
		@returns The result is the transformation of this matrix followed by the transformation of rhs. (DirectX convention)*/
		Matrix3x3 operator *(const Matrix3x3 &rhs) const;
		
		/** Accesses an entry of the matrix (first value indicates the row and the second value indicates the column).
		@param row Determines the row of the element which is returned.
		@param column Denomniates the column of the element which is returned. */
		Real &operator()(unsigned short int row, unsigned short int column)
		{
			assert(row < 3 && column < 3);
			return values[row][column];
		}

		/** Accesses an entry of the matrix (first value indicates the row and the second value indicates the column).
		@param row Determines the row of the element which is returned.
		@param column Denomniates the column of the element which is returned. */
		Real operator()(unsigned short int row, unsigned short int column) const
		{
			assert(row < 3 && column < 3);
			return values[row][column];
		}

		/** Adds a relative x-y-translation to the matrix which is applied after the transformation which is already contained by the matrix.
		@param translation If a homogenous 3D vector is multiplied with this matrix then this added 2D translation is applied to the vector after the transformation already contained by the matrix. */
		void addTranslation(const Vector2 &translation);

		/** Computes the Frobenius inner product of the two matrices which is analogous to the inner product of two vectors. (= sum of pairwise component products)
		@param rhs Computes the Frobenius inner product of rhs and the left hand side matrix. The Frobenius inner product is commutative.
		@return The Frobenius inner product (~= skalar product of both matrices) is returned. */
		Real frobeniusProduct(const Matrix3x3 &rhs) const;

		/** Computes and returns the determinant of the matrix.
		@returns The resulted value is the determinant of the matrix. (If it is 0 then the matrix is singular as there exist no inverse matrix. */
		Real getDeterminant() const;

		/** Returns the entries of the matrix which are stored in row major order (one row after another in the array, DirectX convention).
		@returns The returned value points to an array which contains the rows of the matrix (one after another). */
		const Real *getData() const { return *values; }

		/** Takes a symmetric, real 3x3 matrix and finds its three real values e[0], e[1], e[2]
			with e[0] >= e[1] >= e[2] and A * v0 = e[0] * v[0], A * v[1] = e[1] * v[1], A * v[2] = e[2] * v[2].
		@param eigenvalues Contains the three sorted real eigenvalues with e[0] >= e[1] >= e[2]. They must exist for a 3x3 real symmmetric matrix.
		@param symmetricA The algorithm assumes that symmetricA is a symmetric, real 3x3 matrix.
		@note Function is based on:\n
			  http://en.wikipedia.org/wiki/Eigenvalue_algorithm#3.C3.973_matrices */
		static void getEigenvaluesFromSymmetric(Real eigenvalues[3], const Matrix3x3 &symmetricA);

		/** Returns the rotation angles of the three rotations around the coordinate axis which are encapsulated by this matrix.
		(result = vx * rotX * rotY * rotZ, vx is a row vector)
		All rotation angles are relative to the original coordinate system.
		@param x The angle of the first rotation which revolves a vector around the x axis (relative to the original coordinate system).
		@param y The angle of the second rotation which revolves a vector around the y axis (relative to the original coordinate system).
		@param z The angle of the third rotation which revolves a vector around the z axis (relative to the original coordinate system). */
		void getExtrinsicRotationAngles(Real &x, Real &y, Real &z) const;

		/** Returns the rotation angles of the three rotations around the coordinate axis which are encapsulated by this matrix.
		(result = vx * rotZ * rotY * rotX, vx is a row vector)
		All rotation angles are relative to the rotated coordinate system which is the result of the previous rotation.
		@param x The angle of the first rotation which revolves a vector around the x axis (relative to the original / rotated coordinate system).
		@param y The angle of the second rotation which revolves a vector around the y axis (relative to the rotated coordinate system).
		@param z The angle of the third rotation which revolves a vector around the z axis (relative to the rotated coordinate system). */
		void getIntrinsicRotationAngles(Real &x, Real &y, Real &z) const;

		/** Returns a specific row vector of this matrix. For example, 0th row vector = (m00, m01, m02) for rowVectorIdx = 0.
		@param target Is filled with the specified matrix row entries. For example, row vector 2 (m20, m21, m22) for rowVectorIdx = 2.
		@param rowVectorIdx Specifies which row vector is returned. For this Matrix3x3 object only values in { 0, 1, 2 } make sense. */
		void getRowVector(Vector3 &target, uint32 rowVectorIdx) const;

		/** Returns the trace of the matrix. The trace of a matrix is the sum of its diagonal elements.
		@return Returns the trace of the matrix. The trace of a matrix is the sum of its diagonal elements.*/
		Real getTrace() const { return m00 + m11 + m22;  }

		/** Changes the matrix to the identity matrix.
			This means that the diagonal consists of ones and all other entries are set to 0.*/
		void setToIdentity()
		{
			m00 = m11 = m22 = 1.0f;
			m01 = m02 = 0.0f;
			m10 = m12 = 0.0f;
			m20 = m21 = 0.0f;
		}

		/** Sets every matrix component to zero. */
		void setToZero()
		{
			m00 = m01 = m02 = 0.0f;
			m10 = m11 = m12 = 0.0f;
			m20 = m21 = m22 = 0.0f;
		}

		/** Swaps rows and columns, every row becomes a column with the same number.
			(= Every column becomes a row, for i != j swaps mij and mji.) */
		void transpose()
		{
			// m00, m11, m22 don't change
			Utilities::swap(m01, m10);
			Utilities::swap(m02, m20);
			Utilities::swap(m12, m21);
		}

	private:
		/** Returns the normalized row vector of A * B which was the largest vector in A * B.
			This function is used to make Matrix3x3::createEigenbasisFromSymmetric more numerically stable.
		@param A Returns the normalized row vector of A * B which was the largest vector in A * B.
		@param B Returns the normalized row vector of A * B which was the largest vector in A * B.
		@return Returns the normalized row vector of A * B which was the largest vector in A * B.*/
		static Vector3 getLargestRowVectorNormalized(const Matrix3x3 &A, const Matrix3x3 &B);

	public:
		/// The nine entries of the 3x3 matrix
		union
		{
			/// The nine entries of the 3x3 matrix
			Real values[3][3];
			struct
			{
				Real m00, m01, m02;
				Real m10, m11, m12;
				Real m20, m21, m22;
			};
		};
	};

	/** Write the values of a 3x3 matrix to an output stream.
	@param os The matrix information is written to this stream.
	@param m The entries of m are written to the output stream.
	@returns Operator returns the extended output stream. */
	std::ostream &operator <<(std::ostream &os, const Math::Matrix3x3 &m);
}

#endif // _MATRIX_3_X_3_H_

