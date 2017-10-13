/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Math/MathHelper.h"
#include "Graphics/BaseCamera3D.h"

using namespace Graphics;
using namespace Math;
using namespace Utilities;

void BaseCamera3D::computeLocalAxes(Vector3 &xAxis, Vector3 &yAxis, Vector3 &zAxis,
	const Vector4 &positionWS, const Vector4 &targetWS, const Vector3 &up)
{
	zAxis.set(positionWS.x - targetWS.x, positionWS.y - targetWS.y, positionWS.z - targetWS.z);
	//zAxis.set(targetWS.x - positionWS.x, targetWS.y - positionWS.y, targetWS.z - positionWS.z);
	zAxis.normalize();

	Vector3 projectedUp = zAxis * (zAxis.x * up.x + zAxis.y * up.y + zAxis.z * up.z);
	yAxis.set(up.x - projectedUp.x, up.y - projectedUp.y, up.z - projectedUp.z);
	yAxis.normalize();

	xAxis = yAxis.crossProduct(zAxis); // right handed frame (z-axis points out of the screen)
}

BaseCamera3D::BaseCamera3D(const Matrix4x4 &projection, const Real aspectRatio) :
	mProjection(projection), mPosition(0.0f, 0.0f, 0.0f, 1.0f),  mAspectRatio(aspectRatio)
{
	assert(aspectRatio > 0.0f);
	recalculateViewMatrix();
}

BaseCamera3D::~BaseCamera3D()
{

}

Matrix4x4 BaseCamera3D::computeInverseViewMatrix() const
{
	// view space -> world space
	Matrix3x3 R = computeInverseRotationMatrix();
	return Matrix4x4(R.m00,			R.m01,			R.m02,			0.0f,
					 R.m10,			R.m11,			R.m12,			0.0f,
					 R.m20,			R.m21,			R.m22,			0.0f,
					 mPosition.x,	mPosition.y,	mPosition.z,	1.0f );
}

Matrix3x3 BaseCamera3D::computeInverseRotationMatrix() const
{
	return Matrix3x3(mView.m00,		mView.m10,		mView.m20,
					 mView.m01,		mView.m11,		mView.m21,
					 mView.m02,		mView.m12,		mView.m22);
}

void BaseCamera3D::lookAt(const Vector3 &positionWS, const Vector3 &targetWS, const Vector3 &upWS)
{
	lookAt(Vector4(positionWS.x, positionWS.y, positionWS.z, 1.0f), Vector4(targetWS.x, targetWS.y, targetWS.z, 1.0f), upWS);
}

void BaseCamera3D::lookAt(const Vector4 &positionWS, const Vector4 &targetWS, const Vector3 &upWS)
{
	assert(positionWS != targetWS);

	mPosition.x = positionWS.x;
	mPosition.y = positionWS.y;
	mPosition.z = positionWS.z;
	mPosition.w = 1.0f;
 
	// create an orthonormal coordinate system for the camera
	Vector3 xAxis, yAxis, zAxis;
	computeLocalAxes(xAxis, yAxis, zAxis, positionWS, targetWS, upWS);

	mOrientation = Math::createQuaternionFromAxes(xAxis, yAxis, zAxis);
	recalculateViewMatrix(xAxis, yAxis, zAxis);
}

void BaseCamera3D::moveForwards(Real distance)
{
	Real movementWS[3];
	mOrientation.rotateVector(movementWS[0], movementWS[1], movementWS[2], 0.0f, 0.0f, -distance);
	
	mPosition.x += movementWS[0];
	mPosition.y += movementWS[1];
	mPosition.z += movementWS[2];
	recalculateViewMatrix();
}

void BaseCamera3D::moveSidewards(Real distance)
{
	Real movementWS[3];
	mOrientation.rotateVector(movementWS[0], movementWS[1], movementWS[2], distance, 0.0f, 0.0f);

	mPosition.x += movementWS[0];
	mPosition.y += movementWS[1];
	mPosition.z += movementWS[2];
	recalculateViewMatrix();
}

void BaseCamera3D::moveUpwards(Real distance)
{
	Real movementWS[3];
	mOrientation.rotateVector(movementWS[0], movementWS[1], movementWS[2], 0.0f, distance, 0.0f);

	mPosition.x += movementWS[0];
	mPosition.y += movementWS[1];
	mPosition.z += movementWS[2];
	recalculateViewMatrix();
}

