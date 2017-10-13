/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _IRENDERABLE_H_
#define _IRENDERABLE_H_

#include "Math/Matrix4x4.h"

class IRenderable
{
public:
	virtual ~IRenderable() { }
	virtual void render(const Math::Matrix4x4 &worldViewProjection, const Math::Matrix4x4 &world) = 0;
};

#endif // _IRENDERABLE_H_
