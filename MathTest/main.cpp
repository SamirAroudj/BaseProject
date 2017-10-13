/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

#include "Platform/DataTypes.h"
#include "Math/MathHelper.h"
#include "Math/Matrix2x2.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"
#include "Math/Polybezier.h"
#include "Math/Quaternion.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

using namespace std;
using namespace Math;

void test(const string name, bool value)
{
	cout << name << ((value) ? ": V":": X") << endl;
}

Real realRandom()
{
	const Real MIN = -10.0f;
	const Real MAX =  10.0f;
	return ((float) rand() / RAND_MAX) * (MAX - MIN) - MIN;
}

void testEigensystemsFor2x2Matrices()
{
	const Real	TOLERANCE = 10e-5f;
	Vector2		v0;
	Vector2		v1;
	Real		s0;
	Real		s1;

	const uint32 numIterations	= 100000;
	uint32		 correctEiVec	= 0;

	for (uint32 i = 0; i < numIterations; ++i)
	{
		Matrix2x2 A;
		A.m00 = realRandom();
		A.m01 = realRandom();
		A.m10 = realRandom();
		A.m11 = realRandom();

		A.getEigendata(v0, v1, s0, s1);

		// check the results
		// correct order
		assert(s0 >= s1);

		// test whether V really contains eigenvectors of A (as row vectors)

		Vector2	testV0 = v0 * A; // eigenvector0 * eigenvalue0
		Vector2	testV1 = v1 * A; // eigenvector1 * eigenvalue1

		testV0.normalize();
		testV1.normalize();
		v0.normalize();
		v1.normalize();

		if (fabsr(s0) < EPSILON || testV0.equals(v0, TOLERANCE) || testV0.equals(-v0, TOLERANCE))
			++correctEiVec;

		if (fabsr(s1) < EPSILON || testV1.equals(v1, TOLERANCE) || testV1.equals(-v1, TOLERANCE))
			++correctEiVec;
	}

	uint32	numEigenvectors = 2 * numIterations;
	Real	correctFractionEiVec		= (Real) correctEiVec / (Real) numEigenvectors;
	cout << "Eigensystem creation for 2x2 matrices\n";
	cout << "Ordering of eigenvalues was always correct.\n";
	cout << "Number of eigenvectors computed: " << numEigenvectors << "\n";
	cout << "Fraction of numerically correct eigenvectors with tolerance " << TOLERANCE << ":\n" << correctFractionEiVec << endl;

	// single manually defined test
	{
		Matrix2x2 testMatrix(4, 3, -2, -3);
		testMatrix.getEigendata(v0, v1, s0, s1);

		Real	wantedS0(3);
		Vector2 wantedV0(-2, -1);
		Real	wantedS1(-2);
		Vector2 wantedV1(1, 3);
		v0.normalize();
		v1.normalize();
		wantedV0.normalize();
		wantedV1.normalize();

		assert(fabsr(s0 - wantedS0) < TOLERANCE);
		assert(fabsr(s1 - wantedS1) < TOLERANCE);
		assert(v0.equals(wantedV0, TOLERANCE) || (-v0).equals(wantedV0, TOLERANCE));
		assert(v1.equals(wantedV1, TOLERANCE) || (-v1).equals(wantedV1, TOLERANCE));
	}
}

