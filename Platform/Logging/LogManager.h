/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _LOG_MANAGER_H_
#define _LOG_MANAGER_H_

#include <cassert>
#include "ConsoleLogOutput.h"
#include "FileLogOutput.h"

/// Provides a collection of useful functionality.
/** This namespace is a collection of helper classes. */
namespace Logging
{
	/// Provides functionality to manage developpers' output data
	class LogManager
	{
	public:
		/// Log level is used to define the importance of log entries
		enum LogLevel
		{
			TRACE,		/// least important log entry - used to find out whats going on in a very detailed way
			INFO,		/// quite unimportant log entry that is used to log what is happening
			WARNING,	/// log something that went wrong, but that can be handled by the app
			ERROR,		/// used for recording of errors that prevent correct program execution
			FATAL,		/// used for recording of problems that are terrible and lead to program execution stop
			FORCE		/// used for the most important log entries that are always processed and never ignored
		};
	
		/** Returns the name of the log level as character array. E.g. "Trace" is returned if level is TRACE.
		@param level This is the log level the name of which is requested. See ILogOutput.LogLevel.
		@return Returns the name of the entered level, e.g. "Info". */
		static const char *convertLogLevelToString(LogLevel level);

		/** Returns the log level as LogLevel enumeration value. E.g. TRACE is returned if levelName is "Trace".
		@param levelName Possible Values: Trace, Info, Warning, Error, Fatal, Force.
		@return Returns the enumeration value that corresponds to the entered log level name, e.g. WARNING. */
		static LogLevel convertStringToLogLevel(const char *levelName);

		/** Adds a single log entry to the console and or file output target.
		@param logLevel This is the log level of this entry, see LogLevel enumeration. 
		@param message This is the message that is logged. */
		static void log(LogLevel logLevel, const char *message);
			
	protected:
		/** Initializes the log manager. */
		LogManager();

		/** Destroys the log manager object. */
		~LogManager();

    private:
        /** Copy constructor is forbidden.
        @param copy Copy constructor is forbidden.*/
        LogManager(const LogManager &copy) : mFileOutput("") { assert(false); }

        /** Assignment operator is forbidden.
        @param rhs Operator is forbidden. */
        LogManager &operator =(const LogManager &rhs) { assert(false); return *this; }

    protected:
		static LogManager	sInstance;		/// this is the one and only instance of this manager

		ConsoleLogOutput	mConsoleOutput;	/// this is the output target that writes to the console
		FileLogOutput		mFileOutput;	/// this is the output target that writes to a file
		LogLevel			mLogLevel;		/// minimum log level -> filtering
		bool				mLogToConsole;	/// log messages are only sent to console if this is true
		bool				mLogToFile;		/// log messages are only sent to file if this is true
	};
}

#endif // _LOG_MANAGER_H_
