/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "FrameRateCalculator.h"

using namespace Platform;

void FrameRateCalculator::update()
{
	// time has run out
	if (!mFrameRatePeriod.hasExpired())
		return;

	// measure time & start new measurement
	mFPS				= mFramesThisPeriod / mFrameRatePeriod.getElapsedTime();
	mFramesThisPeriod	= 0;
	mFrameRatePeriod.reset();
}
