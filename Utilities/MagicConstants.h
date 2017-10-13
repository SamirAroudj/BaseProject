/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _UTILITIES_MAGIC_CONSTANTS_H_
#define _UTILITIES_MAGIC_CONSTANTS_H_

#include "Platform/DataTypes.h"

namespace Utilities
{
	/** This seed is used by RandomManager for deterministic pseudo random number generation. */
	const uint32 RANDOM_SEED = 42;

	/** Defines the maximum text length for the toString function of the class TimeMeasurements. */
	const uint32 TIME_MEASUREMENTS_MAX_TEXT_LENGTH = 1000;
}

#endif // _UTILITIES_MAGIC_CONSTANTS_H_
