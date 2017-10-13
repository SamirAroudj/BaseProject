/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "HostNameResolutionException.h"

using namespace FailureHandling;
using namespace std;

HostNameResolutionException::HostNameResolutionException(const string &message, int32 socketErrorCode, const string &hostName) :
	NetworkException(message, socketErrorCode), mHostName(hostName)
{
}


HostNameResolutionException::~HostNameResolutionException()
{
}

ostream &FailureHandling::operator<<(ostream &os, const HostNameResolutionException &exception)
{
	os << "A network exception occured:\n" << exception.getMessage() << endl;
	os << "Host name: " << exception.getHostName() << endl;
	os << "socket error code: " << exception.getSocketErrorCode() << endl;
	os << "Source: " << exception.getSource() << endl;
	return os;
}
