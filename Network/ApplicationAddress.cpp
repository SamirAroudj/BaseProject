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
#include "ApplicationAddress.h"
#include "MagicNetworkConstants.h"
#include "Network.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"

using namespace ExceptionHandling;
using namespace Network;
using namespace std;

void ApplicationAddress::fillValues(int32 socketHandle)
{
	sockaddr_in socketAddress;
	socklen_t socketAddressLength = sizeof(sockaddr);
	if (SOCKET_ERROR == getsockname(socketHandle, reinterpret_cast<sockaddr *>(&socketAddress), &socketAddressLength))
		NetworkExceptionFactory::throwNetworkException("Unable to retrieve udp socket port number.", getError());
	mIPAddress = ntohl(socketAddress.sin_addr.s_addr);
	mPortNumber = ntohs(socketAddress.sin_port);
}

ostream &Network::operator <<(ostream &os, const ApplicationAddress &address)
{
	os << "Application address:\n";
	os << "IP address: " << Network::convertAddressHostByteOrderToASCII(address.getIPAddress());
	os << "\nPort number: " << address.getPortNumber() << "\n";

	return os;
}