void testEigensystemsForSymmetric3x3Matrices()
{
	const Real	 TOLERANCE		= 10e-5f;
	const uint32 numIterations	= 100000;
	Real		 eigenvalues[3];
	uint32		 correctEigenvectorsCount[2] = { 0, 0 };
	uint32		 correctDeterminantsCount[2] = { 0, 0 };

	for (uint32 i = 0; i < numIterations; ++i)
	{
		Matrix3x3 simpleA;
		simpleA.m00 = realRandom();
		simpleA.m11 = realRandom();
		simpleA.m22 = realRandom();

		Matrix3x3::getEigenvaluesFromSymmetric(eigenvalues, simpleA);

		bool correctOrder = true;
		correctOrder &= eigenvalues[0] >= eigenvalues[1];
		correctOrder &= eigenvalues[1] >= eigenvalues[2];
		assert(correctOrder);
	}

	for (uint32 methodIdx = 0; methodIdx < 2; ++methodIdx)
	{
		bool fast = (methodIdx == 0);

		for (uint32 i = 0; i < numIterations; ++i)
		{
			Matrix3x3 A(realRandom(),	realRandom(),	realRandom(),
						0.0f,			realRandom(),	realRandom(),
						0.0f,			0.0f,			realRandom());

			A.m10 = A.m01;
			A.m20 = A.m02;
			A.m21 = A.m12;

			// test correct order of eigenvalues
			Matrix3x3 V = Matrix3x3::createEigenbasisFromSymmetric(eigenvalues, A, fast, 10e-4f);

			// eigenvalues correctly ordered?
			bool correctOrder = true;
			correctOrder &= eigenvalues[0] >= eigenvalues[1];
			correctOrder &= eigenvalues[1] >= eigenvalues[2];
			assert(correctOrder);

			Real absoluteDetV = fabsr(V.getDeterminant());
			if (fabsr(absoluteDetV - 1.0f) < TOLERANCE)
				++correctDeterminantsCount[methodIdx];

			// test whether V really contains eigenvectors of A (as row vectors)
			Matrix3x3	T = V * A;
			Vector3		testV1(T.m00, T.m01, T.m02);
			Vector3		testV2(T.m10, T.m11, T.m12);
			Vector3		testV3(T.m20, T.m21, T.m22);
			testV1 *= 1.0f / eigenvalues[0];
			testV2 *= 1.0f / eigenvalues[1];
			testV3 *= 1.0f / eigenvalues[2];

			Vector3		v1(V.m00, V.m01, V.m02);
			Vector3		v2(V.m10, V.m11, V.m12);
			Vector3		v3(V.m20, V.m21, V.m22);

			Real lengths[3] = { v1.getLength(), v2.getLength(), v3.getLength() };
			for (uint32 i = 0; i < 3; ++i)
				assert(fabsr(lengths[i] - 1.0f) < TOLERANCE);

			if (testV1.equals(v1, TOLERANCE) || testV1.equals(-v1, TOLERANCE))
				++correctEigenvectorsCount[methodIdx];

			if (testV2.equals(v2, TOLERANCE) || testV2.equals(-v2, TOLERANCE))
				++correctEigenvectorsCount[methodIdx];

			if (testV3.equals(v3, TOLERANCE) || testV3.equals(-v3, TOLERANCE))
				++correctEigenvectorsCount[methodIdx];
		}
	}

	uint32	numEigenvectors		= 3 * numIterations;
	Real	correctFractionEiVecFast	= (Real) correctEigenvectorsCount[0] / (Real) numEigenvectors;
	Real	correctFractionEiVec		= (Real) correctEigenvectorsCount[1] / (Real) numEigenvectors;
	Real	correctFractionDetFast		= (Real) correctDeterminantsCount[0] / (Real) numIterations;
	Real	correctFractionDet			= (Real) correctDeterminantsCount[1] / (Real) numIterations;
	cout << "Eigensystem creation for symmetric, real 3x3 matrices\n";
	cout << "Ordering of eigenvalues was always correct.\n";
	cout << "Number of eigenvectors computed: " << numEigenvectors << "\n";
	cout << "Fraction of numerically correct eigenvectors with tolerance " << TOLERANCE << " fast & normal: " <<
		correctFractionEiVecFast << " & " << correctFractionEiVec << endl;
	cout << "Fraction of numerically correct determinants of orthonormal eigenbases (|detV| = 1) with tolerance " << TOLERANCE << " fast & normal:\n" <<
		correctFractionDetFast << " & " << correctFractionDet << endl;
}

void testVector3()
{
	cout<<"\n\n3D vector tests\n\n";

	Vector3 v1, v2, v3, v4, v5, v6, v7, v1n;
	v1 = Vector3(1, 0.5, 1);
	v2 = Vector3(-1, 0.75, -1);
	v3 = Vector3(0, 1.25, 0);
	v4 = Vector3(2, -0.25, 2);
	v5 = Vector3(3, 1.5, 3);
	v6 = Vector3(0.5, 0.25, 0.5);
	v7 = Vector3(-1.25, 0, 1.25);
	v1n = Vector3(-1, -0.5, -1);

	test("Addition", v1 + v2 == v3);
	test("Subtraction", v1 - v2 == v4);

	test("Multiplication by Scalar", v1 * 3 == v5);
	test("Division by Scalar", v1 / 2 == v6);

	v5.normalize();
	test("Normalisation", fabsr(v5.getLength() - 1.0f) < EPSILON);
	test("Negation", -v1 == v1n);

	test("Dot product", fabsr(v1.dotProduct(v1) - 2.25f) < EPSILON);
	test("Cross product", v1.crossProduct(v2)==v7);
}

