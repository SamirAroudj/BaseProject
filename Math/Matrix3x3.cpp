/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Platform/DataTypes.h"
#include "Math/MathHelper.h"
#include "Math/Matrix3x3.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"

using namespace Math;
using namespace std;

Matrix3x3::Matrix3x3(const Matrix3x3 &rhs) :
	m00(rhs.m00),
	m01(rhs.m01),
	m02(rhs.m02),

	m10(rhs.m10),
	m11(rhs.m11),
	m12(rhs.m12),

	m20(rhs.m20),
	m21(rhs.m21),
	m22(rhs.m22)
{

}

/*Matrix3x3::Matrix3x3(const Real *values) :
	m00(values[0]),
	m01(values[1]),
	m02(values[2]),

	m10(values[3]),
	m11(values[4]),
	m12(values[5]),

	m20(values[6]),
	m21(values[7]),
	m22(values[8])
{

}*/

Matrix3x3::Matrix3x3(Real v11, Real v12, Real v13, Real v21, Real v22, Real v23, Real v31, Real v32, Real v33) :
	m00(v11),
	m01(v12),
	m02(v13),

	m10(v21),
	m11(v22),
	m12(v23),

	m20(v31),
	m21(v32),
	m22(v33)
{

}

Matrix3x3::Matrix3x3(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3) :
	Matrix3x3(	v1.x, v1.y, v1.z,
				v2.x, v2.y, v2.z,
				v3.x, v3.y, v3.z	)
{
		
}

Matrix3x3 Matrix3x3::createBasisFromVector(const Vector3 &v, bool normalized)
{
	Vector3 v0, v1, v2;
	v.createBasis(v0, v1, v2);
	return Matrix3x3(v0.x, v0.y, v0.z,
					 v1.x, v1.y, v1.z,
					 v2.x, v2.y, v2.z);
}

