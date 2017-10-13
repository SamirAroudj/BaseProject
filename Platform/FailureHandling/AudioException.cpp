/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "AudioException.h"

using namespace FailureHandling;
using namespace std;

AudioException::AudioException(const string &message) : Exception(message)
{
}


ostream &FailureHandling::operator <<(ostream &os, const AudioException &exception)
{
	os	<< "An exception related to the audio part of the software occured.\n" <<
		"Message: " << exception.getMessage() << "\n" <<
		"Source: " << exception.getSource() << endl;
	return os;
}
