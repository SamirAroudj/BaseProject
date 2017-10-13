/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Platform/DataTypes.h"
#include "Math/MathHelper.h"
#include "Math/Matrix2x2.h"
#include "Math/Vector2.h"

using namespace Math;
using namespace std;

Matrix2x2::Matrix2x2(const Matrix2x2 &rhs) :
	m00(rhs.m00),
	m01(rhs.m01),

	m10(rhs.m10),
	m11(rhs.m11)
{

}

Matrix2x2::Matrix2x2(Real v11, Real v12, Real v21, Real v22) :
	m00(v11),
	m01(v12),

	m10(v21),
	m11(v22)
{

}

Matrix2x2::Matrix2x2(const Vector2 &v1, const Vector2 &v2) :
	Matrix2x2(	v1.x, v1.y,
				v2.x, v2.y)
{

}

Matrix2x2 Matrix2x2::createFromMajorRowArray(const Real * data)
{
	return Matrix2x2(	data[0], data[1],
						data[2], data[3]);
}

Matrix2x2 Matrix2x2::createFromMajorColumnArray(const Real * data)
{
	return Matrix2x2(	data[0], data[2],
						data[1], data[3]);
}


Matrix2x2 Matrix2x2::createInverse(bool &invertible, const Matrix2x2 &m)
{
	Real determinant = m.m00 * m.m11 - m.m01 * m.m10;
	if (fabsr(determinant) < EPSILON)
	{
		invertible = false;
		return Matrix2x2(0.0f, 0.0f, 0.0f, 0.0f);
	}

	// closed form ;)
	Real invD		= 1.0f / determinant;
		 invertible	= true;

	return Matrix2x2(m.m11 * invD, -m.m01 * invD,
					 -m.m10 * invD, m.m00 * invD);
}

Matrix2x2 Matrix2x2::createRotation(Real radian)
{
	Real cosRadian = cosr(radian);
	Real sinRadian = sinr(radian);
	return Matrix2x2(	cosRadian,	sinRadian,
						-sinRadian,	cosRadian	);
}

Matrix2x2 &Matrix2x2::operator =(const Matrix2x2 &rhs)
{
	m00 = rhs.m00;
	m01 = rhs.m01;

	m10 = rhs.m10;
	m11 = rhs.m11;

	return *this;
}

bool Matrix2x2::equals(const Matrix2x2 &rhs, const Real epsilon) const
{
	return	(epsilon > fabsr(m00 - rhs.m00)) &&
			(epsilon > fabsr(m01 - rhs.m01)) &&

			(epsilon > fabsr(m10 - rhs.m10)) &&
			(epsilon > fabsr(m11 - rhs.m11));
}

Matrix2x2 Matrix2x2::operator +(const Matrix2x2 &rhs) const
{
	return Matrix2x2(	m00 + rhs.m00, m01 + rhs.m01,
						m10 + rhs.m10, m11 + rhs.m11	);
}

Matrix2x2 &Matrix2x2::operator +=(const Matrix2x2 &rhs)
{
	m00 += rhs.m00;
	m01 += rhs.m01;

	m10 += rhs.m10;
	m11 += rhs.m11;

	return *this;
}

Matrix2x2 Matrix2x2::operator -(const Matrix2x2 &rhs) const
{
	return Matrix2x2(	m00 - rhs.m00, m01 - rhs.m01,
						m10 - rhs.m10, m11 - rhs.m11	);
}


Matrix2x2 Matrix2x2::operator *(Real scalar) const
{
	return Matrix2x2(	scalar * m00, scalar * m01,
						scalar * m10, scalar * m11	);
}

void Matrix2x2::operator *=(Real scalar)
{
	m00 *= scalar;
	m01 *= scalar;

	m10 *= scalar;
	m11 *= scalar;
}

Matrix2x2 Matrix2x2::operator /(Real scalar) const
{
	Real one_scalar = 1.0f / scalar;
	return Matrix2x2(	one_scalar * m00, one_scalar * m01,
						one_scalar * m10, one_scalar * m11	);
}

