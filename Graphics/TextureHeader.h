/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _TEXTURE_HEADER_H_
#define _TEXTURE_HEADER_H_

#include "Graphics/Texture.h"
#include "Platform/DataTypes.h"
#include "Platform/Utilities/Size2.h"

namespace Graphics
{
	/// Stores header data of a tiled texture.
	struct TextureHeader
	{
	public:
		/** Initializes the data structure with data that crashes to force manual initialization. */
		TextureHeader() : mWidthHeight(-1), mTileSize(1.0f, 1.0f), mFormat(Texture::FORMAT_NUM_OF)
		{}

	public:
		Utilities::Size2<Real> mTileSize; /// width [0] and height [1] of each tile in the texture, size \in (0, 1]^2
		uint32 mWidthHeight;		/// texture dimension, width = height = 2^x
		Texture::Format	mFormat;	/// pixel structure, see Format enumeration
	};
}

#endif // _TEXTURE_HEADER_H_
