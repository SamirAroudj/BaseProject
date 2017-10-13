/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Color.h"

namespace Graphics
{
	struct PositionColorVertex
	{
		Math::Vector3 mPosition;
		Color mColor;
	};

	struct PosNorTexVertex
	{
		Math::Vector3 mPosition;
		Math::Vector3 mNormal;
		Math::Vector2 mTexCoord;
	};

	struct PositionTexCoordVertex
	{
		Math::Vector3 mPosition;
		Math::Vector2 mTexCoord;
	};
}

#endif // _VERTEX_H_