/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Graphics/Camera3D.h"
#include "Graphics/Viewport.h"
#include "Math/Vector2.h"

using namespace Graphics;
using namespace Math;
using namespace Utilities;

Camera3D *Camera3D::msActiveCamera	= NULL;

Camera3D::Camera3D(Real fovY, Real aspectRatio, Real zNear, Real zFar) :
	BaseCamera3D(Matrix4x4(), aspectRatio),
	mFoVY(0.0f),
	mZFar(0.0f),
	mZNear(0.0f)
{
	setProjectionProperties(fovY, aspectRatio, zNear, zFar);
	recalculateViewMatrix();
}

Camera3D::~Camera3D()
{
	if (this == msActiveCamera)
		msActiveCamera = NULL;
}

Matrix3x3 Camera3D::computeInverseProjectionMatrix() const
{
	return Matrix3x3::createInverseProjectionFovRH(mFoVY, mAspectRatio);
}

Matrix3x3 Camera3D::computeHPSToNNRayDirWS(const ImgSize &viewportSize, const bool addPixelCenterOffset) const
{
	// default rendering camera setup
	const Vector2 principalPoint(0.5f, 0.5f);
	const Real sizeOfNDCCube = 2.0f;


	// pixel space -> normalized device space -> view space -> world space
	const Matrix3x3 invViewport = Viewport::computeInverseMatrix(viewportSize, addPixelCenterOffset, principalPoint, sizeOfNDCCube);
	const Matrix3x3 invProj = computeInverseProjectionMatrix();
	const Matrix3x3 invView = computeInverseRotationMatrix();

	return invViewport * invProj * invView;
}

Matrix4x4 Camera3D::computeWorldSpaceToPixelSpaceMatrix(const ImgSize &viewportSize, const bool considerPixelCenterOffset) const
{
	// default rendering camera setup
	const Vector2 principalPoint(0.5f, 0.5f);
	const Real sizeOfNDCCube = 2.0f;
	const Matrix4x4 viewport = Viewport::computeMatrix(viewportSize, considerPixelCenterOffset, principalPoint, sizeOfNDCCube);

	return mView * mProjection * viewport;
}

void Camera3D::setAspectRatio(const Real aspectRatio)
{
	setProjectionProperties(mFoVY, aspectRatio, mZNear, mZFar);
}

void Camera3D::setProjectionProperties(Real fovY, Real aspectRatio, Real zNear, Real zFar)
{
	assert(fovY > 0.0f);
	assert(aspectRatio > 0.0f);
	assert(zNear > 0.0f);
	assert(zFar > zNear);

	mFoVY = fovY;
	mAspectRatio = aspectRatio;
	mZNear = zNear;
	mZFar = zFar;

	mProjection = Matrix4x4::createProjectionFovRHOpenGL(fovY, aspectRatio, zNear, zFar);
}