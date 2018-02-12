/**
	Author: Samir Aroudj
*/

#ifdef CUDA

#include <cstring>
#include "Platform/Profiling/CUDAProfiler.h"
#include "Platform/FailureHandling/FailureHandling.h"

using namespace Profiling;
using namespace FailureHandling;
using namespace std;

CUDAProfiler::EventPair::EventPair()
{
	memset(this, 0, sizeof(EventPair));

	cudaEventCreateWithFlags(&mStart, cudaEventDefault);
	cudaEventCreateWithFlags(&mEnd, cudaEventDefault);
}

CUDAProfiler::CUDAProfiler(const string *measurementNames, uint32 numberOfMeasurementNames) :
	Profiler(measurementNames, numberOfMeasurementNames)
{
	#ifdef PROFILING
		// create an event pair for each measurement type
		for (uint32 i = 0; i < numberOfMeasurementNames; ++i)
			mEventPairs.push_back(EventPair());
	#endif // PROFILING
}

CUDAProfiler::~CUDAProfiler()
{
	#ifdef PROFILING
		uint32 pairCount = (uint32) mEventPairs.size();

		for (uint32 i = 0; i < pairCount; ++i)
		{
			EventPair &pair = mEventPairs[i];

			cudaEventDestroy(pair.mStart);
			cudaEventDestroy(pair.mEnd);
		}
	#endif // PROFILING
}

uint32 CUDAProfiler::addMeasurementType(const string &name)
{
	#ifdef PROFILING
		// create the event pair
		mEventPairs.push_back(EventPair());

		return Profiler::addMeasurementType(name);

	#else
		return 0;

	#endif // PROFILING
}

void CUDAProfiler::recordEndEvent(uint32 index, const cudaStream_t &stream)
{
	#ifdef PROFILING
		assert(index < mEventPairs.size());

		EventPair &pair = mEventPairs[index];
		cudaEventRecord(pair.mEnd, stream);
		pair.mSetEnd = true;
	#endif // PROFILING
}

void CUDAProfiler::recordStartEvent(uint32 index, const cudaStream_t &stream)
{
	#ifdef PROFILING
		assert(index < mEventPairs.size());

		EventPair &pair = mEventPairs[index];
		cudaEventRecord(pair.mStart, stream);
		pair.mSetStart = true;
	#endif // PROFILING
}

void CUDAProfiler::measureEventTiming(uint32 index)
{
	#ifdef PROFILING
		assert(index < mEventPairs.size());
		float		milliseconds = 0;
		EventPair	&pair		 = mEventPairs[index];

		// Did the user record both events?
		assert(pair.mSetStart);
		assert(pair.mSetEnd);

		// compute delta time
		cudaEventElapsedTime(&milliseconds, pair.mStart, pair.mEnd);
		pair.mSetEnd = false;
		checkForErrors();

		// update statistics
		double seconds = milliseconds / 1000.0;
		addTimeMeasurement(index, seconds);
	#endif // PROFILING
}

void CUDAProfiler::measureEventTimings()
{
	#ifdef PROFILING
		// get measurement for each recorded pair
		uint32 pairCount = (uint32) mEventPairs.size();
		for (uint32 pairIdx = 0; pairIdx < pairCount; ++pairIdx)
		{
			// get recorded pair
			EventPair &pair = mEventPairs[pairIdx];
			if (!pair.mSetEnd || !pair.mSetStart)
				continue;

			measureEventTiming(pairIdx);
		}
	#endif // PROFILING
}

#endif // CUDA
