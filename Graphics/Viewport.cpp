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

#include "Graphics/Viewport.h"
#include "Math/Vector2.h"
#include "Platform/Window.h"

using namespace Graphics;
using namespace Math;
using namespace Platform;
using namespace Utilities;

const Viewport *Viewport::sActiveViewport = NULL;
const Real Viewport::MAX_DEPTH = 1.0f;

Matrix3x3 Viewport::computeInverseMatrix(const ImgSize &size, const bool considerPixelCenterOffset, const Vector2 &principalPoint, const Real sizeOfNDCCube)
{
	Matrix3x3 invM;

	// map ([0, viewportSize[0]] x [0, viewportSize[1]]) to (sizeOfNDCCube * [-pp.x, 1 - pp.x] x [-pp.y, 1 - pp.y]) 
	invM.m00 = sizeOfNDCCube / size[0];
	invM.m20 = -sizeOfNDCCube * principalPoint.x;

	invM.m11 = -sizeOfNDCCube / size[1];
	invM.m21 = sizeOfNDCCube * principalPoint.y;

	invM.m22 = 1.0f;

	// add pixel center offset?
	if (considerPixelCenterOffset)
	{
		invM.m20 += 0.5f * invM.m00;
		invM.m21 += 0.5f * invM.m11;
	}

	return invM;
}

Matrix4x4 Viewport::computeMatrix(const ImgSize &size, const bool considerPixelCenterOffset, const Vector2 &principalPoint, const Real sizeOfNDCCube)
{
	const Real invSize = 1.0f / sizeOfNDCCube;
	Matrix4x4 M;

	// map (sizeOfNDCCube * [-0.5, 0.5]) to (viewPortSize * [-0.5, 0.5] + pp * viewPortSize)
	M.m00 = invSize *size[0];
	M.m30 = principalPoint.x * size[0];

	M.m11 = -invSize * size[1];
	M.m31 = principalPoint.y * size[1];

	M.m22 = MAX_DEPTH / 2;
	M.m32 = MAX_DEPTH / 2;

	M.m33 = 1.0f;

	// do the NDC coordinates refer to pixel centers?
	if (considerPixelCenterOffset)
	{
		M.m30 -= 0.5f;
		M.m31 -= 0.5f;
	}

	return M;
}

Vector2 Viewport::transformNDCToPC(const Vector2 &pNDC, const ImgSize &resolution, const bool addPixelCenterOffset)
{
	const Vector2 principalPoint(0.5f, 0.5f);
	return transformNDCToPC(pNDC, resolution, addPixelCenterOffset, principalPoint, 2.0f);
}

Vector2 Viewport::transformNDCToPC(const Vector2 &pNDC, const ImgSize &resolution,
	const bool addPixelCenterOffset, const Vector2 &principalPoint, const Real sizeOfNDCCube)
{
	const Vector2 temp = Vector2(pNDC.x, -pNDC.y) / sizeOfNDCCube + principalPoint;
	Vector2 pPC(temp.x * resolution[0],
				temp.y * resolution[1]);

	if (addPixelCenterOffset)
	{
		pPC.x += 0.5f;
		pPC.y += 0.5f;
	}

	return pPC;
}

Viewport::Viewport() : mSize(Platform::Window::getSingleton().getSize())
{
	mPosition[0] = 0;
	mPosition[1] = 0;
}

Viewport::Viewport(const uint32 position[2], const ImgSize &size) :
	mSize(size)
{
	mPosition[0] = position[0];
	mPosition[1] = position[1];
}


Viewport::~Viewport()
{
	if (this == sActiveViewport)
		sActiveViewport = NULL;
}

void Viewport::becomeActiveViewport() const
{
	// set this view port as the currently active OpenGL view port
	sActiveViewport = this;

	#ifdef _WINDOWS
		// this y-coordinate correction value must be added since glViewport does not work correctly
		// that is the case if full screen is true and if current display dimensions are not equal to maximum screen size
		Platform::Window	&window		= Platform::Window::getSingleton();
		int32				yCorrection	= window.isFullScreen() ? window.getScreenSize()[1] - window.getSize()[1] : 0;
	#else
		int32 yCorrection = 0;
	#endif // WINDOWS

	glViewport(mPosition[0], mPosition[1] + yCorrection,  mSize[0], mSize[1]);
}

void Viewport::set(const uint32 position[2], const ImgSize &size)
{
	// set data
	mPosition[0] = position[0];
	mPosition[1] = position[1];
	mSize = size;

	// update OpenGL
	if (this == sActiveViewport)
		becomeActiveViewport();
}