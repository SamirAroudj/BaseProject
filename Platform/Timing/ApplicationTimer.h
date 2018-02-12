/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>
#include "Math/MathCore.h"
#include "Patterns/Singleton.h"
#include "Patterns/Subject.h"
#include "Platform/Timing/TimePeriod.h"

namespace Timing
{
	/** Only high resolution clock time measurements are done which is why the name can be shortened. */
	typedef std::chrono::high_resolution_clock::time_point TimePoint;

	/// This class provides functionality to measure elapsed time.
	/** The timer class can be used to obtain the elapsed time which was measured between two update calls.*/
	class ApplicationTimer : public Patterns::Singleton<ApplicationTimer>, public Patterns::Subject<TimePeriod>
	{
		friend class TimePeriod;

	public:
		/** Creates and initialises the timer. */
		ApplicationTimer();

		/** Checks in debug mode weather there are no registered time periods anymore. */
		virtual ~ApplicationTimer();

		/** Continues time increase of application time after it was stopped by stopTiming().*/
		void continueTiming();

		/** Obtains the time that has elapsed since the last call to resetTiming() without counting paused time.
		@returns Returns the time elapsed since the last call to resetTiming() whitout counting paused time. The returned value is in seconds.*/
		Real getAppTime() const;

		/** Obtains the real world time that has elapsed since program start.
		So it includes the time that elapsed when the timing was paused and it is not affected by any resets.
		@return Returns the real world time that has elapsed since program start. Paused time is inclusive, resets are ignored.*/
		Real getRealWorldAppTime() const;

		/** Get the time difference between the last two invocations of the update function.
		@returns The returned value is the measured time difference between the last two update calls in seconds. */
		inline Real getDeltaTime() const;

		/** Returns the elapsed time in seconds since the last startTimeMeasurement() call.
			Ignores stopping of timing and is designed for profiling of performance.
		@param reference Elapsed time is measured between now and the entered reference time point or the time point of the most recent call to startTimeMeasurement().
		@return Returns the elapsed time in seconds since the last startTimeMeasurement() call.
			Ignores stopping of timing and is designed for profiling of performance.
		@see startTimeMeasurement() */
		double getTimeMeasurement(const TimePoint *reference) const;

		/** Check weather the timer is currently active.
		@returns Returns true if the timing is stopped. That is the application time is not increased and delta time between two updates is 0. */
		inline bool isTimingStopped() const;

		/** Reset the application time which means that is set to 0.
			The frame of reference for the application time is then the time when this function was called. */
		void resetTiming();

		/** Starts accurate measurement of elapsing time.
			Ignores stopping of timing and is designed for profiling of performance.
		@return The current point in time measured during this call is returned. */
		const TimePoint &startTimeMeasurement();

		/** Stops the increase of application time. */
		void stopTiming();

		/** Invoke this function regularly to update the deltaTime of the timer. For example you can call this function at the beginning of a game loop.
		@returns Returns the time difference between this and the last update call in seconds. */
		Real update();

	private:
		/** Copy constructor is forbidden.
		@param rhs Don't call it, it fails.*/
		ApplicationTimer(const ApplicationTimer &rhs);

		/** Assignment operator is forbidden.
		@param rhs Don't call it, it fails.
		@return Don't call it, it fails. */
		ApplicationTimer &operator =(const ApplicationTimer &rhs);

		/** Computes mDeltaTime in seconds by means of a system time query*/
		void computeDeltaTime();

		#ifdef _WINDOWS
			/** Set the resolution of the system timer back to its original value. */
			void endAccurateTimeServices();
		#endif // _WINDOWS

		#ifdef _WINDOWS
			/** Request an accurate system timer resolution to cap the frame rate accurately. */
			void startAccurateTimeServices();
		#endif // _WINDOWS

	private:
		TimePoint mLastTimeStamp;		/// Contains ameasured high resolution time point of last frame of high performance std clock.
		TimePoint mMeasurementStart;	/// Contains measured high resolution time point of last call of startTimeMeasurement for performance profiling.

		Real mAppTime;					/// Stores the time elapsed since the timer was constructed or since last reset without counting time while being stopped.
		Real mDeltaTime;				/// Contains the elapsed time between the last two update invocations in seconds.
		Real mRealWorldAppTime;			/// Contains the real world time in seconds that has elapsed since program start ignoring resets or pausing
		bool mTimeStopped;				/// Stores weather the application timing is paused / stopped
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline Real ApplicationTimer::getAppTime() const
	{
		return mAppTime;
	}

	inline Real ApplicationTimer::getRealWorldAppTime() const
	{
		return mRealWorldAppTime;
	}

	inline Real ApplicationTimer::getDeltaTime() const
	{
		return mDeltaTime;
	}

	inline bool ApplicationTimer::isTimingStopped() const
	{
		return mTimeStopped;
	}


	inline ApplicationTimer::ApplicationTimer(const ApplicationTimer &rhs)
	{
		assert(false);
	}

	inline ApplicationTimer &ApplicationTimer::operator =(const ApplicationTimer &rhs)
	{
		assert(false);
		return *this;
	}

}

#endif // _TIMER_H_