void testMatrix3x3(){
	cout<<"\n\n3D Matrix tests\n\n";

	static const Real maData[] = 	{
		1,3,2,
		2,1,3,
		3,2,1	
	};

	static const Real mbData[] =	{
		-1, -2, -3,
		2,  3, -1,
		-3,  1,  2
	};

	static const Real mabData[] =	{
		-14, -11, -11,
		5,   7,  12,
		5,  -4,  -1
	};

	static const Real mabInvData[] =	{
		-41, -33,   55,
		-65, -69, -113,
		55, 111,   43
	};

	static const Real mabInvFactor = 1 / 684.0f;

	Matrix3x3 ma = Matrix3x3::createFromMajorColumnArray(maData);
	Matrix3x3 mb = Matrix3x3::createFromMajorColumnArray(mbData);
	Matrix3x3 mab = Matrix3x3::createFromMajorColumnArray(mabData);
	Matrix3x3 mabInv = Matrix3x3::createFromMajorColumnArray(mabInvData);
	mabInv = mabInv * mabInvFactor;

	Real val1 = mb(0, 1);
	Real val2 = mb(1, 2);

	Matrix3x3 rab = ma * mb;
	test("Multiplication",rab == mab);

	test("Determinant",rab.getDeterminant() == -684);

	Matrix3x3 rabInv = Matrix3x3::createInverse(mab);
	test("Inversion",rabInv == mabInv);	


	Matrix3x3 mrx, mry, mrz;
	Vector3 v1, v2, v3;
	Vector3 vx = Vector3(1, 0, 0);
	Vector3 vy = Vector3(0, 1, 0);
	Vector3 vz = Vector3(0, 0, 1);
	//Rotation
	mrx = Matrix3x3::createRotationFromIntrinsicAngles(HALF_PI, 0, 0);
	mry = Matrix3x3::createRotationFromIntrinsicAngles(0, HALF_PI, 0);
	mrz = Matrix3x3::createRotationFromIntrinsicAngles(0, 0, HALF_PI);
	v1 = vy * mrx;
	v2 = vx * mry;
	v3 = vx * mrz;
	test("Rotation",v1 == vz);
	test("Rotation",v2 == -vz);
	test("Rotation",v3 == vy);

	//Rotation2
	mrx = Matrix3x3::createRotationFromExtrinsicAngles(HALF_PI, 0, 0);
	mry = Matrix3x3::createRotationFromExtrinsicAngles(0, HALF_PI, 0);
	mrz = Matrix3x3::createRotationFromExtrinsicAngles(0, 0, HALF_PI);
	v1 = vy * mrx;
	v2 = vx * mry;
	v3 = vx * mrz;
	test("Rotation",v1 == vz);
	test("Rotation",v2 == -vz);
	test("Rotation",v3 == vy);

	Real x, y, z;
	Matrix3x3 mE = Matrix3x3::createRotationFromExtrinsicAngles(HALF_PI, QUARTER_PI, THIRD_PI);
	Matrix3x3 mI = Matrix3x3::createRotationFromIntrinsicAngles(THIRD_PI, QUARTER_PI, HALF_PI);
	mE.getExtrinsicRotationAngles(x, y, z);
	mI.getIntrinsicRotationAngles(x, y, z);

	Vector3 position(1.0f, 1.0f, 1.0f);
	Vector3 translatedPosition(2.0f, 3.0f, 1.0f);
	Matrix3x3 translation;
	translation.addTranslation(Vector2(1.0f, 2.0f));
	position = position * translation;
	test("Translation", position == translatedPosition);
}