Matrix3x3 Matrix3x3::createEigenbasisFromSymmetric(Real *eigenvalues, const Matrix3x3 &A, const bool fast, const Real EQUAL_EIGENVECTORS_TOLERANCE)
{
	// eigenvector extraction is based on Cayley-Hamilton theorem, explained in http://en.wikipedia.org/wiki/Eigenvalue_algorithm#3.C3.973_matrices
	Matrix3x3::getEigenvaluesFromSymmetric(eigenvalues, A);

	// repeated eigenvalues? remember: eigenvalues are sorted with e[0] >= e[1] >= e[2]
	int32 notRepeatedIdx = -1;
	if (EQUAL_EIGENVECTORS_TOLERANCE > fabsr(eigenvalues[0] - eigenvalues[1])) // e[0] == e[1]
	{
		// 1 distinct eigenvalue: e[0] - any eigenbasis will work as A is a uniform scaling matrix with scaling factor e[0], so return the identity matrix
		if (EQUAL_EIGENVECTORS_TOLERANCE > fabsr(eigenvalues[0] - eigenvalues[2]))
			return Matrix3x3();
		
		notRepeatedIdx = 2;
	}
	else if (EQUAL_EIGENVECTORS_TOLERANCE > fabsr(eigenvalues[1] - eigenvalues[2]) ) 
	{
		notRepeatedIdx = 0;
	}
	// else if (e[0] == e[2]) is already checked due to sorted order
	
	// at least two distinct eigenvalues -> compute the eigenvector basis
	Vector3 eigenvectors[3];

	if (-1 != notRepeatedIdx)
	{
		Matrix3x3 A1(A);
		A1.m00 -= eigenvalues[1];
		A1.m00 -= eigenvalues[1];
		A1.m00 -= eigenvalues[1];

		eigenvectors[notRepeatedIdx] = getLargestRowVectorNormalized(A1, A1);
		
		// the other two eigenvectors must be in a plane perpendicular to the already computed one
		Real lengthAlongXAxis = fabsr(eigenvectors[notRepeatedIdx].x);
		if (fabsr(lengthAlongXAxis - 1.0f) < EPSILON) // Is eigenvectors[notRepeatedIdx] parallel to x-axis?
		{
			// make y-axis perpendicular to eigenvectors[notRepeatedIdx]
			eigenvectors[1] = eigenvectors[notRepeatedIdx] * -eigenvectors[notRepeatedIdx].y;
			eigenvectors[1].y += 1;
		}
		else
		{
			// make x-axis perpendicular to eigenvectors[notRepeatedIdx]
			eigenvectors[1] = eigenvectors[notRepeatedIdx] * -eigenvectors[notRepeatedIdx].x;
			eigenvectors[1].x += 1;
		}
		eigenvectors[1].normalize();
		
		uint32 lastEigenvectorIdx = (2 == notRepeatedIdx ? 0 : 2);
		eigenvectors[lastEigenvectorIdx] = eigenvectors[notRepeatedIdx].crossProduct(eigenvectors[1]); // does not need to be normalized
	}
	else
	{
		// 3 distinct eigenvalues
		// Ai = A - eigenvalue[i] * I
		// Vi = Aj * Ak = Ak * Aj with i, j, k � {0, 1, 2} ^ i != j ^ j != k ^ i != k
		// for i � {0, 1, 2}: Vi^t = Vi
		// columns of Vi contain eigenvector vi (based on Cayley-Hamilton theorem) (same is true for rows as Vi is symmetric)
		// ==> compute first row of every Vi to get the eigenvectors
		if (fast)
		{
			Real A0m00 = A.m00 - eigenvalues[0];
			Real A0m11 = A.m11 - eigenvalues[0];
			Real A0m22 = A.m22 - eigenvalues[0];
			Real A1m00 = A.m00 - eigenvalues[1];
			Real A1m11 = A.m11 - eigenvalues[1];
			Real A1m22 = A.m22 - eigenvalues[1];
			Real A2m00 = A.m00 - eigenvalues[2];
			Real A2m11 = A.m11 - eigenvalues[2];
			Real A2m22 = A.m22 - eigenvalues[2];
	
			// v0 = firstRow(A1 * A2)
			eigenvectors[0].set(A1m00 * A2m00	+ A.m01 * A.m10	+ A.m02 * A.m20,
								A1m00 * A.m01	+ A.m01 * A2m11	+ A.m02 * A.m21,
								A1m00 * A.m02	+ A.m01 * A.m12	+ A.m02 * A2m22);
			eigenvectors[0].normalize();

			// v1 = secondRow(A0 * A2)
			eigenvectors[1].set(A.m10 * A2m00	+ A0m11 * A.m10	+ A.m12 * A.m20,
								A.m10 * A.m01	+ A0m11 * A2m11	+ A.m12 * A.m21,
								A.m10 * A.m02	+ A0m11 * A.m12	+ A.m12 * A2m22);
			eigenvectors[1].normalize();

			// v2 = thirdRow(A0 * A1)
			eigenvectors[2].set(A.m20 * A1m00	+ A.m21 * A.m10	+ A0m22 * A.m20,
								A.m20 * A.m01	+ A.m21 * A1m11	+ A0m22 * A.m21,
								A.m20 * A.m02	+ A.m21 * A.m12	+ A0m22 * A1m22);
			eigenvectors[2].normalize();
		}
		else
		{
			Matrix3x3 A0(A);
			A0.m00 -= eigenvalues[0];
			A0.m11 -= eigenvalues[0];
			A0.m22 -= eigenvalues[0];

			Matrix3x3 A1(A);
			A1.m00 -= eigenvalues[1];
			A1.m11 -= eigenvalues[1];
			A1.m22 -= eigenvalues[1];

			Matrix3x3 A2(A);
			A2.m00 -= eigenvalues[2];
			A2.m11 -= eigenvalues[2];
			A2.m22 -= eigenvalues[2];

			eigenvectors[0] = getLargestRowVectorNormalized(A1, A2);
			eigenvectors[1] = getLargestRowVectorNormalized(A0, A2);
			eigenvectors[2] = getLargestRowVectorNormalized(A0, A1);
		}
	}

	return Matrix3x3(eigenvectors[0].x, eigenvectors[0].y, eigenvectors[0].z,
					 eigenvectors[1].x, eigenvectors[1].y, eigenvectors[1].z,
					 eigenvectors[2].x, eigenvectors[2].y, eigenvectors[2].z);
}

