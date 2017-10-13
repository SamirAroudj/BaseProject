/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _TIME_MEASUREMENTS_H_
#define _TIME_MEASUREMENTS_H_

#include <string>
#include "Platform/DataTypes.h"

namespace Utilities
{
	/// Represents several time measurements of a specific code area or for a particular purpose.
	/** Represents time measurements of a specific type or for a particular purpose,
		e.g. measuring the performance of a specific function.
		TimeMeasurements must be fed with measured time periods to gather statistics, such as minimum, maximum and average time. */
	class TimeMeasurements
	{
	public:
		/** Creates an object with 0 time measurements in the beginning and dummy values for min, max & average time.
		@param name Set this to short string identifying this object, e.g. use the name of the function to be profiled.
			The name of a TimeMeasurements object is required by conversion to text. */
		TimeMeasurements(const std::string &name);

		/** Adds a single time measurement which updates the statistics of this object.
		@param time Enter the time in seconds that was measured when profiling some code area.  */
		void add(double time);

		/** Returns the average time of all added measurements in seconds.
		@return The returned value is the average time of all added time period measurements. Unit: seconds
		@see add(...) */
		inline double getAverageTime() const;

		/** Returns the maximum time of all added measurements in seconds.
		@return The returned value is the maximum time of all added time period measurements. Unit: seconds
		@see add(...) */
		inline double getMaxTime() const;

		/** Returns the number of time measurents that were added to this TimeMeasurements representation.
		@return The time measurements count w.r.t. the last call to reset() or object reconstruction is returned.
		@see add(...), reset() */
		inline uint32 getCount() const;

		/** Returns the minimum time of all added measurements in seconds.
		@return The returned value is the minimum time of all added time period measurements. Unit: seconds
		@see add(...) */
		inline double getMinTime() const;

		/** Resets the object for support of new measurements. 
			E.g. Measurement count is set to zero.*/
		void reset();
	
		/** Converts the stored statistics into a string.
		@return The returned string contains this object's name, average, minimum, maximum of added time periods and the number of measurings. */
		std::string toString() const;

	private:
		std::string mName;		/// A name for conversion to text.

		double mSummedTimes;		/// This is the sum of all added time periods.
		double mMaxTime;		/// Contains the maximum of all added time periods.
		double mMinTime;		/// Contains the minimum of all added time periods.
		uint32 mCount;			/// Contains the number of measurements that were added and summed in mSummedTimes.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline double TimeMeasurements::getAverageTime() const
	{
		return mSummedTimes / mCount;
	}

	inline double TimeMeasurements::getMaxTime() const
	{
		return mMaxTime;
	}

	inline uint32 TimeMeasurements::getCount() const
	{
		return mCount;
	}

	inline double TimeMeasurements::getMinTime() const
	{
		return mMinTime;
	}
}

#endif // _TIME_MEASUREMENTS_H_