void testMatrix4x4(){
	cout<<"\n\nHomogenous 3D vector and matrix tests\n\n";

	Matrix4x4 m1, m2, m3;
	Vector4 vx, vy, vz;
	Vector4 v1, v2, v3;
	Matrix4x4 mrx;
	Matrix4x4 mry;
	Matrix4x4 mrz;
	Vector4 v4;
	Vector3 v5;
	Matrix4x4 mrot;
	Matrix4x4 mtran;
	Vector3 v6;
	Vector4 v7;
	Matrix4x4 mtrans;
	Matrix4x4 minv;
	Matrix4x4 mi1, mi2;
	Matrix4x4 proj;

	vx = Vector4(1, 0, 0, 0);
	vy = Vector4(0, 1, 0, 0);
	vz = Vector4(0, 0, 1, 0);
	v4 = Vector4(-1, 0.56f, 9.3f, 1);
	v5 = Vector3(v4.x, v4.y, v4.z);

	//Multiplication
	m2(1, 3) = 5;
	m2(3, 1) = 7;
	m2(2, 2) = 0;
	m2(0, 2) = 11;
	m2(2, 0) = 13;
	m2(3, 3) = 99;
	m2(3, 0) = -1;
	m2(0, 3) = 1;

	m3 = m1 * m2 * m1;
	test("Multiplication",m3 == m2);

	// single Rotation
	mrx = Matrix4x4::createRotationX(HALF_PI);
	mry = Matrix4x4::createRotationY(HALF_PI);
	mrz = Matrix4x4::createRotationZ(HALF_PI);
	v1 = vy * mrx;
	v2 = vx * mry;
	v3 = vx * mrz;
	test("Rotation",v1 == vz);
	test("Rotation",v2 == -vz);
	test("Rotation",v3 == vy);

	//Inversion
	Real data1[] = {
		1,2,3,4,
		2,3,4,1,
		3,4,1,2,
		4,1,2,3
	};

	Real data2[] =	{
		-9,  1,  1, 11,
		1,  1, 11, -9,
		1, 11, -9,  1,
		11, -9,  1,  1
	};

	mi1 = Matrix4x4::createFromMajorColumnArray(data1);

	Real val1 = mi1(1, 2);
	Real val2 = mi1(3, 0);

	mi2 = Matrix4x4::createInverse(mi1);
	Matrix4x4 imr = Matrix4x4::createFromMajorColumnArray(data2);
	imr = imr * 1/40.0f;

	Matrix4x4 identity;
	Matrix4x4 shouldBeIdentity = mi1 * mi2;

	test("Inversion", identity == shouldBeIdentity);
	test("Inversion", mi2 == imr);

	//OrthoProjection
	proj = Matrix4x4::createOrthoLHDirectX(200, 200, -100, 100);
	Vector4 center(0,0,0,1);
	Vector4 minPoint(-100, -100, -100, 1);
	Vector4 maxPoint(100, 100, 100, 1);

	Vector4 projCenter = center * proj;
	Vector4 projMinPoint = minPoint * proj;
	Vector4 projMaxPoint = maxPoint * proj;

	test("Orthogonal projection",	fabsr(projCenter.x - 0) < EPSILON && 
									fabsr(projCenter.y - 0) < EPSILON && 
									fabsr(projCenter.z - 0.5f) < EPSILON);
	test("Orthogonal projection",	fabsr(projMinPoint.x - -1) < EPSILON && 
									fabsr(projMinPoint.y - -1) < EPSILON && 
									fabsr(projMinPoint.z - 0) < EPSILON);
	test("Orthogonal projection",	fabsr(projMaxPoint.x - 1) < EPSILON && 
									fabsr(projMaxPoint.y - 1) < EPSILON && 
									fabsr(projMaxPoint.z - 1) < EPSILON);

	proj = Matrix4x4::createOrthoRHDirectX(200, 200, -100, 100);
	center = Vector4(0,0,0,1);
	minPoint = Vector4(-100, -100, +100, 1);
	maxPoint = Vector4(100, 100, -100, 1);

	projCenter = center * proj;
	projMinPoint = minPoint * proj;
	projMaxPoint = maxPoint * proj;

	test("Orthogonal projection",	fabsr(projCenter.x - 0) < EPSILON && 
									fabsr(projCenter.y - 0) < EPSILON && 
									fabsr(projCenter.z - 0.5f) < EPSILON);
	test("Orthogonal projection",	fabsr(projMinPoint.x - -1) < EPSILON && 
									fabsr(projMinPoint.y - -1) < EPSILON && 
									fabsr(projMinPoint.z - 0) < EPSILON);
	test("Orthogonal projection",	fabsr(projMaxPoint.x - 1) < EPSILON && 
									fabsr(projMaxPoint.y - 1) < EPSILON && 
									fabsr(projMaxPoint.z - 1) < EPSILON);

	/*proj = Matrix4x4::createOrtho(200, 200, -1, 1);
	Vector4 center(0,0,0,1);
	Vector4 minPoint(-100, -100, 1, 1);
	Vector4 maxPoint(100, 100, -1, 1);

	Vector4 projCenter = proj * center;
	Vector4 projMinPoint = proj * minPoint;
	Vector4 projMaxPoint = proj * maxPoint;

	test("Orthogonal projection", projCenter.x == 0 && projCenter.y == 0 && projCenter.z == 0);
	test("Orthogonal projection", projMinPoint.x == -1 && projMinPoint.y == -1 && projMinPoint.z == -1);
	test("Orthogonal projection", projMaxPoint.x == 1 && projMaxPoint.y == 1 && projMaxPoint.z == 1);*/

	//FrustumProjection
	proj = Matrix4x4::createProjectionRHDirectX(2, 2, 0.1f, 100.0f);
	minPoint = Vector4(0, 0, -0.1f, 1);
	maxPoint = Vector4(0, 0, -100, 1);

	minPoint = minPoint * proj;
	minPoint /= minPoint.w;
	maxPoint = maxPoint * proj;
	maxPoint /= maxPoint.w;

	test("Frustum projection", minPoint.x == 0 && minPoint.y == 0 && fabsr(minPoint.z - 0) < EPSILON);
	test("Frustum projection", maxPoint.x == 0 && maxPoint.y == 0 && fabsr(maxPoint.z - 1) < EPSILON);
	
	//FrustumProjection2
	proj = Matrix4x4::createProjectionLHDirectX(2, 2, 0.1f, 100.0f);
	minPoint = Vector4(0, 0, 0.1f, 1);
	maxPoint = Vector4(0, 0, 100, 1);

	minPoint = minPoint * proj;
	minPoint /= minPoint.w;
	maxPoint = maxPoint * proj;
	maxPoint /= maxPoint.w;

	test("Frustum projection", minPoint.x == 0 && minPoint.y == 0 && fabsr(minPoint.z - 0) < EPSILON);
	test("Frustum projection", maxPoint.x == 0 && maxPoint.y == 0 && fabsr(maxPoint.z - 1) < EPSILON);

	Vector4 position(3.0f, 4.0f, 5.0f, 1.0f);
	Vector3 translation(-4.0f, 6.0f, 2.0f);
	Vector4 translatedPosition(-1.0f, 10.0f, 7.0f, 1.0f);
	Matrix4x4 translationMatrix = Matrix4x4::createTranslation(translation);
	position = position * translationMatrix;
	test("Create translation", position == translatedPosition);

	// test inverse projection with fov, zNear, zFar
	Real fovY				= Math::HALF_PI;
	Real aspectRatio		= 4.0f / 3.0f;
	Real zNear				= 5.0f;
	Real zFar				= 500.0f;
	Matrix4x4 fovProj		= Matrix4x4::createProjectionFovRHDirectX(fovY, aspectRatio, zNear, zFar);
	Matrix3x3 invFovProj	= Matrix3x3::createInverseProjectionFovRH(fovY, aspectRatio);

	//Matrix4x4 identity1 = fovProj * invFovProj;
	//Matrix4x4 identity2 = invFovProj * fovProj;
	Matrix4x4 I;
	//test("test createInverseProjectionFovLHOpenGL", (identity1 == I) && (identity2 == I));
}

