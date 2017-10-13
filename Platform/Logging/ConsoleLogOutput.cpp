/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <iostream>
#include "ConsoleLogOutput.h"

using namespace ExceptionHandling;
using namespace Logging;
using namespace std;

ConsoleLogOutput::ConsoleLogOutput()
{

}


ConsoleLogOutput::~ConsoleLogOutput()
{

}

void ConsoleLogOutput::log(Real time, const char *logLevel, const char *message, Exception *exception)
{
	int hours, minutes, seconds, milliseconds;
	convertTime(time, hours, minutes, seconds, milliseconds);

	cout << "[" << hours << " " << minutes << " " << seconds << " " << milliseconds << "]";
	cout << "[" << logLevel << "]:\t\t" << message << "\n";

	if (exception)
		cout << "Appended exception: " + exception->getMessage();

	cout << endl;
}