Vector3 Matrix3x3::getLargestRowVectorNormalized(const Matrix3x3 &A, const Matrix3x3 &B)
{
	Matrix3x3 C = A * B;

	// squared lengths
	Real lengthsSq[3]	= { C.m00 * C.m00 + C.m01 * C.m01 + C.m02 * C.m02,
							C.m10 * C.m10 + C.m11 * C.m11 + C.m12 * C.m12,
							C.m20 * C.m20 + C.m21 * C.m21 + C.m22 * C.m22, };

	// identify largest row vector
	uint32	maxIdx	= 0;
	Real	maxSq	= lengthsSq[maxIdx];

	if (lengthsSq[1] > maxSq)
	{
		maxSq	= lengthsSq[1];
		maxIdx	= 1;
	}

	if (lengthsSq[2] > maxSq)
		maxSq = lengthsSq[2];
	
	// create normalized vector from largest row
	Real	one_length = 1.0f / sqrtr(maxSq);
	Vector3	vector(C(maxIdx, 0), C(maxIdx, 1), C(maxIdx, 2));
	vector *= one_length;

	// normalize again due to numerical problems
	Real lengthSq = vector.getLengthSquared();
	if (fabsr(lengthSq - 1.0f) > EPSILON)
	{
		one_length	 = 1.0f / sqrtr(lengthSq);
		vector		*= one_length;
	}

	return vector;
}

void Matrix3x3::getEigenvaluesFromSymmetric(Real eigenvalues[3], const Matrix3x3 &A)
{
	// check that A is really symmetric
	#ifdef _DEBUG
		assert(EPSILON > fabsr(A.m01 - A.m10));
		assert(EPSILON > fabsr(A.m02 - A.m20));
		assert(EPSILON > fabsr(A.m12 - A.m21));
	#endif // _DEBUG

	// based on: http://en.wikipedia.org/wiki/Eigenvalue_algorithm#3.C3.973_matrices
	Real p1 = A.m01 * A.m01 + A.m02 * A.m02 + A.m12 * A.m12;
		
	if (p1 < EPSILON)  // Is A a diagonal matrix? Return sorted eigenvalues: e1 >= e2 >= e3
	{
		if (A.m00 >= A.m11)
		{
			if (A.m11 >= A.m22)
			{
				eigenvalues[0] = A.m00;
				eigenvalues[1] = A.m11;
				eigenvalues[2] = A.m22;
			}
			else if (A.m00 >= A.m22)
			{
				eigenvalues[0] = A.m00;
				eigenvalues[1] = A.m22;
				eigenvalues[2] = A.m11;
			}
			else
			{
				eigenvalues[0] = A.m22;
				eigenvalues[1] = A.m00;
				eigenvalues[2] = A.m11;
			}
		}
		else
		{
			if (A.m00 >= A.m22)
			{
				eigenvalues[0] = A.m11;
				eigenvalues[1] = A.m00;
				eigenvalues[2] = A.m22;
			}
			else if (A.m11 >= A.m22)
			{
				eigenvalues[0] = A.m11;
				eigenvalues[1] = A.m22;
				eigenvalues[2] = A.m00;
			}
			else
			{
				eigenvalues[0] = A.m22;
				eigenvalues[1] = A.m11;
				eigenvalues[2] = A.m00;
			}
		}

		return;
	}

	// A is not a diagonal matrix so transform A to A = pB + qI to make the solution easier
	// (b is an eigenvalue of B if and only if a = pb + q is an eigenvalue of A)
	Real traceA	= A.getTrace();
	Real q		= traceA * ONE_THIRD;

	// p2 = p2 = 2 * p1 + (A11 - q)^2 + (A22 - q)^2 + (A33 - q)^2
	Real temp = 0.0f;
	Real p2	  = 2.0f * p1;

	temp = A.m00 - q;
	p2 += temp * temp;

	temp = A.m11 - q;
	p2 += temp * temp;

	temp = A.m22 - q;
	p2 += temp * temp;

	Real p = sqrtr(p2 * (1.0f / 6.0f));

	// B = (1 / p) * (A - q * I)
	Real one_p = 1.0f / p;
	Matrix3x3 B(one_p * (A.m00 - q),	one_p * (A.m01),		one_p * (A.m02),
				one_p * (A.m10),		one_p * (A.m11 - q),	one_p * (A.m12),
				one_p * (A.m20),		one_p * (A.m21),		one_p * (A.m22 - q));

	// phi = acos(|B| / 2) / 3
	Real r = B.getDeterminant() * 0.5f;
 
	// If floating point computations were completely correct, r � [-1, 1] were true. However, it must be capped due to floating point errors:
	Real phi = 0.0f;
	if (r <= -1.0f)
		phi = THIRD_PI;
	else if (r < 1.0f)
		phi = acosr(r) * ONE_THIRD;
	// else if (phi >= 1) phi = 0.0f;
 
	// now get the eigenvalues with e1 >= e2 >= e3 :)
	temp = 2.0f * p;
	eigenvalues[0] = q + temp * cosr(phi);
	eigenvalues[2] = q + temp * cosr(phi + (2.0f * THIRD_PI));
	eigenvalues[1] = traceA - eigenvalues[0] - eigenvalues[2]; // traceA = e1 + e2 + e3
}