void testQuat()
{
	cout<<"\n\nQuaternion tests\n\n";

	Quaternion::createFromIntrinsicAngles(THIRD_PI, THIRD_PI, THIRD_PI);
	Quaternion::createFromIntrinsicAngles(QUARTER_PI, QUARTER_PI, QUARTER_PI);
	Quaternion::createFromIntrinsicAngles(HALF_PI, HALF_PI, HALF_PI);
	Quaternion::createFromIntrinsicAngles(0.1f, 0.2f, 0.3f);
	Quaternion::createFromIntrinsicAngles(2.0f, 3.0f, -0.3f);

	Vector3 v1;
	Quaternion q1;
	Matrix3x3 rot;
	Vector3 w1;
	Vector3 r1;
	Vector4 v2;
	Quaternion q2, q3, q4;
	Matrix4x4 rot2;
	Vector4 w2;
	Vector4 r2;
	Quaternion q5;
	Quaternion q5n;
	Quaternion q5c;

	v1 = Vector3(1, 0, 0);
	r1 = Vector3(0, 0, 1);
	v2 = Vector4(15, 0, 0, 1);
	r2 = Vector4(0, -15, 0, 1);

	q5 = Quaternion(1, 2, 3, 0.5);
	q5n = Quaternion(-1, -2, -3, -0.5);
	q5c = Quaternion(-1, -2, -3, 0.5);

	//RotAxis
	v1 = Vector3(1, 0, 0);
	q1 = Quaternion(Vector3(0, 1, 0), -HALF_PI);
	rot = createMatrix3x3FromQuaternion(q1);
	w1 = v1 * rot;

	test("Axis Rotation", r1 == w1);

	// Transform
	q2 = Quaternion(Vector3(0, 1, 0), PI);
	q3 = Quaternion(Vector3(0, 0, -1), HALF_PI);
	Matrix4x4 m1, m2;
	m1 = createMatrix4x4FromQuaternion(q2);
	m2 = createMatrix4x4FromQuaternion(q3);
	rot2 = m2 * m1;
	w2 = Vector4(1, 0, 0, 1);
	r2 = Vector4(0, -1, 0, 1);
	v2 = w2 * rot2;
	q4 = q3 * q2;
	Vector4 v3 = w2 * createMatrix4x4FromQuaternion(q4);

	test("Transform", r2 == v2);
	test("Transform", r2 == v3);

	Vector3 vq2, vq3;
	Real a2,a3;
	q2.getAxisAndAngle(a2, vq2);
	q3.getAxisAndAngle(a3, vq3);
	test("Axis/Angle", Vector3(0, 1, 0) == vq2 && fabsr(PI-a2) < EPSILON);
	test("Axis/Angle", Vector3(0, 0, -1) == vq3 && fabsr(HALF_PI-a3) < EPSILON);

	//RotEuler
	v2 = Vector4(15, 0, 0, 1);
	r2 = Vector4(0, -15, 0, 1);
	q2 = Quaternion::createFromExtrinsicAngles(0, 0, -HALF_PI);
	rot2 = createMatrix4x4FromQuaternion(q2);
	w2 = v2 * rot2;
	test("Euler Angles", r2 == w2);


	//Negation
	test("Negation",-q5 == q5n);

	//Conjugate
	q5.conjugate();
	test("Conjugate", q5 == q5c);
	q5.conjugate();

	cout<<"\n\nQuaternion to/from matrix conversion seems to be working. \n\n";
	// around X
	for (Real angle = 0.0; angle < TWO_PI; angle += 0.5)
	{
		const Vector3 axis(1.0f, 0.0f, 0.0f);
		Quaternion q1 = Quaternion(axis, angle);
		Matrix4x4 m1 = createMatrix4x4FromQuaternion(q1);
		Quaternion q2 = createQuaternionFromMatrix(m1);
		Quaternion q3 = createQuaternionFromAxes(Vector3(m1.m00, m1.m01, m1.m02), Vector3(m1.m10, m1.m11, m1.m12), Vector3(m1.m20, m1.m21, m1.m22));
		test("from Matrix (X rotation)", q1 == q2 || q1 == -q2);
		test("from Axes (X rotation)", q1 == q3 || q1 == -q3);
	}
	cout<<"\n";
	// around Y
	for (Real angle = 0.0; angle < TWO_PI; angle += 0.5){
		const Vector3 axis(0.0f, 1.0f, 0.0f);
		Quaternion q1 = Quaternion(axis, angle);
		Matrix4x4 m1 = createMatrix4x4FromQuaternion(q1);
		Quaternion q2 = createQuaternionFromMatrix(m1);
		Quaternion q3 = createQuaternionFromAxes(Vector3(m1.m00, m1.m01, m1.m02), Vector3(m1.m10, m1.m11, m1.m12), Vector3(m1.m20, m1.m21, m1.m22));
		test("from Matrix (Y rotation)", q1 == q2 || q1 == -q2);
		test("from Axes (Y rotation)", q1 == q3 || q1 == -q3);
	}
	cout<<"\n";
	// around Z
	for (Real angle = 0.0; angle < TWO_PI; angle += 0.5){
		const Vector3 axis(0.0f, 0.0f, 1.0f);
		Quaternion q1 = Quaternion(axis, angle);
		Matrix4x4 m1 = createMatrix4x4FromQuaternion(q1);
		Quaternion q2 = createQuaternionFromMatrix(m1);
		Quaternion q3 = createQuaternionFromAxes(Vector3(m1.m00, m1.m01, m1.m02), Vector3(m1.m10, m1.m11, m1.m12), Vector3(m1.m20, m1.m21, m1.m22));
		test("from Matrix (Z rotation)", q1==q2 || q1==-q2);
		test("from Axes (Z rotation)", q1 == q3 || q1 == -q3);
	}

	// rotate vectors with quaternions
	Vector3 xAxis(1.0f, 0.0f, 0.0f);
	Vector3 yAxis(0.0f, 1.0f, 0.0f);
	Vector3 zAxis(0.0f, 0.0f, 1.0f);
	Vector3 testVector(0.0f, 1.0f, 0.0f);
	Vector3 result;

	Quaternion qX = Quaternion(xAxis, QUARTER_PI);
	Quaternion qY = Quaternion(yAxis, QUARTER_PI);
	Quaternion qZ = Quaternion(zAxis, QUARTER_PI);

	Vector3 desiredResult(0.0f, 1.0f, 1.0f);
	desiredResult.normalize();
	qX.rotateVector(result, testVector);
	test("quaternion vector rotation", desiredResult == result);

	testVector.x = 1.0f;
	testVector.y = 0.0f;
	testVector.z = 0.0f;
	desiredResult.x = 1.0f;
	desiredResult.y = 0.0f;
	desiredResult.z = -1.0f;
	desiredResult.normalize();
	qY.rotateVector(result, testVector);
	test("quaternion vector rotation", desiredResult == result);

	testVector.x = 1.0f;
	testVector.y = 0.0f;
	testVector.z = 0.0f;
	desiredResult.x = 1.0f;
	desiredResult.y = 1.0f;
	desiredResult.z = 0.0f;
	desiredResult.normalize();
	qZ.rotateVector(result, testVector);
	test("quaternion vector rotation", desiredResult == result);

	cout << "Quaternion output test: ";
	cout << qZ << endl;
}

