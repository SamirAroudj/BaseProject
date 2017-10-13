/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "SocketBindException.h"

using namespace FailureHandling;
using namespace std;

SocketBindException::SocketBindException(const std::string &message, int32 socketErrorCode, uint16 portNumber) : 
	NetworkException(message, socketErrorCode), mPortNumber(portNumber)
{
}


SocketBindException::~SocketBindException(void)
{
}

ostream &FailureHandling::operator<<(ostream &os, const SocketBindException &exception)
{
	os << "A socket bind exception occured:\n" << exception.getMessage() << endl;
	os << "Port which could not be bound: " << exception.getPortNumber() << endl;
	os << "Socket error code: " << exception.getSocketErrorCode() << endl;
	os << "Source: " << exception.getSource() << endl;
	return os;
}