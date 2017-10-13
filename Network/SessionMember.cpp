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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif // _WINDOWS

#include "SessionMember.h"

using namespace Network;
using namespace std;

SessionMember::SessionMember(uint32 IPAddress, uint16 UDPPortNumber, uint16 identifier, bool multicastSupport) :
	mIPAddress(IPAddress), mIdentifier(identifier), mUDPPortNumber(UDPPortNumber), mMulticastSupport(multicastSupport)
{

}

void SessionMember::getIPAddress(string &IPAddress) const
{
	in_addr socketIPAddress;
	socketIPAddress.s_addr = htonl(mIPAddress);
	IPAddress = ::inet_ntoa(socketIPAddress);
}
