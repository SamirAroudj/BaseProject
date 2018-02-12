/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Platform/Timing/ApplicationTimer.h"
#include "Platform/Timing/TimePeriod.h"

using namespace Timing;

TimePeriod::TimePeriod(Real period) : 
	mElapsedTime(0.0f), mTimePeriod(period), mStopped(false)
{
	ApplicationTimer::getSingleton().registerObserver(this);	
}


TimePeriod::TimePeriod(const TimePeriod &copy) :
	mElapsedTime(copy.mElapsedTime), mTimePeriod(copy.mTimePeriod), mStopped(copy.mStopped)
{
	ApplicationTimer::getSingleton().registerObserver(this);
}

TimePeriod::~TimePeriod()
{
	ApplicationTimer::getSingleton().deregisterObserver(this);
}


TimePeriod &TimePeriod::operator =(const TimePeriod &rhs)
{
	mElapsedTime = rhs.mElapsedTime;
	mTimePeriod = rhs.mTimePeriod;
	mStopped = rhs.mStopped;
	return *this;
}

void TimePeriod::continueTiming()
{
	assert(mStopped);
	mStopped = false;
}

void TimePeriod::decreasePeriod(Real delta)
{
	assert(delta > 0.0f);
	assert(mTimePeriod > delta);
	mTimePeriod -= delta;
}

void TimePeriod::increasePeriod(Real delta)
{
	assert(delta > 0.0f);
	mTimePeriod += delta;
}

void TimePeriod::reset()
{
	mElapsedTime = 0.0f;
	mStopped = false;
}

void TimePeriod::reset(Real period)
{
	mTimePeriod = period;
	mElapsedTime = 0.0f;
	mStopped = false;
}

void TimePeriod::stopTiming()
{
	assert(!mStopped);
	mStopped = true;
}

void TimePeriod::update(Real deltaTime)
{
	if (mStopped)
		return;

	mElapsedTime += deltaTime;
}

