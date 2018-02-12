/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _FRAME_RATE_CALCULATOR_H_
#define _FRAME_RATE_CALCULATOR_H_

#include <cassert>
#include "Platform/Timing/TimePeriod.h"

namespace Profiling
{
	/// Small helper class used to measure rendered frames per second.
	class FrameRateCalculator
	{
	public:
		/** Creates the calculater and sets current fps to a negative value.
		@param frameRatePeriod Defines over what time period in seconds frames are counted to measure average frames per second over this period. */
		FrameRateCalculator(Real frameRatePeriod) : mFrameRatePeriod(frameRatePeriod), mFPS(-1.0f), mFramesThisPeriod(0) { assert(frameRatePeriod > 0.0f); }

		/** Returns how many frames per second were rendered last frame rate period.
			The frame rate period length by this objects TimePeriod object.
		@return Returns the number of frames rendered last frame rate period. */
		inline Real getFPS() const { return mFPS; }

		/** Increases the number of frames rendered this frame rate period. */
		inline void onFrameRendered() { ++mFramesThisPeriod; }

		/** Updates FPS calculation. */
		void update();

	private:
		Timing::TimePeriod mFrameRatePeriod;	/// Defines over what time period the average frame rate is measured.
		Real mFPS;								/// Stores how many frames per second were rendered last frame rate period.	
		uint32 mFramesThisPeriod;				/// Counts how many frames have been rendered this frame rate period, used for FPS calculation
	};
}

#endif // _FRAME_RATE_CALCULATER_H_
