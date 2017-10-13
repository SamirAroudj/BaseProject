/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cassert>
#include "ConnectionCreationException.h"

using namespace FailureHandling;
using namespace std;

ConnectionCreationException::ConnectionCreationException(const string &message, Type type, int32 socketErrorCode,
	const string &address, uint16 portNumber, const string &password) :
	NetworkException(message, socketErrorCode), mIPAddress(address), mPortNumber(portNumber), mPassword(password), mType(type)
{
}


ConnectionCreationException::~ConnectionCreationException(void)
{
}

ostream &FailureHandling::operator<<(ostream &os, const ConnectionCreationException &exception)
{
	os << "A connection creation exception occured:\n" << exception.getMessage() << endl;
	switch(exception.getType())
	{
	case ConnectionCreationException::TIME_OUT:
		os << "The server didn't answer in the specified period of time." << endl;
		break;

	case ConnectionCreationException::WRONG_PASSWORD:
		os << "The password specified to join the network session is wrong. " << endl;
		break;

	case ConnectionCreationException::SESSION_IS_FULL:
		os << "There is no free place to join the network session." << endl;
		break;

	case ConnectionCreationException::SOCKET_PROBLEM:
		os << "There is a problem with the socket implementation." << endl;
		if (101 == exception.getSocketErrorCode())
			os << "The connection destination is unreachable." << endl;
		break;

	default:
		assert(false);
	}
	os << "Destination IP address: " << exception.getIPAddress() << " and port number: " << exception.getPortNumber() << endl;
	os << "socket error code: " << exception.getSocketErrorCode() << endl;
	os << "entered password: " << exception.getPassword() << endl;
	os << "Source: " << exception.getSource() << endl;
	return os;
}
