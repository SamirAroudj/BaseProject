/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _FRAME_STATISTICS_H_
#define _FRAME_STATISTICS_H_

#include <string>
#include "Graphics3D/MagicGraphicsConstants.h"
#include "Platform/TimePeriod.h"

namespace Graphics
{
	/// The FrameStatistics class is used to compute the average frames per second and the average time to draw one frame in milliseconds.
	class FrameStatistics
	{
	public:
		/** The number of drawn frames are calculated over a period of measurePeriod seconds and then used for FPS calculation.
		@param measurePeriod The number of drawn frames are calculated over a period of measurePeriod seconds.
			The number is used to compute the FPS, so that FPS = number of drawn frames / measurePeriod. */
		FrameStatistics(Real measurePeriod = FRAME_STATISTICS_MEASURE_PERIOD);

		/** The statistics contain the average number of frames per second and the time needed to draw a frame in milliseconds.
		@return The returned string contains the average frames per second (FPS) and the time necessary to draw a frame in milliseconds (MSPF). */
		const std::string &getStatistics() const { return mStatistics; }

		/** Updates this object to refresh the statistic calculations. */
		void update();

	private:
		std::string mStatistics;				/// The statistics contain the average number of frames per second and the time needed to draw a frame in milliseconds.
		Platform::TimePeriod mMeasurePeriod;	/// Specifies the time period over which frames are counted to compute average statistics.
		uint32 mFramesThisPeriod;				/// The current number of drawn frames during this measure period.
	};
}

#endif // _FRAME_STATISTICS_H_
