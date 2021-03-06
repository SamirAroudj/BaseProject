/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Graphics/PinholeCamera.h"
#include "Graphics/Viewport.h"

using namespace Graphics;
using namespace Math;
using namespace Utilities;

PinholeCamera::PinholeCamera() :
	BaseCamera3D(Matrix4x4::createProjectionRealWorld(1.0f, 1.0f), 1.0f),
	mPrincipalPoint(0.5f, 0.5f), mDistortion{0.0f, 0.0f}, mFocalLength(1.0f)
{
	lookAt(Vector3(), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
}

PinholeCamera::PinholeCamera(const Quaternion &orientation, const Vector4 &positionWS,
	const Real focalLength, const Vector2 &principalPoint, const Real aspectRatio,
	const Real distortion[2]) :
	BaseCamera3D(Matrix4x4::createProjectionRealWorld(focalLength, aspectRatio), aspectRatio),
	mPrincipalPoint(principalPoint), mFocalLength(focalLength), mDistortion{distortion[0], distortion[1]}
{
	setOrientation(orientation);
	setPosition(positionWS.x, positionWS.y, positionWS.z, positionWS.w);
}

PinholeCamera::~PinholeCamera()
{
}

Matrix3x3 PinholeCamera::computeInverseProjectionMatrix() const
{
	return Matrix3x3::createInverseProjectionRealWorld(mFocalLength, mAspectRatio);
}

Matrix3x3 PinholeCamera::computeHPSToNNRayDirWS(const ImgSize &viewportSize, const bool addPixelCenterOffset) const
{
	// pixel space -> normalized device space -> view space -> world space
	const Matrix3x3 invViewport = computeInverseViewportMatrix(viewportSize, addPixelCenterOffset);
	const Matrix3x3 invProj = computeInverseProjectionMatrix();
	const Matrix3x3 invView = computeInverseRotationMatrix();

	return invViewport * invProj * invView;
}

Matrix3x3 PinholeCamera::computeInverseViewportMatrix(const ImgSize &viewportSize, const bool addPixelCenterOffset) const
{
	const Real sizeOfNDCCube = 1.0f;
	return Viewport::computeInverseMatrix(viewportSize, addPixelCenterOffset, mPrincipalPoint, sizeOfNDCCube);
}

Matrix4x4 PinholeCamera::computeWorldSpaceToPixelSpaceMatrix(const ImgSize &viewportSize, const bool considerPixelCenterOffset) const
{
	const Real sizeOfNDCCube = 1.0f;
	const Matrix4x4 viewport = Viewport::computeMatrix(viewportSize, considerPixelCenterOffset, mPrincipalPoint, sizeOfNDCCube);

	return mView * mProjection * viewport;
}

void PinholeCamera::setAspectRatio(const Real aspectRatio)
{
	assert(aspectRatio > 0.0f);
	mAspectRatio = aspectRatio;
	mProjection = Matrix4x4::createProjectionRealWorld(mFocalLength, mAspectRatio);
}