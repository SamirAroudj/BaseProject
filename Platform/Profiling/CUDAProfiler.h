/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _CUDA_PROFILER_H_
#define _CUDA_PROFILER_H_
#ifdef CUDA

#include "Platform/Profiling/Profiler.h"

namespace Profiling
{
	/// Extends Profiler class with event functionality to profile asynchronously running CUDA code.
	class CUDAProfiler : public Profiler
	{
	public:
		/// Contains to events to record the start and end time points of the possibly asynchronous execution of some code.
		struct EventPair
		{
			/** Sets the whole struct to 0 and creates the events afterwards. */
			EventPair();

			cudaEvent_t mEnd;		/// Used to record the end time point of asynchronously executed code.
			cudaEvent_t mStart;		/// Used to record the start time point of asynchronously executed code.
			bool		mSetEnd;	/// Is true if the time measurement end event was scheduled. Set to false after each measuremnt.
			bool		mSetStart;	/// Is true if the time measurement start event was scheduled.
		};

		/** Creates the profiler and allocates time measurement representations if wanted.
			All created TimeMeasurement Objects are ordered as their names in measurementNames,
			e.g. object with names[0] is identified by its index 0.
		@param measurementNames Set this to an array of names for description of the measurement statistics to be created.
		@param numberOfMeasurementNames Set this to the element count of measurementNames which is equal to the number of
			TimeMeasurement representions to be created. */
		CUDAProfiler(const std::string *measurementNames, uint32 numberOfMeasurementNames);

		/** Frees resources. */
		~CUDAProfiler();

		/** Adds a new TimeMeasurements object.
		@param name This name describes the TimeMeasurements object.
		@return Returns the index of the new TimeMeasurements object which is necesary to access it. */
		uint32 addMeasurementType(const std::string &name);

		/** Call this to schedule the end event for time period measurement between an event pair for a particular piece of code.
		@param index This identifies the event pair and TimeMeasurements object for which the end CUDA event is recorded.
			Objects order equals their creation order.
		@param stream Set this to the CUDA stream in which the event shall be insterted to measure performance of code synchronized by this stream. */
		void recordEndEvent(uint32 index, const cudaStream_t &stream);

		/** Call this to schedule the start event for time period measurement between an event pair for a particular piece of code.
		@param index This identifies the event pair and TimeMeasurements object for which the end CUDA event is recorded.
			Objects order equals their creation order.
		@param stream Set this to the CUDA stream in which the event shall be insterted to measure performance of code synchronized by this stream.*/
		void recordStartEvent(uint32 index, const cudaStream_t &stream);

		/** Measures the time period for the event pair and TimeMeasurements object identified by index and updates the TimeMeasurements object statistics.
		@param index This identifies the event pair and TimeMeasurements object for which the end CUDA event is recorded.
			Objects order equals their creation order. */
		void measureEventTiming(uint32 index);

		/** Measures the time period between the recordings of the start and end event for each pair and
			adds the measurements to their respective TimeMeasurement objects.
			Only events for which both recordStartEvent(...) and recordEndEvent(...) were called can be measured.
			You also need to make sure that these events actually have been recorded, asynchronous computations must have been finished.
		@see recordStartEvent(...), recordEndEvent(...) */
		void measureEventTimings();

	private:
		std::vector<EventPair> mEventPairs; /// Each event pair is used to profile some particular piece of code.
	};
}

#endif // CUDA
#endif // _CUDA_PROFILER_H_
