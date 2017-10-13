/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _NETWORK_H_
#define	_NETWORK_H_

#ifdef _LINUX
#include <cerrno>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif // _LINUX

#include <vector>
#include "LANInfo.h"
#include "TCPPacket.h"
#include "UDPPacket.h"

/// Contained code implements a client-server architecture.
/** There is functionality to send and receive data over different kinds of channels that make use of TCP or UDP.
	The best choice of a channel highly depends on the requirements of the data to be sent. */
namespace Network
{
#ifdef _WINDOWS
	typedef int32 socklen_t;
#endif // _WINDOWS
	typedef char MessageType;

	void changeToNonblockingMode(int32 socketHandle);
	void closeSocket(int32 socketHandle);
	char *convertAddressHostByteOrderToASCII(uint32 IPAddress);
	int32 getError();
	/// the first LANInfo in the vector contains the IP address of the primary network interface of the machine
	void getLANsMachineIsConnectedTo(std::vector<LANInfo> &LANs);
	// interfaceIPAddress in host byte order
	bool getMulticastSupport(uint32 socketHandle, uint32 interfaceIPAddress);
	void getSocketState(int32 socketHandle, bool &readable, bool &writable);
	void initializeNetwork();
	void shutDownNetwork();
}

#endif	/* _NETWORK_H_ */

