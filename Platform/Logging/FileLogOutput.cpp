/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "FileLogOutput.h"
#include "Platform/ExceptionHandling/FileAccessException.h"

using namespace ExceptionHandling;
using namespace Logging;
using namespace Platform;
using namespace std;

FileLogOutput::FileLogOutput(const string &fileName, File::FileMode mode) : 
	mLogFile(fileName, mode, false)
{

}


FileLogOutput::~FileLogOutput()
{

}

void FileLogOutput::log(Real time, const char *logLevel, const char *message, Exception *exception)
{
	int hours, minutes, seconds, milliseconds;
	convertTime(time, hours, minutes, seconds, milliseconds);

	fprintf(&mLogFile.getHandle(), "[%dh %dm %ds %dms][%s]:\t\t %s\n",
			hours, minutes, seconds, milliseconds, logLevel, message);

	if (exception)
		fprintf(&mLogFile.getHandle(), "Appended exception: %s\n", exception->getMessage());
}