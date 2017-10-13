/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _CONSOLE_LOG_OUTPUT_H_
#define _CONSOLE_LOG_OUTPUT_H_

#include <cassert>
#include "LogOutput.h"

namespace Logging
{
	class ConsoleLogOutput : public LogOutput
	{
	public:
		/** Creates an output channel to log messages to console. */
		ConsoleLogOutput();

		/** Destroys the console output channel. */
		virtual ~ConsoleLogOutput();

		/** Adds a single log entry to this output target.
		@param time This is the time which has elapsed since program start in seconds.
		@param logLevel This is the log level of this entry, see LogLevel enumeration of LogManager. 
		@param message This is the message that is logged. 
		@param exception The data of an optional exception is also logged. */
		virtual void log(Real time, const char *logLevel, const char *message,
						 FailureHandling::Exception *exception = NULL);

	private:
        /** Copy constructor is forbidden.
        @param copy Copy constructor is forbidden. */
		ConsoleLogOutput(const ConsoleLogOutput &copy)
		{
			assert(false);
		}

        /** Assignment operator is forbidden.
        @param rhs Operator is forbidden.*/
		ConsoleLogOutput &operator =(const ConsoleLogOutput &rhs)
		{
			assert(false); return *this;
		}
	};
}

#endif // _CONSOLE_LOG_OUTPUT_H_

