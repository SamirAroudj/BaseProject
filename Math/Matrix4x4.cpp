/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Math/MathHelper.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

using namespace Math;
using namespace std;

Matrix4x4::Matrix4x4(const Matrix4x4 &rhs) :
	m00(rhs.m00), m01(rhs.m01), m02(rhs.m02), m03(rhs.m03),
	m10(rhs.m10), m11(rhs.m11), m12(rhs.m12), m13(rhs.m13),
	m20(rhs.m20), m21(rhs.m21), m22(rhs.m22), m23(rhs.m23),
	m30(rhs.m30), m31(rhs.m31), m32(rhs.m32), m33(rhs.m33)
{

}

/*Matrix4x4::Matrix4x4(const Real *values) :
	m00(values[0]),		m01(values[1]),		m02(values[2]),		m03(values[3]),
	m10(values[4]),		m11(values[5]),		m12(values[6]),		m13(values[7]),
	m20(values[8]),		m21(values[9]),		m22(values[10]),	m23(values[11]),
	m30(values[12]),	m31(values[13]),	m32(values[14]),	m33(values[15])
{

}*/

Matrix4x4::Matrix4x4(	Real v11, Real v12, Real v13, Real v14,
						Real v21, Real v22, Real v23, Real v24,
						Real v31, Real v32, Real v33, Real v34,
						Real v41, Real v42, Real v43, Real v44) :
	m00(v11), m01(v12), m02(v13), m03(v14),
	m10(v21), m11(v22), m12(v23), m13(v24),
	m20(v31), m21(v32), m22(v33), m23(v34),
	m30(v41), m31(v42), m32(v43), m33(v44)
{

}

inline void Matrix4x4::addTranslation(const Vector3 &translation)
{
	addTranslation(translation.x, translation.y, translation.z);
}

Matrix4x4 Matrix4x4::createFromMajorRowArray(const Real * data)
{
	return Matrix4x4(	data[0],  data[1],  data[2],  data[3],
						data[4],  data[5],  data[6],  data[7],
						data[8],  data[9],  data[10], data[11],
						data[12], data[13], data[14], data[15] );
}

Matrix4x4 Matrix4x4::createFromMajorColumnArray(const Real * data)
{
	return Matrix4x4(	data[0], data[4], data[8],  data[12],
						data[1], data[5], data[9],  data[13],
						data[2], data[6], data[10], data[14],
						data[3], data[7], data[11], data[15] );
}