Matrix3x3 Matrix3x3::createFromMajorRowArray(const Real * data)
{
	return Matrix3x3(	data[0], data[1], data[2],
						data[3], data[4], data[5],
						data[6], data[7], data[8]	);
}

Matrix3x3 Matrix3x3::createFromMajorColumnArray(const Real * data)
{
	return Matrix3x3(	data[0], data[3], data[6],
						data[1], data[4], data[7],
						data[2], data[5], data[8]	);
}


Matrix3x3 Matrix3x3::createInverse(const Matrix3x3 &m)
{
	Real coFactor11  = computeDeterminant(m.m11, m.m12, m.m21, m.m22);
	Real coFactor21  = -computeDeterminant(m.m01, m.m02, m.m21, m.m22);
	Real coFactor31  = computeDeterminant(m.m01, m.m02, m.m11, m.m12);
	Real determinant = (m.m00 * coFactor11 + m.m10 * coFactor21 + m.m20 * coFactor31);

	assert(fabsr(determinant) > EPSILON);
	Real oD = 1.0f / determinant;

	return Matrix3x3(
		oD * coFactor11,						oD * coFactor21,						oD * coFactor31,
		oD * (m.m12 * m.m20 - m.m22 * m.m10),	oD * (m.m00 * m.m22 - m.m20 * m.m02),	oD * (m.m02 * m.m10 - m.m12 * m.m00),
		oD * (m.m10 * m.m21 - m.m20 * m.m11),	oD * (m.m01 * m.m20 - m.m21 * m.m00),	oD * (m.m00 * m.m11 - m.m01 * m.m10));
}

Matrix3x3 Matrix3x3::createInverseProjectionFovRH(const Real fovY, const Real aspectRatio)
{
	const Real f = 1 / tanr(fovY * 0.5f); 
	const Real a = aspectRatio;
	return Matrix3x3(a,		0.0f,	0.0f,
					 0.0f,	1.0f,	0.0f,
					 0.0f,	0.0f,	-f);
}

Matrix3x3 Matrix3x3::createInverseProjectionRealWorld(const Real f, const Real aspectRatio)
{
	Real sX, sY;
	getXYProjectionScaleFactors(sX, sY, f, aspectRatio);

	return Matrix3x3(1.0f / sX,	0.0f,		0.0f,
					 0.0f,		1.0f / sY,	0.0f,
					 0.0f,		0.0f,		-1.0f);
}

Matrix3x3 Matrix3x3::createRotationX(Real radian)
{
	Real cosRadian = cosr(radian);
	Real sinRadian = sinr(radian);
	return Matrix3x3(	1.0f,	0.0f,		0.0f,
						0.0f,	cosRadian,	sinRadian,
						0.0f,	-sinRadian,	cosRadian);
}

Matrix3x3 Matrix3x3::createRotationY(Real radian)
{
	Real cosRadian = cosr(radian);
	Real sinRadian = sinr(radian);
	return Matrix3x3(	cosRadian,	0.0f,	-sinRadian,
						0.0f,		1.0f,	0.0f,
						sinRadian,	0.0f,	cosRadian);
}

Matrix3x3 Matrix3x3::createRotationZ(Real radian)
{
	Real cosRadian = cosr(radian);
	Real sinRadian = sinr(radian);
	return Matrix3x3(	cosRadian,	sinRadian,	0.0f,
						-sinRadian,	cosRadian,	0.0f,
						0.0f,		0.0f,		1.0f);
}

