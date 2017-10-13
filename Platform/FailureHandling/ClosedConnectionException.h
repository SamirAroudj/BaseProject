/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _CLOSED_CONNECTION_EXCEPTION_H_
#define _CLOSED_CONNECTION_EXCEPTION_H_

#include "NetworkException.h"

namespace FailureHandling
{
	/// This connection should be created if a closed connection is detected.
	/** The class is used for the case that the remote side closed the TCP connection.
        This situation should be indicated by a 0 returned by the recv(int socket...) function indicating that
        the remote side closed the connection by stopping sending data. */
	class ClosedConnectionException : public NetworkException
	{
	public:
		/** Creates an exception of the type ClosedConnectionException which should be thrown if connection was closed by the remote host.
		This situation should be indicated by a 0 returned by the recv(int socket...) function indicating that the remote side closed the connection by stopping sending data.
		@param message Should contain a description of the reason why the exception is created.
		@param socketErrorCode A specific error code obtained by the socket implementation that indicates the problem. Should be 0. See NetworkException.*/
		ClosedConnectionException(const std::string &message, int32 socketErrorCode);
	};
}

#endif // _CLOSED_CONNECTION_EXCEPTION_H_
