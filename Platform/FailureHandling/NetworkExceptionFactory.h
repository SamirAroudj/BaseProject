/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _NETWORK_EXCEPTION_FACTORY_H_
#define _NETWORK_EXCEPTION_FACTORY_H_

#include "ClosedConnectionException.h"
#include "ConnectionCreationException.h"
#include "HostNameResolutionException.h"
#include "InvalidSocketException.h"
#include "NetworkException.h"
#include "SocketBindException.h"

namespace FailureHandling
{
	/// This class is a central point for network exception handling.
	/** Functions for every network exception type are implemented.
	Use this class to throw the corresponding network exceptions. */
	class NetworkExceptionFactory
	{
	public:
		/** Throws an exception of the type HostNameResolutionException which should be thrown if a host name could not be translated into an IP address.
		@param message Should contain a description of the reason why the exception is created.
		@param socketErrorCode Should be filled with an error code obtained from the socket implementation.  See NetworkException.
		@param hostName Should be the name of the host that could not be translated. */
		static void throwHostNameResolutionException(const std::string &message, int32 socketErrorCode, const std::string &hostName)
		{
			throw HostNameResolutionException(message, socketErrorCode, hostName);
		}
		
		/** Throws an exception of the type InvalidSocketException which should be thrown if a socket could not be created.
		@param message Should contain a description of the reason why the exception is created.
		@param socketErrorCode Should be filled with an error code obtained from the socket implementation.  See NetworkException.*/
		static void throwInvalidSocketException(const std::string &message, int32 socketErrorCode)
		{
			throw InvalidSocketException(message, socketErrorCode);
		}

		/** Throws an exception of the type ConnectionCreationException which should be thrown if a connection could not be established.
		@param message Should contain a description of the reason why the exception is created.
		@param type Should be either SOCKET_PROBLEM, TIME_OUT, WRONG_PASSWORD or SESSION_IS_FULL, see ConnectionCreationException::Type
		@param socketErrorCode Should be filled with an error code obtained from the socket implementation.  See NetworkException.
		@param address Should contain the IP address identifying the host the application could not connect to. 
		@param portNumber Should contain the port number of the application running on the remote host to which the connection could not be established.
		@param password Should contain the password which the user entered in order to connect to the network session. */
		static void throwNetworkConnectionCreationException(const std::string &message, ConnectionCreationException::Type type, int32 socketErrorCode,
			const std::string &address = "", uint16 portNumber = 0, const std::string &password = "")
		{
			throw ConnectionCreationException(message, type, socketErrorCode, address, portNumber, password);
		}
		
		/** Throws an exception of the type NetworkException which should be thrown if some network functionality could not be executed properly.
		@param message Should contain a description of the reason why the exception is created.
		@param socketErrorCode Should be filled with an error code obtained from the socket implementation.  See NetworkException. */
		static void throwNetworkException(const std::string &message, int32 socketErrorCode);

		/** Creates an exception that describes a problem which should be occured while socket binding and contains the corresponding error code retireved from the socket implementation.
			@param message Describes the circumstances and or the reason for the creation of the thrown exception.
			@param socketErrorCode A specific error code obtained by the socket implementation that indicates the problem. See NetworkException.
			@param portNumber This number could not be used to bind the port to. */
		static void throwSocketBindException(const std::string &message, int32 socketErrorCode, uint16 portNumber)
		{
			throw SocketBindException(message, socketErrorCode, portNumber);
		}

		/** Throws an exception of the type ClosedConnectionException which should be thrown if connection was closed by the remote host.
		@param message Should contain a description of the reason why the exception is created. 
		@param socketErrorCode A specific error code obtained by the socket implementation that indicates the problem. See NetworkException.*/
		static void throwClosedConnectionException(const std::string &message, int32 socketErrorCode)
		{
			throw ClosedConnectionException(message, socketErrorCode);
		}
	};
}

#endif // _NETWORK_EXCEPTION_HANDLING_H_