void BaseCamera3D::pitch(Real deltaAngle)
{
	Quaternion rotation(Vector3(mView.m00, mView.m10, mView.m20), deltaAngle);
	mOrientation *= rotation;

	mOrientation.normalize();
	recalculateViewMatrix();
}

void BaseCamera3D::roll(Real deltaAngle)
{
	Quaternion rotation(Vector3(mView.m02, mView.m12, mView.m22), deltaAngle);
	mOrientation *= rotation;

	mOrientation.normalize();
	recalculateViewMatrix();
}

void BaseCamera3D::yaw(Real deltaAngle, bool fixedYawAxis)
{
	if (fixedYawAxis)
	{
		Quaternion rotation(Vector3(0.0f, 1.0f, 0.0f), deltaAngle);
		mOrientation *= rotation;
	}
	else
	{
		Quaternion rotation(Vector3(mView.m01, mView.m11, mView.m21), deltaAngle);
		mOrientation *= rotation;
	}
	
	mOrientation.normalize();
	recalculateViewMatrix();
}

void BaseCamera3D::recalculateViewMatrix()
{
	Vector3 xAxis(1.0f, 0.0f, 0.0f);
	Vector3 yAxis(0.0f, 1.0f, 0.0f);
	Vector3 zAxis(0.0f, 0.0f, 1.0f);
	mOrientation.rotateVector(xAxis, xAxis);
	mOrientation.rotateVector(yAxis, yAxis);
	mOrientation.rotateVector(zAxis, zAxis);

	recalculateViewMatrix(xAxis, yAxis, zAxis);
}

void BaseCamera3D::recalculateViewMatrix(const Vector3 &rotatedXAxis, const Vector3 &rotatedYAxis, const Vector3 &rotatedZAxis)
{
	// fill the view matrix with the inverse orientation and the negative translation (after inverse rotation = transposed rotation matrix) 
	mView.m00 = rotatedXAxis.x;
	mView.m10 = rotatedXAxis.y;
	mView.m20 = rotatedXAxis.z;
	mView.m30 = -(rotatedXAxis.x * mPosition.x + rotatedXAxis.y * mPosition.y + rotatedXAxis.z * mPosition.z);

	mView.m01 = rotatedYAxis.x;
	mView.m11 = rotatedYAxis.y;
	mView.m21 = rotatedYAxis.z;
	mView.m31 = -(rotatedYAxis.x * mPosition.x + rotatedYAxis.y * mPosition.y + rotatedYAxis.z * mPosition.z);

	mView.m02 = rotatedZAxis.x;
	mView.m12 = rotatedZAxis.y;
	mView.m22 = rotatedZAxis.z;
	mView.m32 = -(rotatedZAxis.x * mPosition.x + rotatedZAxis.y * mPosition.y + rotatedZAxis.z * mPosition.z);

	mView.m03 = mView.m13 = mView.m23 = 0.0f;
	mView.m33 = 1.0f;
}

void BaseCamera3D::setOrientation(const Quaternion &newOrientation)
{
#ifdef _DEBUG
	Real length = newOrientation.getLength();
	assert(fabsr(length - 1.0f) < EPSILON);
#endif // _DEBUG

	mOrientation = newOrientation;
	recalculateViewMatrix();
}

void BaseCamera3D::setOrientation(const Matrix3x3 &newRightHandedAxes)
{
	const Vector3 xAxis(newRightHandedAxes.m00, newRightHandedAxes.m01, newRightHandedAxes.m02);
	const Vector3 yAxis(newRightHandedAxes.m10, newRightHandedAxes.m11, newRightHandedAxes.m12);
	const Vector3 zAxis(newRightHandedAxes.m20, newRightHandedAxes.m21, newRightHandedAxes.m22);

	mOrientation = Math::createQuaternionFromMatrix(newRightHandedAxes);

	recalculateViewMatrix(xAxis, yAxis, zAxis);
}

void BaseCamera3D::setPosition(const Real xWS, const Real yWS, const Real zWS, const Real wWS)
{
	mPosition.set(xWS, yWS, zWS, wWS);
	recalculateViewMatrix();
}

void BaseCamera3D::setProjectionMatrix(const Matrix4x4 &P)
{
	mProjection = P;
}