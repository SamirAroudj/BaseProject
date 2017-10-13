/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _CONNECTION_EXCEPTION_H_
#define _CONNECTION_EXCEPTION_H_

#include "NetworkException.h"
namespace FailureHandling
{
	/// Should be thrown if a TCP connection to a host application identified by an IP address and port number could not be established in a particular time period.
    /** The exception contains the IP address and port number to identify the remote application to
        which the connection could not be established. Also an description and corresponding error code are contained. */
	class ConnectionCreationException : public NetworkException
	{
	public:
		/// connection creation attempt fail reason: server does not answer, wrong session password, no free place remote session, OS socket implementation problem
        enum Type
        {
            SOCKET_PROBLEM,
            TIME_OUT,
            WRONG_PASSWORD,
            SESSION_IS_FULL,
            NUM_OF
        };

		/** Creates an exception of the type ConnectionCreationException which should be thrown if a connection could not be established.
		@param message Should contain a description of the reason why the exception is created.
		@param type Should be either SOCKET_PROBLEM, TIME_OUT, WRONG_PASSWORD or SESSION_IS_FULL, see ConnectionCreationException::Type
		@param socketErrorCode Should be filled with an error code obtained from the socket implementation.  See NetworkException.
		@param address Should contain the IP address identifying the host the application could not connect to. 
		@param portNumber Should contain the port number of the application running on the remote host to which the connection could not be established. 
		@param password Should contain the password which the user entered in order to connect to the network session.*/
		ConnectionCreationException(const std::string &message, Type type, int32 socketErrorCode,
			const std::string &address = "", uint16 portNumber = 0, const std::string &password = "");

		/** normal destructor */
		~ConnectionCreationException(void);

		/** Get the address of the remote host the program could not connect to.
		@returns Should contain the IP address identifying the host the application could not connect to. */
		const std::string &getIPAddress() const { return mIPAddress; }

		/** Get the password the user entered. A password can be required to connect to a network session.
		@returns Should contain the password which the user entered in order to connect to the network session. */
		const std::string &getPassword() const { return mPassword; }

		/** Get the port number identifying the application the program could not connect to.
		@returns Should contain the port number of the application running on the remote host to which the connection could not be established. */
		uint16 getPortNumber() const { return mPortNumber; }

		/** Get the particular reason for the connection creation fail as enumeration value.
		@returns The returned value should be either SOCKET_PROBLEM, TIME_OUT, WRONG_PASSWORD or SESSION_IS_FULL, see ConnectionCreationException::Type */
		Type getType() const { return mType; }

		/** Give the exception the IP address of the remote host that cannot be connected to. 
		@param IPAddress Should contain the IP address identifying the host the application could not connect to. */
		void setIPAddress(const std::string &IPAddress) { mIPAddress = IPAddress; }

		/** Set the port number to a value that identifies the application running on the host the program could not connect to.
		@param portNumber Should contain the port number of the application running on the remote host to which the connection could not be established. */
		void setPortNumber(uint16 portNumber) { mPortNumber = portNumber; }

	private:
        std::string mIPAddress;     /// IP address of the unreachable remote host
        std::string mPassword;      /// the password which the user entered to connect to the network session
        uint16      mPortNumber;    /// port number of the unreachable application
        Type        mType;          /// the reason for the creation of this connection as an enumberation value
	};
	
	/** Creates a small output message and writes it to the ostream.
	@returns The returned ostream can be used for further concatenation.
	@param os This is the stream the message describing the exception will be written to.
	@param exception The information contained by this exception will be written to the ostream. */	
	std::ostream &operator<<(std::ostream &os, const ConnectionCreationException &exception);
}

#endif // _CONNECTION_EXCEPTION_H_

