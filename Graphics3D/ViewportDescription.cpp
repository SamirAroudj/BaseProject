/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "ViewportDescription.h"

using namespace Graphics;


ViewportDescription::ViewportDescription(uint32 topLeftX, uint32 topLeftY, uint32 width, uint32 height, Real maxDepth, Real minDepth) :
	mMaxDepth(maxDepth),
	mMinDepth(minDepth),
	mHeight(height),
	mTopLeftX(topLeftX),
	mTopLeftY(topLeftY),
	mWidth(width)
{ }