Matrix3x3 Matrix3x3::createRotationFromExtrinsicAngles(Real radianX, Real radianY, Real radianZ)
{
	/* the used code is a faster version of the concatenation
	Matrix3x3 mX = Matrix3x3::createRotationX(radianX);
	Matrix3x3 mY = Matrix3x3::createRotationY(radianY);
	Matrix3x3 mZ = Matrix3x3::createRotationZ(radianZ);
	return mX * mY * mZ;*/

	Real cosX = cosr(radianX); Real sinX = sinr(radianX);
	Real cosY = cosr(radianY); Real sinY = sinr(radianY);
	Real cosZ = cosr(radianZ); Real sinZ = sinr(radianZ);

	return Matrix3x3(
		cosZ * cosY,						sinZ * cosY,						-sinY,
		cosZ * sinY * sinX - sinZ * cosX,	sinZ * sinY * sinX + cosZ * cosX,	cosY * sinX,
		sinZ * sinX + cosZ * sinY * cosX,	sinZ * sinY * cosX - cosZ * sinX,	cosY * cosX);

}

Matrix3x3 Matrix3x3::createRotationFromIntrinsicAngles(Real radianX, Real radianY, Real radianZ)
{
	/* the used code is a faster version of the concatenation
	Matrix3x3 mX = Matrix3x3::createRotationX(radianX);
	Matrix3x3 mY = Matrix3x3::createRotationY(radianY);
	Matrix3x3 mZ = Matrix3x3::createRotationZ(radianZ);
	return mZ * mY * mX; */

	Real cosX = cosr(radianX); Real sinX = sinr(radianX);
	Real cosY = cosr(radianY); Real sinY = sinr(radianY);
	Real cosZ = cosr(radianZ); Real sinZ = sinr(radianZ);

	return Matrix3x3(
		cosY * cosZ,	cosX * sinZ + cosZ * sinX * sinY,	sinX * sinZ - cosX * cosZ * sinY,
		-cosY * sinZ,	cosX * cosZ - sinX * sinY * sinZ,	cosX * sinY * sinZ + cosZ * sinX,
		sinY,			-cosY * sinX,						cosX * cosY);
}

void Matrix3x3::addTranslation(const Vector2 &translation)
{
	m20 += translation.x;
	m21 += translation.y;
}

bool Matrix3x3::equals(const Matrix3x3 &rhs, const Real epsilon) const
{
	return	(epsilon > fabsr(m00 - rhs.m00)) &&
			(epsilon > fabsr(m01 - rhs.m01)) &&
			(epsilon > fabsr(m02 - rhs.m02)) &&

			(epsilon > fabsr(m10 - rhs.m10)) &&
			(epsilon > fabsr(m11 - rhs.m11)) &&
			(epsilon > fabsr(m12 - rhs.m12)) &&

			(epsilon > fabsr(m20 - rhs.m20)) &&
			(epsilon > fabsr(m21 - rhs.m21)) &&
			(epsilon > fabsr(m22 - rhs.m22));
}

Real Matrix3x3::frobeniusProduct(const Matrix3x3 &rhs) const
{
	return	m00 * rhs.m00 + m01 * rhs.m01 + m02 * rhs.m02 +
			m10 * rhs.m10 + m11 * rhs.m11 + m12 * rhs.m12 +
			m20 * rhs.m20 + m21 * rhs.m21 + m22 * rhs.m22;
}

void Matrix3x3::getColumnVector(Vector3 &target, uint32 columnVectorIdx) const
{
	assert(columnVectorIdx >= 0);
	assert(columnVectorIdx <= 2);

	target.x = values[0][columnVectorIdx];
	target.y = values[1][columnVectorIdx];
	target.z = values[2][columnVectorIdx];
}

Real Matrix3x3::getDeterminant() const
{
	return	 m00 * (m11 * m22 - m21 * m12)
			-m10 * (m01 * m22 - m21 * m02)
			+m20 * (m01 * m12 - m11 * m02);
}

void Matrix3x3::getExtrinsicRotationAngles(Real &x, Real &y, Real &z) const
{
	Real sinY = -m02;
	Real cosY = sqrtr(1.0f - sinY * sinY);
	if (fabsr(cosY) < EPSILON)
	{
		x = acosr(m20);
		y = HALF_PI;
		z = 0.0f;
		return;
	}

	Real cosX = m22 / cosY;
	Real cosZ = m00 / cosY;

	x = acosr(cosX);
	y = asinr(sinY);
	z = acosr(cosZ);
}

