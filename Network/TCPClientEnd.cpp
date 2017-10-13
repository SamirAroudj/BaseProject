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
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif // _WINDOWS

#include <cassert>
#include "Network.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"
#include "TCPClientEnd.h"
#include "TCPPacketWriter.h"

using namespace ExceptionHandling;
using namespace Network;
using namespace std;


TCPClientEnd::TCPClientEnd(int32 socketHandle) : 
	TCPEnd(socketHandle), 
	mPartialSentPacket(NULL), mPendingPacket(NULL),
	mNumberOfPendingBytes(0)
{

}

TCPClientEnd::~TCPClientEnd()
{
	delete mPendingPacket;
	delete mPartialPacket;
}

void TCPClientEnd::connect(const ApplicationAddress &serverAddress)
{	
	// prepare the destination description: IP address + port number
	sockaddr_in remoteAddress;
	remoteAddress.sin_family = AF_INET;
	remoteAddress.sin_addr.s_addr = htonl(serverAddress.getIPAddress());
	remoteAddress.sin_port = htons(serverAddress.getPortNumber());
	memset(remoteAddress.sin_zero, 0, 8);

	// and connect to the device indicated by the created address
	if (SOCKET_ERROR != ::connect(mSocketHandle, reinterpret_cast<sockaddr *>(&remoteAddress), sizeof(sockaddr)))
		return;

	int32 errorCode = getError();
#ifdef _WINDOWS
	if (WOULD_BLOCK != errorCode)	// a WSAEWOULDBLOCK error should occur in most cases since the socket is nonblocking and a connection cannot be established immediately
#else
	if (EINPROGRESS != errorCode)
#endif // _WINDOWS
	{
		NetworkExceptionFactory::throwNetworkConnectionCreationException("Could not establish a connection.", 
			ConnectionCreationException::SOCKET_PROBLEM, errorCode,
			convertAddressHostByteOrderToASCII(serverAddress.getIPAddress()), serverAddress.getPortNumber());
	}
}

bool TCPClientEnd::notifyReceivingIsPossible()
{
	mReadyToSend = true;
	return receive();
}		

void TCPClientEnd::addMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables)
{
	assert(serializables && numOfSerializables > 0);
	const TCPPacket *packet = createTCPPacket(messageIdentifier, serializables, numOfSerializables, mIdentifier);
	mNumberOfPendingBytes += packet->getLengthHeaderInclusive();
	mPendingPacket = (mPendingPacket) ? new TCPPacket(mPendingPacket, packet) : packet;
}

void TCPClientEnd::addPacket(const TCPPacket *&packet, bool appendToFront)
{
	if (!mPendingPacket)
	{
		mPendingPacket = packet;
		return;
	}

	mPendingPacket = (appendToFront) ? new TCPPacket(packet, mPendingPacket) : new TCPPacket(mPendingPacket, packet);
}

void TCPClientEnd::send()
{
	if (!mReadyToSend)
		return;
	
	if (mPartialSentPacket)	// packet couldn't be sent completely last time so try to finish it at first
	{						// it's important that no other packet is sent inbetween the start and end of a transmission of one single packet
		uint32 sentBytes = 0;
		bool packetDispatched = sendOnePacket(*mPartialPacket, sentBytes);
		mNumberOfPendingBytes -= sentBytes;
		if (!packetDispatched)
			return;
		delete mPartialSentPacket;
		mPartialPacket = NULL;
	}

	if (mPendingPacket) // if sending isn't completely possible then it will become the mPartialSentPacket to be sent at first when function is called again
	{
		uint32 sentBytes = 0;
		bool packetDispatched = sendOnePacket(*mPendingPacket, sentBytes);
		mNumberOfPendingBytes -= sentBytes;
		if (!packetDispatched)
		{
			mPartialSentPacket = mPendingPacket;
			mPendingPacket = NULL;
			return;
		}
		delete mPendingPacket;
		mPendingPacket = NULL;
	}
}