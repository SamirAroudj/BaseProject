/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _INVALID_SOCKET_EXCEPTION_H_
#define _INVALID_SOCKET_EXCEPTION_H_

#include "NetworkException.h"

namespace FailureHandling
{
	/// Is intended for the case a socket could not be created.
	/** This exception should be thrown if a socket(...) function call failed. */
	class InvalidSocketException : public NetworkException
	{
	public:
		/** Creates an exception of the type InvalidSocketException which should be thrown if a socket could not be created.
		@param message Should contain a description of the reason why the exception is created.
		@param socketErrorCode Should be filled with an error code obtained from the socket implementation.  See NetworkException.*/
		InvalidSocketException(const std::string &message, int32 socketErrorCode);

		/** normal destructor */
		~InvalidSocketException();
	};
}

#endif // _INVALID_EXCEPTION_H_

