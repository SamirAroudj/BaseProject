/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _PLATFORM_MAGIC_CONSTANTS_H_
#define _PLATFORM_MAGIC_CONSTANTS_H_

#include "Platform/DataTypes.h"

namespace Platform
{
	/** Defines the root folder where all the data for the application is stored. */
	#define DATA_DIRECTORY "Data/"

    /** The default path and name of the configuration file to load and apply */
	extern const char *DEFAULT_CONFIG_FILE;

    /** This is the name and path of the file that stores log messages. */
	extern const char *LOG_FILE_NAME;

    /** Standard width of window's client area in pixel if no configuration file is used. */
	extern const uint32 WINDOW_DEFAULT_WIDTH;

    /** Standard height of window's client area in pixel if no configuration file is used. */
	extern const uint32 WINDOW_DEFAULT_HEIGHT;

    /** Standard number of bits per pixel which are used to store depth (z-distance) values of pixels. */
	extern const uint16 WINDOW_DEFAULT_DEPTH_BITS_PER_PIXEL;

    /** Standard number of bits per pixel which are used to store stencil information. */
	extern const uint16 WINDOW_DEFAULT_STENCIL_BITS_PER_PIXEL;
}

#endif // _PLATFORM_MAGIC_CONSTANTS_H_
