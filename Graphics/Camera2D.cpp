/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
#include <Windows.h>
#endif // _WINDOWS

#include <GL/gl.h>
#include "Graphics/Camera2D.h"

using namespace Graphics;
using namespace Math;
using namespace Platform;

const Camera2D *Camera2D::msActiveCamera = NULL;

Camera2D::Camera2D(const Vector2 &position, Real angle, Real projectionScale) :
	mPosition(position),
	mRight(1.0f, 0.0f),
	mUp(0.0f, 1.0f),
	mAngle(angle),
	mFrustumScale(projectionScale)
{
	assert(projectionScale > 0.0f);

	if (!msActiveCamera)
		msActiveCamera = this;
}

Camera2D::~Camera2D()
{
	if (this == msActiveCamera)
		msActiveCamera = NULL;
}

void Camera2D::becomeActiveCamera() const
{
	msActiveCamera = this;

	// set up an orthogonal projection
	Real aspectRatio = mViewport.getAspectRatio();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	glOrtho(ORTHO_PROJECTION_LEFT   * mFrustumScale * aspectRatio,
			ORTHO_PROJECTION_RIGHT  * mFrustumScale * aspectRatio,
			ORTHO_PROJECTION_BOTTOM * mFrustumScale,
			ORTHO_PROJECTION_TOP    * mFrustumScale,
			-ORTHO_PROJECTION_Z_NEAR, -ORTHO_PROJECTION_Z_FAR);

	// use mViewport
	mViewport.becomeActiveViewport();
}

Matrix4x4 Camera2D::computeViewMatrix(const Vector2 &position, const Vector2 & right, const Vector2 &up)
{
	// return (R T)^-1 = T^-1 R^-1 = V^-1
	Real m41 = -right.x * position.x - right.y * position.y;
	Real m42 = -up.x * position.x - up.y * position.y;

	return Matrix4x4(right.x,	up.x,	0.0f,	0.0f,
					 right.y,	up.y,	0.0f,	0.0f,
					 0.0f,		0.0f,	1.0f,	0.0f,
					 m41,		m42,	0.0f,	1.0f);
}

Vector2 Camera2D::convertViewSpaceToWorldSpace(const Vector2 &viewPoint) const
{
	Vector2 worldPoint(viewPoint);

	worldPoint.rotate(mAngle);
	worldPoint += mPosition;

	return worldPoint;
}

void Camera2D::moveSidewards(Real distance)
{
	mPosition.x += distance * mRight.x;
	mPosition.y += distance * mRight.y;
}

void Camera2D::moveUpwards(Real distance)
{
	mPosition.x += distance * mUp.x;
	mPosition.y += distance * mUp.y;
}

void Camera2D::setAngle(Real angle)
{
	mAngle = angle;														// set and normalize angle
	if (mAngle > PI)
		mAngle -= TWO_PI;
	else if (mAngle < -PI)
		mAngle += TWO_PI;

	Real cosA = cosr(mAngle);											// compute mRight and mUp
	Real sinA = sinr(mAngle);

	mRight.x = 1.0f * cosA;
	mRight.y = 1.0f * sinA;
	mUp.x = -1.0f * sinA;
	mUp.y = 1.0f * cosA;
}
