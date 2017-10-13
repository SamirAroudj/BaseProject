/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Exception.h"

using std::ostream;
using std::endl;
using namespace FailureHandling;

Exception::Exception(const std::string &message) : mMessage(message)
{
	mSource = "Not implemented so far.";
}

ostream &FailureHandling::operator<<(ostream &os, const Exception &exception)
{
	os << "An exception occured:\n" << exception.getMessage() << endl;
	os << "Source: " << exception.getSource() << endl;
	return os;
}
