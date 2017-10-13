/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <sstream>
#include "FrameStatistics.h"

using namespace Graphics;
using namespace std;

FrameStatistics::FrameStatistics(Real measurePeriod) : 
	mStatistics(L""),
	mMeasurePeriod(measurePeriod),
	mFramesThisPeriod(0)
{
}

void FrameStatistics::update()
{
	++mFramesThisPeriod;

	if (!mMeasurePeriod.hasExpired())
		return;

	Real framesPerSecond = mFramesThisPeriod / mMeasurePeriod.getElapsedTime();
	Real millisecondsPerFrame = 1000.0f / framesPerSecond;

	wostringstream os(wostringstream::out);
	os.precision(6);
	os << L"FPS: " << framesPerSecond << 
		L"\nMSPF: " << millisecondsPerFrame << L"\n";
	mStatistics = os.str();

	mFramesThisPeriod = 0;
	mMeasurePeriod.reset();
}