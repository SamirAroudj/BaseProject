/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#include "Platform/Storage/File.h"
#include "Platform/Profiling/Profiler.h"

using namespace Profiling;
using namespace std;
using namespace Storage;
using namespace Timing;


Profiler::Profiler(const string *measurementNames, uint32 numberOfMeasurementNames)
{
	#ifdef PROFILING
		// allocate memory & create 1x TimeMeasurements for each type / name
		mMeasurementStarts.resize(numberOfMeasurementNames);
		for (uint32 i = 0; i < numberOfMeasurementNames; ++i)
			mTimeMeasurements.push_back(measurementNames[i]);
	#endif // PROFILING
}

Profiler::~Profiler()
{

}

uint32 Profiler::addMeasurementType(const string &name)
{
	#ifdef PROFILING
		// add new representation & update mMeasurementStarts
		mTimeMeasurements.push_back(TimeMeasurements(name));

		size_t newCount = mTimeMeasurements.size();
		mMeasurementStarts.resize(newCount);

		return (uint32) (newCount - 1);
	#endif // PROFILING
}

void Profiler::addTimeMeasurement(uint32 index, double timePeriod)
{
	#ifdef PROFILING
		// update the specific TimeMeasurements object
		assert(index < mTimeMeasurements.size());
		mTimeMeasurements[index].add(timePeriod);
	#endif // PROFILING
}

void Profiler::endTimeMeasurement(uint32 index)
{
	#ifdef PROFILING
		assert(index < mTimeMeasurements.size());

		// compute & store delta time
		ApplicationTimer &appTimer = ApplicationTimer::getSingleton();
		TimeMeasurements &times = mTimeMeasurements[index];
		const TimePoint *start = &mMeasurementStarts[index];

		times.add(appTimer.getTimeMeasurement(start));
	#endif // PROFILING
}

void Profiler::reset()
{
	#ifdef PROFILING
		size_t count = mTimeMeasurements.size();
		for (size_t i = 0; i < count; ++i)
			mTimeMeasurements[i].reset();
	#endif // PROFILING
}

void Profiler::saveToFile(const std::string &fileName) const
{
	#ifdef PROFILING
		File file(fileName, File::CREATE_WRITING, false);

		fprintf(&file.getHandle(), "Profiling File\n");

		size_t count = mTimeMeasurements.size();
		for (size_t i = 0; i < count; ++i)
		{
			string text = mTimeMeasurements[i].toString();
			fputs(text.c_str(), &file.getHandle());
			fputc('\n', &file.getHandle());
		}
	#endif // PROFILING
}


void Profiler::startTimeMeasurement(uint32 index)
{
	#ifdef PROFILING
		assert(index < mTimeMeasurements.size());

		// store current time point
		mMeasurementStarts[index] = ApplicationTimer::getSingleton().startTimeMeasurement();
	#endif // PROFILING
}
