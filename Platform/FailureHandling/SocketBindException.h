/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _SOCKET_BIND_EXCEPTION_H_
#define _SOCKET_BIND_EXCEPTION_H_

#include "NetworkException.h"

namespace FailureHandling
{
	/// This exception is used when a socket cannot be bound to a specific port.
	/** This exception is thrown when a socket cannot be bound to a port since this port is already used. */
	class SocketBindException : public NetworkException
	{
	public:
		/** Creates an excepiton that describes a problem which is occured while socket binding and contains the corresponding error code retireved from the socket implementation.
			@param message Describes the circumstances and or the reason for the creation of the thrown exception.
			@param socketErrorCode A specific error code obtained by the socket implementation that indicates the problem.
			@param portNumber This number could not be used to bind the port to. */
		SocketBindException(const std::string &message, int32 socketErrorCode, uint16 portNumber);

		/** normal destructor */
		~SocketBindException(void);

		/** Returns the port number that could not be used by the application. 
		@returns The socket could not be bound to the return value, the port number seems to be already used. */
		uint16 getPortNumber() const { return mPortNumber; }

	private:
		/// the port number which caused the problem
		uint16 mPortNumber;
	};

	/** Creates a small output message and writes it to the ostream.
	@returns The returned ostream can be used for further concatenation.
	@param os This is the stream the message describing the exception will be written to.
	@param exception The information contained by this exception will be written to the ostream. */
	std::ostream &operator<<(std::ostream &os, const SocketBindException &exception);
}

#endif // _SOCKET_BIND_EXCEPTION_H_

