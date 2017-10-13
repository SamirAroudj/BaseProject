/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _GRAPHICS_MAGIC_CONSTANTS_H_
#define _GRAPHICS_MAGIC_CONSTANTS_H_

#include "Color.h"
#include "Platform/DataTypes.h"

namespace Graphics
{
	/// The distance of the camera's near plane to the viewer.
	const Real ORTHO_PROJECTION_Z_NEAR = 0.0f;

	/// The distance of the camera's far plane to the viewer.
	const Real ORTHO_PROJECTION_Z_FAR = 100.f;

	/// Coordinates for the bottom clipping plane. (independent of viewport's or window's aspect ratio)
	const Real ORTHO_PROJECTION_BOTTOM = -1.0f;

	/// Coordinates for the left clipping plane. (independent of viewport's or window's aspect ratio)
	const Real ORTHO_PROJECTION_LEFT = -1.0f;

	/// Coordinates for the right clipping plane. (independent of viewport's or window's aspect ratio)
	const Real ORTHO_PROJECTION_RIGHT = 1.0f;

	/// Coordinates for the top clipping plane. (independent of viewport's or window's aspect ratio)
	const Real ORTHO_PROJECTION_TOP = 1.0f;

	/// This is the default projection scale (~= zoom) factor. Higher values mean that more things are visible and things are rendered smaller.
	const Real ORTHO_PROJECTION_DEFAULT_SCALE = 6.0f;

	/// Path to the location where textures are stored.
	extern const char *PATH_TO_TEXTURES;

	/// Default width of a sprite object. It is used to create sprite quads.
	const Real SPRITE_WIDTH = 1.0f;

	/// Default height of a sprite object. It is used to create sprite quads.
	const Real SPRITE_HEIGHT = 1.0f;
	
	/// Defines the maximum width and height of textures to be at max 2^TEXTURE_MAX_EXPONENT.
	const uint32 TEXTURE_MAX_EXPONENT = 13;

	/// Color which is used to clear the back buffer.
	extern const Color BACK_BUFFER_CLEAR_COLOR;

	/// Theses are the first characters in each tile texture.
	const uint32 TILE_TEXTURE_START_CHAR_COUNT = 13;
	const char TILE_TEXTURE_START_CHARS[TILE_TEXTURE_START_CHAR_COUNT] = "Tile Texture";
}

#endif // _GRAPHICS_MAGIC_CONSTANTS_H_
