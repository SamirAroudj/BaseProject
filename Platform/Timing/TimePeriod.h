/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _TIME_PERIOD_H_
#define _TIME_PERIOD_H_

#include "Platform/DataTypes.h"

namespace Timing
{
	class ApplicationTimer;
	
	/// This class represents a period of time that can expire. For example it can be used to detect timeouts.
    /** A TimePeriod object stores a time duration and starts to count the time at its creation in order
        to detect weather the given period of time has experied.
        All TimePeriod objects are updated automatically by the Application timer for convenience and to ensure correct values.
        There is only one current application time at each frame. */
	class TimePeriod
	{
	friend class ApplicationTimer;

	public:
		/** Creates a TimePeriod object that will expire after period seconds. </summary>
		@param name="period">	Defines the time period in seconds the expiration of which is detected */
		TimePeriod(Real period);

		/** Creates a copy of the TimePeriod and also registers it for updates.
		@param copy The new TimePeriod retrieves its values from copy.*/
		TimePeriod(const TimePeriod &copy);

		/** Destroys and deregisters the TimePeriod object. */
		~TimePeriod();

		/** Assignment operator sets the values of the time period with values from rhs.
		@param rhs Its values are used to fill this time period object with new values. </param>
		@return The time period with the new values is returned.*/
		TimePeriod &operator =(const TimePeriod &rhs);

		/**  Continues expiration of this time period.*/
		void continueTiming();

		/** Decreases time period expiration of which is measured.
		@param delta Time period value is decreased by delta seconds. This value must be positive.*/
		void decreasePeriod(Real delta);

		/** Obtain elapsed time since last reset or object construction. (in seconds)
		@return Time which has elapsed since last reset or object construction is returned. (in seconds)*/
		Real getElapsedTime() const { return mElapsedTime; }

		/** Retrieve the length of the period of time the expiration of which is measured.
		@return The function returns the length of the time period in seconds.*/
		Real getLength() const { return mTimePeriod; }

		/** Detect weather t seconds have passed by since the construction of the time period whereas t is the time in seconds used for the constructor of this object.
		@return The function retuns true when more than t seconds have passed by since the construction of this object with t seconds as parameter. */
		bool hasExpired() const { return mElapsedTime >= mTimePeriod; }

		/** Increases the time period the expiration of which is measured.
		@param delta The time period value is increased by delta seconds. This value must be positive.*/
		void increasePeriod(Real delta);

		/** Returns true if the time period expiration is stopped.
		@return If the function returns true then the time period won't expire since the passing of its time is stopped.*/
		bool isStopped() const { return mStopped; }

		/** Sets the time period to its creation state. That means the time period will expire when the in the constructor specified time has passed since this call.
			Timing continues as the reset function call also restarts the timing if it was stopped.*/
		void reset();

		/** Sets the time period to its creation state and assigns a new time period the expiration of which is measured.
			That means the time period will expire when period seconds have passed since this call.
			Timing continues as the reset function call also restarts the timing if it was stopped.
		@param period Defines the time period in seconds the expiration of which is detected.*/
		void reset(Real period);

		/** Stops the time period expiration by fixing its elapsed time until continueTiming() is called. */
		void stopTiming();

	private:
		/** Update the internal passed time in order to detect time period expiration. This function is automatically called by the ApplicationTimer.
		@param This is the time in seconds that has passed between the last call of update and the current invocation of the update function. */
		void update(Real deltaTime);

    private:
		Real mElapsedTime;	/// This is the time which has passed since the objects construction or reset.
		Real mTimePeriod;	/// This is the period of time the expiration of which is measured.
		bool mStopped;		/// Is true if the passing of mElapsedTime is stopped.
	};
}

#endif // _TIME_PERIOD_H_

