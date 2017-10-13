/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "LogOutput.h"

using namespace Logging;

void LogOutput::convertTime(Real time, int32 &hours, int32 &minutes, int32 &seconds, int32 &milliseconds)
{
	hours = static_cast<int>(time / 3600);											// extract hours, minutes, seconds and milliseconds
	time -= hours;

	minutes = static_cast<int>(time / 60);
	time -= minutes;
		
	seconds = static_cast<int>(time);
	time -= seconds;

	milliseconds = static_cast<int>(time * 1000);
}