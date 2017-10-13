/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _GRAPHICS_3D_MAGIC_CONSTANTS_H_
#define _GRAPHICS_3D_MAGIC_CONSTANTS_H_

#include <string>
#include "Math/MathHelper.h"
#include "Graphics3D/Color.h"
#include "Platform/DataTypes.h"

namespace Graphics
{ 
	/// <summary> This is the maximum distance of a pixel that can be stored in the depth buffer. That is, the corresponding pixel is as far away from the viewer as possible. </summary>
	const Real DEPTH_BUFFER_MAX_DEPTH = 1.0f;

	/// <summary> This is the minimum distance of a pixel that can be stored in the depth buffer. That is, the corresponding pixel is as close to the viewer as possible. </summary>
	const Real DEPTH_BUFFER_MIN_DEPTH = 0.0f;

	/// <summary> The stencil value of every pixel will be set to this value at the beginning of each frame. </summary>
	const uint8 STENCIL_BUFFER_CLEAR_VALUE = 0;

	/// <summary> This value is used to compute the frames per second.
	/// 		  The number of drawn frames is counted over the period of FRAME_STATISTICS_MEASURE_PERIOD seconds and devided by this time.
	/// 		  FPS = numOfFrames / FRAME_STATISTICS_MEASURE_PERIOD
	/// 		  See FrameStatistics </summary>
	const Real FRAME_STATISTICS_MEASURE_PERIOD = 1.0f;


	// camera parameters

	/// <summary> Field of view (angle) along the y-axis. </summary>
	const Real FOVY = Math::QUARTER_PI;

	/// <summary> The distance of the camera's near plane to the viewer. </summary>
	const Real Z_NEAR = 1.0f;

	/// <summary> The distance of the camera's far plane to the viewer. </summary>
	const Real Z_FAR = 500.f;


	// standard colours
	
	/// <summary> This color is used to clear the back buffer by filling it entirely with this color. </summary>
	extern const Color BACK_BUFFER_CLEAR_COLOR;

	/// <summary> completely black </summary>
	extern const Color BLACK;

	/// <summary> intensest blue </summary>
	extern const Color BLUE;

	/// <summary> intensest cyan </summary>
	extern const Color CYAN;

	/// <summary> intensest green </summary>
	extern const Color GREEN;

	/// <summary> intensest magenta </summary>
	extern const Color MAGENTA;

	/// <summary> intensest red </summary>
	extern const Color RED;

	/// <summary> intensest yellow </summary>
	extern const Color YELLOW;

	/// <summary> completely white </summary>
	extern const Color WHITE;

	/// <summary> Dark green color could be used for trees / thick grass </summary>
	extern const Color DARK_GREEN;

	/// <summary> Colour of withered grass </summary>
	extern const Color DRY_GRASS;

	/// <summary> colour of a greyish beach </summary>
	extern const Color GREY_BEACH;

	/// <summary> colour of greyish rocks </summary>
	extern const Color ROCK;

	
	// default media assets paths

	/// <summary> The default path to the folder containing material files. </summary>
	extern const char *PATH_TO_MATERIALS;

	/// <summary> The default path to the folder containing mesh files. </summary>
	extern const char *PATH_TO_MESHES;

	/// <summary> The default path to the folder which contains shaders. </summary>
	extern const char *PATH_TO_SHADERS;

	/// <summary> The default path to the folder which contains textures. </summary>
	extern const char *PATH_TO_TEXTURES;
}

namespace GUI
{
	/// <summary> The default face name which is the standard value for the first font to be loaded by the FontManager. E.g. Arial </summary>
	extern const std::wstring DEFAULT_FACE_NAME;

	/// <summary> The default font height of the first font that is loaded by the FontManager. </summary>
	const uint32 DEFAULT_FONT_HEIGHT = 24u;
}

#endif // _GRAPHICS_3D_MAGIC_CONSTANTS_H_