/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _NETWORK_EXCEPTION_H_
#define _NETWORK_EXCEPTION_H_

#include "Platform/DataTypes.h"
#include "Exception.h"

namespace FailureHandling
{
	/// This class should be created if there is a problem related to network functionality.
	/** This class is the base type for several other network exceptions and can be used if there is no particular other class for the problem. */
	class NetworkException : public Exception
	{
	public:
		/** Creates an exception of the common base type NetworkException which is thrown if some network functionality could not be executed properly.
		@param message Should contain a description of the reason why the exception is created.
		@param socketErrorCode Should be filled with an error code obtained from the socket implementation. */
		NetworkException(const std::string &message, int32 socketErrorCode);

		/** Returns a socket code that was created by WinSock, a Berkeley Sockets or in few cases by game application,
			0 if the remote side closed the connection,
			1 if the hosts WinSock version is not sufficient,
			see WinSock / Berkely documentation for other socket error codes for further information.
			@returns Should contain an error code indicating the reason why a function could not be executed properly. */
		int32 getSocketErrorCode() const { return mSocketErrorCode; }

	private:
		/// error code corresponding to a particular problem
		/** @see int32 getSocketErrorCode8) const */
		int32 mSocketErrorCode;
	};
		
	/** Exception's message, source etc. can be printed to a console window.
	@param os This is the desired output stream, eg cout.
	@param exception This is the exception the message and source of which is printed.
	*/
	std::ostream &operator<<(std::ostream &os, const NetworkException &exception);
}

#endif // _NETWORK_EXCEPTION_H_

