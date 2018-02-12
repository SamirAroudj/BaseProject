/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Platform/Profiling/TimeMeasurements.h"

using namespace Profiling;
using namespace std;

TimeMeasurements::TimeMeasurements(const string &name) :
	mName(name), mSummedTimes(0.0f),
	mMaxTime(REAL_MIN), mMinTime(REAL_MAX), mCount(0)
{
	reset();
}

void TimeMeasurements::add(double time)
{
	mSummedTimes += time;
	++mCount;

	if (time > mMaxTime)
		mMaxTime = time;

	if (time < mMinTime)
		mMinTime = time;
}

void TimeMeasurements::reset()
{
	mSummedTimes	= 0.0f;
	mMaxTime		= REAL_MIN;
	mMinTime		= REAL_MAX;
	mCount			= 0;
}

string TimeMeasurements::toString() const
{
	double average = mSummedTimes / mCount;
	char buffer[TIME_MEASUREMENTS_MAX_TEXT_LENGTH];

	snprintf(buffer, TIME_MEASUREMENTS_MAX_TEXT_LENGTH, "%s %f %f %f %u",
		mName.c_str(), average, mMinTime, mMaxTime, mCount);

	return string(buffer);
}
