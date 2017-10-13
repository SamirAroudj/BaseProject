/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _LOG_OUTPUT_H_
#define _LOG_OUTPUT_H_

#include <cassert>
#include "Platform/DataTypes.h"
#include "Platform/FailureHandling/Exception.h"

namespace Logging
{
	/// Interface that must be implemented by classes that process log output data
	class LogOutput
	{
	public:
		/** Virtual destructor for safe programming reasons. */
		virtual ~LogOutput() { }

		/** Adds a single log entry to this output target.
		@param time This is the time which has elapsed since program start in seconds.
		@param logLevel This is the log level of this entry, see LogLevel enumeration of LogManager. 
		@param message This is the message that is logged.
		@param exception The data of an optional exception is also logged. */
		virtual void log(Real time, const char *logLevel, const char *message,
						 FailureHandling::Exception *exception = NULL) = 0;

	protected:
        /** Empty constructor. */
        LogOutput() {}\

		/** Extracts hours, minutes, seconds and milliseconds from a floating point time variable with the unit seconds.
			E.g. 3671.10762345 is converted to 1h, 1m, 11s and 1076ms.
		@param time This variable stores a time value in seconds.
		@param hours Contains the hours extracted from time.
		@param minutes Contains the minutes extracted from time.
		@param seconds Contains the seconds extracted from time.
		@param milliseconds Contains the milliseconds extracted from time*/
		static void convertTime(Real time, int32 &hours, int32 &minutes, int32 &seconds, int32 &milliseconds);

	private:
        /** Copy constructor is forbidden.
        @param copy Copy constructor is forbidden. */
        LogOutput(LogOutput &copy) { assert(false); }

        /** Assignment operator is forbidden.
        @param rhs Operator is forbidden.*/
		LogOutput &operator =(const LogOutput &rhs) { assert(false); return *this; }
	};
}

#endif // _I_LOG_OUPUT_H_