Matrix4x4 Matrix4x4::createInverse(const Matrix4x4 &m)
{
	Real s0 = computeDeterminant(m.m00, m.m01, m.m10, m.m11);
	Real c5 = computeDeterminant(m.m22, m.m23, m.m32, m.m33);

	Real s1 = computeDeterminant(m.m00, m.m02, m.m10, m.m12);
	Real c4 = computeDeterminant(m.m21, m.m23, m.m31, m.m33);

	Real s2 = computeDeterminant(m.m00, m.m03, m.m10, m.m13);
	Real c3 = computeDeterminant(m.m21, m.m22, m.m31, m.m32);

	Real s3 = computeDeterminant(m.m01, m.m02, m.m11, m.m12);
	Real c2 = computeDeterminant(m.m20, m.m23, m.m30, m.m33);

	Real s4 = computeDeterminant(m.m01, m.m03, m.m11, m.m13);
	Real c1 = computeDeterminant(m.m20, m.m22, m.m30, m.m32);

	Real s5 = computeDeterminant(m.m02, m.m03, m.m12, m.m13);
	Real c0 = computeDeterminant(m.m20, m.m21, m.m30, m.m31);

	Real determinant = (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
	assert(fabsr(determinant) > EPSILON);

	Real oD = 1.0f / determinant;

	return Matrix4x4(
		oD * ( m.m11 * c5 - m.m12 * c4 + m.m13 * c3),
		oD * (-m.m01 * c5 + m.m02 * c4 - m.m03 * c3),
		oD * ( m.m31 * s5 - m.m32 * s4 + m.m33 * s3),
		oD * (-m.m21 * s5 + m.m22 * s4 - m.m23 * s3),
		
		oD * (-m.m10 * c5 + m.m12 * c2 - m.m13 * c1),
		oD * ( m.m00 * c5 - m.m02 * c2 + m.m03 * c1),
		oD * (-m.m30 * s5 + m.m32 * s2 - m.m33 * s1),
		oD * ( m.m20 * s5 - m.m22 * s2 + m.m23 * s1),
		
		oD * ( m.m10 * c4 - m.m11 * c2 + m.m13 * c0),
		oD * (-m.m00 * c4 + m.m01 * c2 - m.m03 * c0),
		oD * ( m.m30 * s4 - m.m31 * s2 + m.m33 * s0),
		oD * (-m.m20 * s4 + m.m21 * s2 - m.m23 * s0),
		
		oD * (-m.m10 * c3 + m.m11 * c1 - m.m12 * c0),
		oD * ( m.m00 * c3 - m.m01 * c1 + m.m02 * c0),
		oD * (-m.m30 * s3 + m.m31 * s1 - m.m32 * s0),
		oD * ( m.m20 * s3 - m.m21 * s1 + m.m22 * s0));
}

Matrix4x4 Matrix4x4::createInverseCameraTransformation(const Vector3 &cx, const Vector3 &cy, const Vector3 &cz, const Vector3 &position)
{
	return Matrix4x4(	
		cx.x,						cy.x,						cz.x,						0.0f,
		cx.y,						cy.y,						cz.y,						0.0f,
		cx.z,						cy.z,						cz.z,						0.0f,
		-position.dotProduct(cx),	-position.dotProduct(cy),	-position.dotProduct(cz),	1.0f);
}

Matrix4x4 Matrix4x4::createOrthoLHDirectX(const Real width, const Real height, const Real zNear, const Real zFar)
{
	Real temp = 1.0f / (zNear - zFar);
	return Matrix4x4(	2.0f / width,		0.0f,			0.0f,			0.0f,
						0.0f,				2.0f / height,	0.0f,			0.0f,
						0.0f,				0.0f,			-temp,			0.0f,
						0.0f,				0.0f,			zNear * temp,	1.0f);
}

Matrix4x4 Matrix4x4::createOrthoRHDirectX(const Real width, const Real height, const Real zNear, const Real zFar)
{
	Real temp = 1.0f / (zNear - zFar);
	return Matrix4x4(	2.0f / width,		0.0f,			0.0f,			0.0f,
						0.0f,				2.0f / height,	0.0f,			0.0f,
						0.0f,				0.0f,			temp,			0.0f,
						0.0f,				0.0f,			zNear * temp,	1.0f);
}

Matrix4x4 Matrix4x4::createProjectionLHDirectX(const Real width, const Real height, const Real zNear, const Real zFar)
{
	Real temp = 1.0f / (zFar - zNear);

	return Matrix4x4(	2.0f * zNear / width,	0.0f,					0.0f,					0.0f,
						0.0f,					2 * zNear / height,		0.0f,					0.0f,
						0.0f,					0.0f,					zFar * temp,			1.0f,
						0.0f,					0.0f,					-zFar * zNear * temp,	0.0f);
}

Matrix4x4 Matrix4x4::createProjectionRHDirectX(const Real width, const Real height, const Real zNear, const Real zFar)
{
	const Real temp = 1.0f / (zNear - zFar);

	return Matrix4x4(	2.0f * zNear / width,	0.0f,					0.0f,					0.0f,
						0.0f,					2 * zNear / height,		0.0f,					0.0f,
						0.0f,					0.0f,					zFar * temp,			-1.0f,
						0.0f,					0.0f,					zFar * zNear * temp,	0.0f);
}

Matrix4x4 Matrix4x4::createProjectionFovLHDirectX(const Real fovY, const Real aspectRatio, const Real zNear, const Real zFar)
{
	const Real f = 1.0f / tanr(fovY * 0.5f);
	const Real temp = 1.0f / (zFar - zNear);
	const Real a = f / aspectRatio;
	return Matrix4x4(a,		0.0f,	0.0f,					0.0f,
					 0.0f,	f,		0.0f,					0.0f,
					 0.0f,	0.0f,	zFar * temp,			1.0f,
					 0.0f,	0.0f,	-zNear * zFar * temp,	0.0f);
}

Matrix4x4 Matrix4x4::createProjectionFovRHDirectX(const Real fovY, const Real aspectRatio, const Real zNear, Real zFar)
{
	const Real f = 1.0f / tanr(fovY * 0.5f);
	const Real temp = 1.0f / (zNear - zFar);
	const Real a = f / aspectRatio;
	return Matrix4x4(a,		0.0f,	0.0f,					 0.0f,
					 0.0f,	f,		0.0f,					 0.0f,
					 0.0f,	0.0f,	zFar * temp,			-1.0f,
					 0.0f,	0.0f,	zNear * zFar * temp,	 0.0f);
}

Matrix4x4 Matrix4x4::createProjectionFovRHOpenGL(const Real fovY, const Real aspectRatio, const Real zNear, Real zFar)
{
	const Real f = 1.0f / tanr(fovY * 0.5f);
	const Real temp = 1.0f / (zNear - zFar);
	const Real a = f / aspectRatio;
	return Matrix4x4(a,		0.0f,	0.0f,						 0.0f,
					 0.0f,	f,		0.0f,						 0.0f,
					 0.0f,	0.0f,	(zNear + zFar) * temp,		-1.0f,
					 0.0f,	0.0f,	2 * zNear * zFar * temp,	 0.0f);
}

Matrix4x4 Matrix4x4::createProjectionRealWorld(const Real f, const Real pixelAspectRatio)
{	
	Real sX, sY;
	getXYProjectionScaleFactors(sX, sY, f, pixelAspectRatio);

	return Matrix4x4(sX,	0.0f,	0.0f,	0.0f,
					 0.0f,	sY,		0.0f,	0.0f,
					 0.0f,	0.0f,	-1.0f,	-1.0f,
					 0.0f,	0.0f,	0.0f,	0.0f);
}

Matrix4x4 Matrix4x4::createRotationX(const Real radian)
{
	const Real cosR = cosr(radian);
	const Real sinR = sinr(radian);
	return Matrix4x4(	1.0f,	0.0f,	0.0f,	0.0f,
						0.0f,	cosR,	sinR,	0.0f,
						0.0f,	-sinR,	cosR,	0.0f,
						0.0f,	0.0f,	0.0f,	1.0f);
}

Matrix4x4 Matrix4x4::createRotationY(const Real radian)
{
	const Real cosR = cosr(radian);
	const Real sinR = sinr(radian);
	return Matrix4x4(	cosR,	0.0f,	-sinR,	0.0f,
						0.0f,	1.0f,	0.0f,	0.0f,
						sinR,	0.0f,	cosR,	0.0f,
						0.0f,	0.0f,	0.0f,	1.0f);
}

Matrix4x4 Matrix4x4::createRotationZ(const Real radian)
{
	const Real cosR = cosr(radian);
	const Real sinR = sinr(radian);
	return Matrix4x4(	cosR,	sinR,	0.0f,	0.0f,
						-sinR,	cosR,	0.0f,	0.0f,
						0.0f,	0.0f,	1.0f,	0.0f,
						0.0f,	0.0f,	0.0f,	1.0f);
}

Matrix4x4 Matrix4x4::createTranslation(const Vector3 &translation)
{
	return Matrix4x4(	1.0f,			0.0f,			0.0f,			0.0f,
						0.0f,			1.0f,			0.0f,			0.0f,
						0.0f,			0.0f,			1.0f,			0.0f,
						translation.x,	translation.y,	translation.z,	1.0f);
}

bool Matrix4x4::equals(const Matrix4x4 &rhs, const Real epsilon) const
{
	return	(epsilon > fabsr(m00 - rhs.m00)) &&
			(epsilon > fabsr(m01 - rhs.m01)) &&
			(epsilon > fabsr(m02 - rhs.m02)) &&
			(epsilon > fabsr(m03 - rhs.m03)) &&

			(epsilon > fabsr(m10 - rhs.m10)) &&
			(epsilon > fabsr(m11 - rhs.m11)) &&
			(epsilon > fabsr(m12 - rhs.m12)) &&
			(epsilon > fabsr(m13 - rhs.m13)) &&

			(epsilon > fabsr(m20 - rhs.m20)) &&
			(epsilon > fabsr(m21 - rhs.m21)) &&
			(epsilon > fabsr(m22 - rhs.m22)) &&
			(epsilon > fabsr(m23 - rhs.m23)) &&
			
			(epsilon > fabsr(m30 - rhs.m30)) &&
			(epsilon > fabsr(m31 - rhs.m31)) &&
			(epsilon > fabsr(m32 - rhs.m32)) &&
			(epsilon > fabsr(m33 - rhs.m33));
}

Real Matrix4x4::frobeniusProduct(const Matrix4x4 &rhs) const
{
	return	m00 * rhs.m00 + m01 * rhs.m01 + m02 * rhs.m02 + m03 * rhs.m03 +
			m10 * rhs.m10 + m11 * rhs.m11 + m12 * rhs.m12 + m13 * rhs.m13 +
			m20 * rhs.m20 + m21 * rhs.m21 + m22 * rhs.m22 + m23 * rhs.m23 +
			m30 * rhs.m30 + m31 * rhs.m31 + m32 * rhs.m32 + m33 * rhs.m33;
}

Real Matrix4x4::getDeterminant() const
{
	return
		  computeDeterminant(m00, m01, m10, m11) * computeDeterminant(m22, m23, m32, m33)
		- computeDeterminant(m00, m02, m10, m12) * computeDeterminant(m21, m23, m31, m33)
		+ computeDeterminant(m00, m03, m10, m13) * computeDeterminant(m21, m22, m31, m32)
		+ computeDeterminant(m01, m02, m11, m12) * computeDeterminant(m20, m23, m30, m33)
		- computeDeterminant(m01, m03, m11, m13) * computeDeterminant(m20, m22, m30, m32)
		+ computeDeterminant(m02, m03, m12, m13) * computeDeterminant(m20, m21, m30, m31);
}

void Matrix4x4::getRowVector(Vector4 &target, uint32 rowVectorIdx) const
{
	assert(rowVectorIdx >= 0);
	assert(rowVectorIdx <= 3);

	target.x = values[rowVectorIdx][0];
	target.y = values[rowVectorIdx][1];
	target.z = values[rowVectorIdx][2];
	target.w = values[rowVectorIdx][3];
}

Matrix4x4 &Matrix4x4::operator =(const Matrix4x4 &rhs)
{
	m00 = rhs.m00;
	m01 = rhs.m01;
	m02 = rhs.m02;
	m03 = rhs.m03;

	m10 = rhs.m10;
	m11 = rhs.m11;
	m12 = rhs.m12;
	m13 = rhs.m13;

	m20 = rhs.m20;
	m21 = rhs.m21;
	m22 = rhs.m22;
	m23 = rhs.m23;

	m30 = rhs.m30;
	m31 = rhs.m31;
	m32 = rhs.m32;
	m33 = rhs.m33;

	return *this;
}

/*Matrix4x4 &Matrix4x4::operator =(const Real *values)
{
	m00 = values[0];
	m01 = values[1];
	m02 = values[2];
	m03 = values[3];

	m10 = values[4];
	m11 = values[5];
	m12 = values[6];
	m13 = values[7];

	m20 = values[8];
	m21 = values[9];
	m22 = values[10];
	m23 = values[11];

	m30 = values[12];
	m31 = values[13];
	m32 = values[14];
	m33 = values[15];
}*/

Matrix4x4 Matrix4x4::operator +(const Matrix4x4 &rhs) const
{
	return Matrix4x4(	m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02, m03 + rhs.m03,
						m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12, m13 + rhs.m13,
						m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22, m23 + rhs.m23,
						m30 + rhs.m30, m31 + rhs.m31, m22 + rhs.m32, m33 + rhs.m23);
}

Matrix4x4 &Matrix4x4::operator +=(const Matrix4x4 &rhs)
{
	m00 += rhs.m00;
	m01 += rhs.m01;
	m02 += rhs.m02;
	m03 += rhs.m03;
	
	m10 += rhs.m10;
	m11 += rhs.m11;
	m12 += rhs.m12;
	m13 += rhs.m13;

	m20 += rhs.m20;
	m21 += rhs.m21;
	m22 += rhs.m22;
	m23 += rhs.m23;

	m30 += rhs.m30;
	m31 += rhs.m31;
	m32 += rhs.m32;
	m33 += rhs.m33;

	return *this;
}

Matrix4x4 Matrix4x4::operator -(const Matrix4x4 &rhs) const
{
	return Matrix4x4(	m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02, m03 - rhs.m03,
						m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12, m13 - rhs.m13,
						m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22, m23 - rhs.m23,
						m30 - rhs.m30, m31 - rhs.m31, m22 - rhs.m32, m33 - rhs.m23);
}

Matrix4x4 Matrix4x4::operator *(Real scalar) const
{
	return Matrix4x4(	scalar * m00, scalar * m01, scalar * m02, scalar * m03,
						scalar * m10, scalar * m11, scalar * m12, scalar * m13,
						scalar * m20, scalar * m21, scalar * m22, scalar * m23,
						scalar * m30, scalar * m31, scalar * m32, scalar * m33);
}

Matrix4x4 Matrix4x4::operator /(Real scalar) const
{
	Real one_scalar = 1.0f / scalar;
	return Matrix4x4(	one_scalar * m00, one_scalar * m01, one_scalar * m02, one_scalar * m03,
						one_scalar * m10, one_scalar * m11, one_scalar * m12, one_scalar * m13,
						one_scalar * m20, one_scalar * m21, one_scalar * m22, one_scalar * m23,
						one_scalar * m30, one_scalar * m31, one_scalar * m32, one_scalar * m33);
}

Matrix4x4 Matrix4x4::operator *(const Matrix4x4 &b) const
{
	return Matrix4x4(	m00 * b.m00 + m01 * b.m10 + m02 * b.m20 + m03 * b.m30,
						m00 * b.m01 + m01 * b.m11 + m02 * b.m21 + m03 * b.m31,
						m00 * b.m02 + m01 * b.m12 + m02 * b.m22 + m03 * b.m32,
						m00 * b.m03 + m01 * b.m13 + m02 * b.m23 + m03 * b.m33,

						m10 * b.m00 + m11 * b.m10 + m12 * b.m20 + m13 * b.m30,
						m10 * b.m01 + m11 * b.m11 + m12 * b.m21 + m13 * b.m31,
						m10 * b.m02 + m11 * b.m12 + m12 * b.m22 + m13 * b.m32,
						m10 * b.m03 + m11 * b.m13 + m12 * b.m23 + m13 * b.m33,

						m20 * b.m00 + m21 * b.m10 + m22 * b.m20 + m23 * b.m30,
						m20 * b.m01 + m21 * b.m11 + m22 * b.m21 + m23 * b.m31,
						m20 * b.m02 + m21 * b.m12 + m22 * b.m22 + m23 * b.m32,
						m20 * b.m03 + m21 * b.m13 + m22 * b.m23 + m23 * b.m33,
						
						m30 * b.m00 + m31 * b.m10 + m32 * b.m20 + m33 * b.m30,
						m30 * b.m01 + m31 * b.m11 + m32 * b.m21 + m33 * b.m31,
						m30 * b.m02 + m31 * b.m12 + m32 * b.m22 + m33 * b.m32,
						m30 * b.m03 + m31 * b.m13 + m32 * b.m23 + m33 * b.m33);
}

inline void Matrix4x4::setColumn(const Math::Vector4 &newColumn, const uint32 columnIdx)
{
	setColumn(newColumn.x, newColumn.y, newColumn.z, newColumn.w, columnIdx);
}

inline void Matrix4x4::setDiagonal(const Math::Vector4 &newDiagonal)
{
	setDiagonal(newDiagonal.x, newDiagonal.y, newDiagonal.z, newDiagonal.w);
}

inline void Matrix4x4::setRow(const Math::Vector4 &newRow, const uint32 rowIdx)
{
	setRow(newRow.x, newRow.y, newRow.z, newRow.w, rowIdx);
}

inline void Matrix4x4::setTranslation(const Math::Vector3 &translation)
{
	setTranslation(translation.x, translation.y, translation.z);
}

void Matrix4x4::transpose()
{
	// m00, m11, m22, m33 don't change
	swap(m01, m10);
	swap(m02, m20);
	swap(m03, m30);
	swap(m12, m21);
	swap(m13, m31);
	swap(m23, m32);
}

ostream &operator <<(ostream &os, const Matrix4x4 &m)
{
	os << "Matrix 4x4:\n";
	for (unsigned short int i = 0; i < 4; ++i)
	{
		for (unsigned short int j = 0; j < 4; ++j)
			os << m(i, j) << "\t";
		os << "\n";
	}

	return os;
}
