/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
#include <WinSock2.h>
#endif // _WINDOWS
#include "NetworkExceptionFactory.h"

using namespace FailureHandling;

void NetworkExceptionFactory::throwNetworkException(const std::string &message, int32 socketErrorCode)
{
	switch(socketErrorCode)
	{
#ifdef _WINDOWS
	case WSAECONNRESET:
	case WSAECONNABORTED:
		throw ClosedConnectionException(message, socketErrorCode);
		break;
#endif // _WINDOWS

	default:
		throw NetworkException(message, socketErrorCode);
		break;
	}
}