void Matrix3x3::getIntrinsicRotationAngles(Real &x, Real &y, Real &z) const
{
	Real sinY = m20;
	Real cosY = sqrtr(1.0f - sinY * sinY);
	if (fabsr(cosY) < EPSILON)
	{
		x = acosr(m11);
		y = HALF_PI;
		z = 0.0f;
		return;
	}

	Real cosX = m22 / cosY;
	Real cosZ = m00 / cosY;

	x = acosr(cosX);
	y = asinr(sinY);
	z = acosr(cosZ);
}

void Matrix3x3::getRowVector(Vector3 &target, uint32 rowVectorIdx) const
{
	assert(rowVectorIdx >= 0);
	assert(rowVectorIdx <= 2);

	target.x = values[rowVectorIdx][0];
	target.y = values[rowVectorIdx][1];
	target.z = values[rowVectorIdx][2];
}

Matrix3x3 &Matrix3x3::operator =(const Matrix3x3 &rhs)
{
	m00 = rhs.m00;
	m01 = rhs.m01;
	m02 = rhs.m02;

	m10 = rhs.m10;
	m11 = rhs.m11;
	m12 = rhs.m12;

	m20 = rhs.m20;
	m21 = rhs.m21;
	m22 = rhs.m22;

	return *this;
}

/*Matrix3x3 &Matrix3x3::operator =(const Real *values)
{
	m00 = values[0];
	m01 = values[1];
	m02 = values[2];

	m10 = values[3];
	m11 = values[4];
	m12 = values[5];

	m20 = values[6];
	m21 = values[7];
	m22 = values[8];

	return *this;
}*/

Matrix3x3 Matrix3x3::operator +(const Matrix3x3 &rhs) const
{
	return Matrix3x3(	m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02,
						m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12,
						m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22);
}

Matrix3x3 &Matrix3x3::operator +=(const Matrix3x3 &rhs)
{
	m00 += rhs.m00;
	m01 += rhs.m01;
	m02 += rhs.m02;
	
	m10 += rhs.m10;
	m11 += rhs.m11;
	m12 += rhs.m12;

	m20 += rhs.m20;
	m21 += rhs.m21;
	m22 += rhs.m22;

	return *this;
}

Matrix3x3 Matrix3x3::operator -(const Matrix3x3 &rhs) const
{
	return Matrix3x3(	m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02,
						m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12,
						m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22);
}


Matrix3x3 Matrix3x3::operator *(Real scalar) const
{
	return Matrix3x3(	scalar * m00, scalar * m01, scalar * m02,
						scalar * m10, scalar * m11, scalar * m12,
						scalar * m20, scalar * m21, scalar * m22);
}

void Matrix3x3::operator *=(Real scalar)
{
	m00 *= scalar;
	m01 *= scalar;
	m02 *= scalar;

	m10 *= scalar;
	m11 *= scalar;
	m12 *= scalar;

	m20 *= scalar;
	m21 *= scalar;
	m22 *= scalar;
}

Matrix3x3 Matrix3x3::operator /(Real scalar) const
{
	Real one_scalar = 1.0f / scalar;
	return Matrix3x3(	one_scalar * m00, one_scalar * m01, one_scalar * m02,
						one_scalar * m10, one_scalar * m11, one_scalar * m12,
						one_scalar * m20, one_scalar * m21, one_scalar * m22);
}

Matrix3x3 Matrix3x3::operator *(const Matrix3x3 &b) const
{
	return Matrix3x3(	m00 * b.m00 + m01 * b.m10 + m02 * b.m20,	m00 * b.m01 + m01 * b.m11 + m02 * b.m21,	m00 * b.m02 + m01 * b.m12 + m02 * b.m22,
						m10 * b.m00 + m11 * b.m10 + m12 * b.m20,	m10 * b.m01 + m11 * b.m11 + m12 * b.m21,	m10 * b.m02 + m11 * b.m12 + m12 * b.m22, 
						m20 * b.m00 + m21 * b.m10 + m22 * b.m20,	m20 * b.m01 + m21 * b.m11 + m22 * b.m21,	m20 * b.m02 + m21 * b.m12 + m22 * b.m22);
}

ostream &operator <<(ostream &os, const Matrix3x3 &m)
{
	os << "Matrix 3x3:\n";
	for (unsigned short int i = 0; i < 3; ++i)
	{
		for (unsigned short int j = 0; j < 3; ++j)
			os << m(i, j) << "\t";
		os << "\n";
	}

	return os;
}
