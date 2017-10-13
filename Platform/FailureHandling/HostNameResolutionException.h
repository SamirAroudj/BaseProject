/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _HOST_NAME_RESOLUTION_EXCEPTION_H_
#define _HOST_NAME_RESOLUTION_EXCEPTION_H_

#include "NetworkException.h"
#include <string>

namespace FailureHandling
{
	/// The purpose of this exception is to indicate the case that a host name could not be translated into an IP address.
	/** The exception should be thrown if the domain name service could not be used properly to get the IP address of a host. */
	class HostNameResolutionException : public NetworkException
	{
	public:
		/** Creates an exception of the type HostNameResolutionException which is thrown if a host name could not be translated into an IP address.
		@param message Should contain a description of the reason why the exception is created.
		@param socketErrorCode Should be filled with an error code obtained from the socket implementation.  See NetworkException.
		@param hostName Should be the name of the host that could not be translated. */
		HostNameResolutionException(const std::string &message, int32 socketErrorCode, const std::string &hostName);

		/** normal destructor */
		~HostNameResolutionException();

		/** Can be used to get the host name the IP address of which could not be obtained.
		@returns Returns the name of the host that could not be translated to an IP address due to a DNS / network problem or wrong host name. */
		const std::string &getHostName() const { return mHostName; }

	private:
		/// the name of the host the IP address of which could not be retrieved.
		const std::string mHostName;
	};
	
	/** Exception's message, source etc. can be printed to a console window.
	@param os This is the desired output stream, eg cout.
    @param hostName This is the exception the message and source of which is printed. */
	std::ostream &operator<<(std::ostream &os, const HostNameResolutionException &hostName);
}

#endif // _HOST_NAME_RESOLUTION_EXCEPTION_H_

