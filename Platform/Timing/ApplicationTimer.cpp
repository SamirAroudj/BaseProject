/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
#include <Windows.h>
#elif _LINUX
#include <ctime>
#endif // _WINDOWS

#include "Platform/Timing/ApplicationTimer.h"
#include "Platform/Utilities/HelperFunctions.h"

using namespace std::chrono;
using namespace Timing;

ApplicationTimer::ApplicationTimer() :
	mDeltaTime(0.0f),
	mAppTime(0.0f), mRealWorldAppTime(0.0f),
	mTimeStopped(false)
{	
	#ifdef _WINDOWS
		startAccurateTimeServices();
	#endif // _WINDOWS

	mLastTimeStamp		= high_resolution_clock::now();
	mMeasurementStart	= mLastTimeStamp;
}

#ifdef _WINDOWS	// request time services which are as accurate as possible (e.g. sleep function really sleeps 16ms and not 20ms)
	void ApplicationTimer::startAccurateTimeServices()
	{
			TIMECAPS timeCaps;
			timeGetDevCaps(&timeCaps, sizeof(timeCaps));
			timeBeginPeriod(timeCaps.wPeriodMin);
	}
#endif // _WINDOWS


ApplicationTimer::~ApplicationTimer()
{
	#ifdef _WINDOWS
		endAccurateTimeServices();
	#endif // _WINDOWS
}

#ifdef _WINDOWS	// reset system time services back to normal
	void ApplicationTimer::endAccurateTimeServices()
	{
			TIMECAPS timeCaps;
			timeGetDevCaps(&timeCaps, sizeof(timeCaps));
			timeEndPeriod(timeCaps.wPeriodMin);
	}
#endif // _WINDOWS

void ApplicationTimer::continueTiming()
{
	assert(mTimeStopped);
	mTimeStopped = false;
}

double ApplicationTimer::getTimeMeasurement(const TimePoint *reference) const
{
	TimePoint now		= high_resolution_clock::now();
	TimePoint previous	= (reference ? *reference : mMeasurementStart);

	return duration_cast<duration<double>>(now - previous).count();
}

void ApplicationTimer::resetTiming()
{
	mAppTime = 0.0f;
}

const TimePoint &ApplicationTimer::startTimeMeasurement()
{
	mMeasurementStart = high_resolution_clock::now();
	return mMeasurementStart;
}
		
void ApplicationTimer::stopTiming()
{
	assert(!mTimeStopped);

	mTimeStopped	= true;
	mDeltaTime		= 0;
}

Real ApplicationTimer::update()
{
	computeDeltaTime();
	mRealWorldAppTime += mDeltaTime;
	
	if (mTimeStopped)
	{
		mDeltaTime = 0.0f;
	}
	else
	{
		// update app time & time periods
		mAppTime += mDeltaTime;
		const uint32 count = getObserverCount();
		for (uint32 i = 0; i < count; ++i)
			mObservers[i]->update(mDeltaTime);
	}

	return mDeltaTime;
}

void ApplicationTimer::computeDeltaTime()
{
	// compute time between last and this frame
	TimePoint now;

	now				= high_resolution_clock::now();
	mDeltaTime		= duration_cast<duration<Real>>(now - mLastTimeStamp).count();
	mLastTimeStamp	= now;

	if (mDeltaTime < 0.0f)
		mDeltaTime = 0.0f;
	if (mDeltaTime > 10.0f)
		mDeltaTime = 10.0f; // todo log this
}