Matrix2x2 Matrix2x2::operator *(const Matrix2x2 &b) const
{
	return Matrix2x2(m00 * b.m00 + m01 * b.m10, m00 * b.m01 + m01 * b.m11,
					 m10 * b.m00 + m11 * b.m10, m10 * b.m01 + m11 * b.m11);
}

Real Matrix2x2::frobeniusProduct(const Matrix2x2 &rhs) const
{
	return		m00 * rhs.m00
			+	m01 * rhs.m01
			+	m10 * rhs.m10
			+	m11 * rhs.m11;
}

Real Matrix2x2::getDeterminant() const
{
	return m00 * m11 -m10 * m01;
}


void Matrix2x2::getEigendata(Vector2 &eigenvector0, Vector2 &eigenvector1, Real &eigenvalue0, Real &eigenvalue1) const
{
	getEigenvalues(eigenvalue0, eigenvalue1);
	getEigenvectors(eigenvector0, eigenvector1, eigenvalue0, eigenvalue1);
}

void Matrix2x2::getEigenvalues(Real &eigenvalue0, Real &eigenvalue1) const
{
	Real halfTrace	 = 0.5f * getTrace();
	Real determinant = getDeterminant();
	Real difference	 = sqrtr(halfTrace * halfTrace - determinant);

	eigenvalue0 = halfTrace + difference;
	eigenvalue1 = halfTrace - difference;
}

void Matrix2x2::getEigenvectors(Vector2 &eigenvector0, Vector2 &eigenvector1, const Real eigenvalue0, const Real eigenvalue1) const
{
// harvard method
//	if (fabsr(m10) > 10e-10)//Math::EPSILON)
//	{
//		eigenvector0.set(eigenvalue0 - m11, m10);
//		eigenvector0.normalize();

//		eigenvector1.set(eigenvalue1 - m11, m10);
//		eigenvector1.normalize();
//	}
//	else if (fabsr(m01) > 10e-10)// Math::EPSILON)
//	{
//		eigenvector0.set(m01, eigenvalue0 - m00);
//		eigenvector0.normalize();
//		eigenvector1.set(m01, eigenvalue1 - m00);
//		eigenvector1.normalize();
//	}
//	else
//	{
//		eigenvector0.set(1.0f, 0.0f);
//		eigenvector1.set(0.0f, 1.0f);
//	}

	if (fabsr(m01) < EPSILON && fabsr(m11) < EPSILON)
	{
		eigenvector0.set(1.0f, 0.0f);
		eigenvector1.set(0.0f, 1.0f);
		return;
	}

//	Real v[] = {m00 - eigenvalue1, m10, m01, m11 - eigenvalue1 };
//	if (v[0] * v[0] + v[1] * v[1] < v[2] * v[2] + v[3] * v[3])
//		eigenvector0.set(v[0], v[1]);
//	else
//		eigenvector0.set(v[2], v[3]);

	// Wikipedia method based on Cayley-Hamilton theorem
	// https://en.wikipedia.org/wiki/Eigenvalue_algorithm
//	eigenvector0.set(m00 - eigenvalue1, m10);
////	eigenvector0.set(m01, m11 - eigenvalue1);
//	eigenvector1.set(m00 - eigenvalue0, m10);
////	eigenvector1.set(m01, m11 - eigenvalue0);

	eigenvector0.set(m00 - eigenvalue1, m01);
//	eigenvector0.set(m10, m11 - eigenvalue1);
	eigenvector1.set(m00 - eigenvalue0, m01);
//	eigenvector1.set(m10, m11 - eigenvalue0);
}

void Matrix2x2::getRowVector(Vector2 &target, uint32 rowVectorIdx) const
{
	assert(rowVectorIdx >= 0);
	assert(rowVectorIdx <= 1);

	target.x = values[rowVectorIdx][0];
	target.y = values[rowVectorIdx][1];
}

ostream &operator <<(ostream &os, const Matrix2x2 &m)
{
	os << "Matrix 2x2:\n";
	for (unsigned short int i = 0; i < 2; ++i)
	{
		for (unsigned short int j = 0; j < 2; ++j)
			os << m(i, j) << "\t";
		os << "\n";
	}

	return os;
}

