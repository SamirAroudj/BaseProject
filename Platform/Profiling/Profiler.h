/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _PROFILER_H_
#define _PROFILER_H_

#include <vector>
#include "Patterns/Singleton.h"
#include "Platform/DataTypes.h"
#include "Platform/Profiling/TimeMeasurements.h"
#include "Platform/Timing/ApplicationTimer.h"

namespace Profiling
{
	/// Realizes typical profiling functionality for performance evaluation.
	/** The Profiler class realizes helper functions for easy profiling, such as runtime measurements. */
	class Profiler : public Patterns::Singleton<Profiler>
	{
	public:
		/** Creates the profiler and allocates time measurement representations if wanted.
			All created TimeMeasurement Objects are ordered in the same order as their names in measurementNames,
			e.g. object with names[0] is identified by its index 0.
		@param measurementNames Set this to an array of names for description of the measurement statistics to be created.
		@param numberOfMeasurementNames Set this to the element count of measurementNames which is equal to the number of
			TimeMeasurement representions to be created. */
		Profiler(const std::string *measurementNames, uint32 numberOfMeasurementNames);

		/** Frees resources. */
		~Profiler();

		/** Adds a new TimeMeasurements object.
		@param name This name describes the TimeMeasurements object.
		@return Returns the index of the new TimeMeasurements object which is necesary to access it. */
		uint32 addMeasurementType(const std::string &name);

		/** Adds a single time measurement, timePeriod, to the TimeMeasurements object identified by index.
		@param index This identifies the TimeMeasurements object to which the time period is added. Objects order equals their creation order.
		@param timePeriod Set this to the measured time period in seconds. */
		void addTimeMeasurement(uint32 index, double timePeriod);

		/** Ends the measurement of a time period fo the TimeMeasurements object identified by index.
			Time period measurement must be called for the same object (= index) before this call to start the measurement.
		@param index This identifies the TimeMeasurements object. Objects order equals their creation order.
		@see startTimeMeasurement(...) */
		void endTimeMeasurement(uint32 index);

		/** Provides access to all TimeMeasurements objects.
		@param index This identifies the TimeMeasurements object. Objects order equals their creation order.
		@return Returns the TimeMeasurements object identified by index. */
		inline const TimeMeasurements &getTimeMeasurements(uint32 index) const;

		/** Starts the measurement of a time period for the TimeMeasurements object identified by index.
			Use this to profile a single function call for example.
			The time period end is measured by a call to endTimeMeasurement(index) with the same index.
		@param index This identifies the TimeMeasurements object. Objects order equals their creation order.
		@see endTimeMeasurement(...) */
		void startTimeMeasurement(uint32 index);

		/** Resets all TimeMeasurement representations. */
		void reset();

		/** Stores the statistics of all measurements in a file.
		@param fileName Set this to the complete file name including path and file extension. */
		void saveToFile(const std::string &fileName) const;

	private:
		std::vector<TimeMeasurements>	mTimeMeasurements;	/// Stores all time measurement representations.
		std::vector<Timing::TimePoint>	mMeasurementStarts;	/// Stores for each TimeMeasurements the most recent measure start time point.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline const TimeMeasurements &Profiler::getTimeMeasurements(uint32 index) const
	{
		assert(index < mTimeMeasurements.size());
		return mTimeMeasurements[index];
	}
}

#endif // _PROFILER_H_

