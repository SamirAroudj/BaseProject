/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _FILE_LOG_OUTPUT_H_
#define _FILE_LOG_OUTPUT_H_

#include <cassert>
#include <string>
#include "../Storage/File.h"
#include "LogOutput.h"

namespace Logging
{
	/** Implements the Interface ILogOutput to write log data to files. */
	class FileLogOutput : public Logging::LogOutput
	{
	public:
		/** Creates an output channel to log messages to files.
		@param fileName This is the path and name of the file where the logging messages are written to.
		@param mode Defines how the file is treated. See Platform::File::FileMode. */
		FileLogOutput(const std::string &fileName,
					  Storage::File::FileMode mode = Storage::File::CREATE_WRITING);

		/** Destroys the file output channel. */
		virtual ~FileLogOutput();

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
		FileLogOutput(const FileLogOutput &copy) : mLogFile(Storage::Path(""), Storage::File::CREATE_WRITING, false)
		{
			assert(false);
		}

        /** Assignment operator is forbidden.
        @rhs Operator is forbidden. */
		FileLogOutput &operator =(const FileLogOutput &rhs)
		{
			assert(false); return *this;
		}  

    private:
        Storage::File mLogFile;	/// this is the file log messages are written to
	};
}

#endif // _FILE_LOG_OUTPUT_H_