void testVector4()
{
	Vector4 v1(2.0f, 3.0f, 4.0f, 5.0f);
	Vector4 v2(1.0f, 2.0f, 3.0f, 4.0f);

	Vector4 min = v1.minimum(v2);
	Vector4 max = v2.maximum(v1);

	test("vector4 minimum", min == v2);
	test("vector4 maximum", max == v1);

	cout << endl;
}

void testBezierCurve()
{
	cout << "Bezier curve tests:\n ";
	Vector2 points[] = { Vector2(0.0f, 0.0f), Vector2(0.0f, 1.0f), Vector2(1.0f, 1.0f), Vector2(1.0f, 0.0f) };
	BezierCurve<Vector2> curve(points, 4);

	Vector2 p = curve.evaluate(0.5f);

	test("cubic Bezier curve, evaluate", p == Vector2(0.5f, 0.75f));

	// todo more tests

	cout << endl;
}

void testPolybezier()
{
	cout << "Polybezier test:\n";

	uint32	curveCount	= 2;
	Vector2 points[]	=
	{
		Vector2(0.0f, 0.0f),
		Vector2(0.0f, 1.0f), Vector2(1.0f, 1.0f), Vector2(1.0f, 0.0f),
		Vector2(1.0f, -1.0f), Vector2(0.0f, -1.0f), Vector2(-0.5f, -0.5f),
	};
	uint32	degrees[]	= { 3, 3 };

	Polybezier<Vector2> test(points, degrees, curveCount);
	test.refine();

	// todo more tests

	cout << endl;
}

int main(int argc, char** argv)
{
	testEigensystemsFor2x2Matrices();
	testEigensystemsForSymmetric3x3Matrices();
	testVector4();
	testVector3();
	testMatrix3x3();
	testMatrix4x4();
	testQuat();
	testBezierCurve();
	testPolybezier();
	
	unsigned short int dummy;
	cin >> dummy;

	return 0;
}

