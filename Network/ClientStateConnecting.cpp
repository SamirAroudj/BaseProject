/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
#include <WinSock2.h>
#else
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include "Network.h"
#endif // _WINDOWS

#include "ClientStateConnecting.h"
#include "MagicNetworkConstants.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"

using namespace ExceptionHandling;
using namespace Network;
using namespace std;

ClientStateConnecting::ClientStateConnecting(const ApplicationAddress &serverAddress, const string &password, Real timeoutSeconds) : 
	mTimeout(timeoutSeconds),
	mPassword(password),
	mServerAddress(serverAddress)
{
	mTCPClient = new TCPClientEnd(socket(PF_INET, SOCK_STREAM, 0));
	try
	{
		mTCPClient->connect(serverAddress);
	}
	catch(NetworkException &exception)
	{
		delete mTCPClient;
		throw exception;
	}
}


ClientStateConnecting::~ClientStateConnecting()
{
	if (mTCPClient)
		delete mTCPClient;
}


ClientState::State ClientStateConnecting::update()
{
	timeval immediately = { 0, 0 }; // don't wait until something is readable or writable, just check the state
	fd_set writableSockets;	// prepare for writability check
	FD_ZERO(&writableSockets);
	FD_SET(mTCPClient->getSocketHandle(), &writableSockets);
	select(mTCPClient->getSocketHandle() + 1, 0, &writableSockets, NULL, &immediately);

	// is there an answer for the connection request?
	if (FD_ISSET(mTCPClient->getSocketHandle(), &writableSockets))
	{
#ifdef _WINDOWS
		return ClientState::CONNECTED;
#else
		socklen_t addressLength = sizeof(sockaddr);
		sockaddr_in socketAddress;
		if (SOCKET_ERROR == getpeername(mTCPClient->getSocketHandle(), reinterpret_cast<sockaddr *>(&socketAddress), &addressLength))
		{
			char buffer[1];
			int32 errorNumber = recv(mTCPClient->getSocketHandle(), buffer, 1, 0);
			NetworkExceptionFactory::throwNetworkConnectionCreationException("Could not create the requested connection to the server.",
				ConnectionCreationException::SOCKET_PROBLEM, errorNumber,
                    Network::convertAddressHostByteOrderToASCII(mServerAddress.getIPAddress()), mServerAddress.getPortNumber(),
                    "Not related to a password problem.");
		}
		else
		{
			return ClientState::CONNECTED;
		}
#endif
	}

	// check the time period which takes care that the connection request response is received in the specified time
	if (mTimeout.hasExpired())
		return ClientState::DISCONNECTED;

	return ClientState::NO_STATE_CHANGE;
}
