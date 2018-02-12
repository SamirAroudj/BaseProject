/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "../ApplicationTimer.h"
#include "../MagicConstants.h"
#include "LogManager.h"

using namespace Platform;
using namespace std;
using namespace Logging;


// constants
const char *Platform::LogManager::LOG_FILE_NAME = DATA_DIRECTORY "Application.log";

// static variables
LogManager LogManager::sInstance = LogManager();

const char *LogManager::convertLogLevelToString(LogLevel level)
{
	switch(level)
	{
	case TRACE:
		return "Trace";

	case INFO:
		return "Info";

	case WARNING:
		return "Warning";

	case ERROR:
		return "Error";

	case FATAL:
		return "Fatal";

	case FORCE:
		return "Force";

	default:
		assert(false);
		return "";
	}
}

LogManager::LogLevel LogManager::convertStringToLogLevel(const char *levelName)
{
	if (0 == strcmp("Trace", levelName))
		return TRACE;

	if (0 == strcmp("Info", levelName))
		return INFO;

	if (0 == strcmp("Warning", levelName))
		return WARNING;

	if (0 == strcmp("Error", levelName))
		return ERROR;

	if (0 == strcmp("Fatal", levelName))
		return FATAL;

	if (0 == strcmp("Force", levelName))
		return FORCE;

	assert(false);
	return TRACE;
}


void LogManager::log(LogLevel logLevel, const char *message)
{
	if (logLevel < sInstance.mLogLevel)
		return;

	Real		time		= 0.0f;
	const char *levelName	= convertLogLevelToString(logLevel);

	if (ApplicationTimer::exists())
		time = ApplicationTimer::getSingleton().getDeltaTimeSinceStart();

	if (sInstance.mLogToConsole)
		sInstance.mConsoleOutput.log(time, levelName, message);
	if (sInstance.mLogToFile)
		sInstance.mFileOutput.log(time, levelName, message);
}

LogManager::LogManager() :
	mConsoleOutput(), mFileOutput(LOG_FILE_NAME), mLogLevel(TRACE),
	mLogToConsole(true), mLogToFile(true)
{	
	//TODO falsche reihenfolge
}

LogManager::~LogManager()
{
}
