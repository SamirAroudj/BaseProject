/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _INPUT_MAGIC_CONSTANTS_H_
#define _INPUT_MAGIC_CONSTANTS_H_

#include "Platform/DataTypes.h"

namespace Input
{
	/** Maximum length of the text that a user can enter at once by means of the TextInput class.
		Real text can be one character longer since '\0' is appended. */
	const uint32 MAX_TEXT_INPUT_LENGTH = 500;

	/** The maximum number of keyboard keys supported. */
	const uint32 MAX_NUM_OF_KEYBOARD_KEYS = 256;

	#ifdef _WINDOWS
		/** The inverse of the maximum relative mouse motion is used to normalize and cap relative mouse motions retrieved from the mouse device for windows. */
		const float ONE_MAX_RELATIVE_MOUSE_MOTION = 1.0f / 180.0f;

		/** The default scale factor defines how the relative mouse motions are added to the absolute mouse position.
			Should be smaller than 1.0f and greater than zero. */
		const float MOUSE_REALTIVE_TO_ABSOLUTE_SCALE = 0.15f;
	#endif // _WINDOWS

	#ifdef _LINUX
		/** A single xlib mouse wheel event results in addition of this constant to the relative mouse motion along the x-axis during the corresponding frame. */
		const Real MOUSE_RELATIVE_Z_MOTION_PER_EVENT = 0.1f;
	#endif // _LINUX

	// game controller constants

	/** This is the maximum number of game controller buttons that are supported. */
	const uint32 CONTROLLER_BUTTON_COUNT = 32;

	/** This is the maximum number of controllers that can be used. */
	const uint32 CONTROLLER_MAX_COUNT = 4;

	/** This is the number of axes that are ranged and have a dead zone. lx, ly, z, rx, ry */
	const uint32 CONTROLLER_RANGED_AXES = 5;

	/** This is the maximum number of supported controller axes. lx, ly, z, rx, ry, povx, povy */
	const uint32 CONTROLLER_AXES = 7;

	/** This value indicates that the entire axis of a game controller is dead.
		It is used to compute axis dead zone range on a percentage basis. */
	const uint32 CONTROLLER_DEAD_ZONE_MAXIMUM = 10000;

	/** It is assumed that each game controller axis has a dead zone. This constant defines the assumed minimum dead range for an axis if
		the device does not report its dead zone size or if it reports a dead zone size of zero. */
	const Real CONTROLLER_DEAD_ZONE_MINIMUM = 0.05f;
}

#endif // _INPUT_MAGIC_CONSTANTS_H_
