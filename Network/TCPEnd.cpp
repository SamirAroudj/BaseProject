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
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif // _WINDOWS

#include <cassert>
#include "MagicNetworkConstants.h"
#include "Network.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"
#include "TCPEnd.h"
#include "TCPPacketWriter.h"

using ExceptionHandling::NetworkExceptionFactory;
using namespace Network;
using namespace std;

TCPEnd::TCPEnd(int32 socketHandle) :
			mPartialPacket(NULL),
			mReceivedPacketBytes(0),
			mSocketHandle(socketHandle),
			mSentPacketBytes(0),
			mIdentifier(0),
			mReadyToSend(false),
			mStoppedSending(false)
{
	if (mSocketHandle == INVALID_SOCKET) // valid socket?
		NetworkExceptionFactory::throwInvalidSocketException("Unable to create a TCP socket.", getError());

#ifdef _WINDOWS
	char disable = 1;
#else
	int32 disable = 1; // do not delay data
#endif // _WINDOWS
	if (SOCKET_ERROR == setsockopt(mSocketHandle, IPPROTO_TCP, TCP_NODELAY, &disable, sizeof(disable)))
		NetworkExceptionFactory::throwNetworkException("Unable to disable the Nagle Algorithm.", getError());
	changeToNonblockingMode(mSocketHandle);
}

TCPEnd::~TCPEnd()
{
	closeSocket(mSocketHandle);
	while(!mReceivedPackets.empty()) // delete pending packets
	{
		Packet *p = mReceivedPackets.front();
		mReceivedPackets.pop();
		delete p;
	}

	if (mPartialPacket)
		delete mPartialPacket;
}

uint32 TCPEnd::getIPAddress() const
{
	sockaddr_in socketAddress;
	socklen_t socketAddressLength = sizeof(sockaddr); 
	if (SOCKET_ERROR == getsockname(mSocketHandle, reinterpret_cast<sockaddr *>(&socketAddress), &socketAddressLength))
		NetworkExceptionFactory::throwNetworkException("Could not retrieve TCP socket address.", getError());

	return ntohl(socketAddress.sin_addr.s_addr);
}

void TCPEnd::notifySendingIsPossible()
{
	mReadyToSend = true;
}

bool TCPEnd::notifyReceivingIsPossible()
{
	return receive();
}

bool TCPEnd::receive()
{
	while(true) // receive data as long as there is data
	{			// either while loop is aborted by an error since the connection was closed or there is no more data so that isThereNoDataOrError returns true
		if (!mPartialPacket)
		{
			uint16 dataLength; // start to build a new packet -> determine it's length
			int32 receivedBytes = recv(mSocketHandle, reinterpret_cast<char *>(&dataLength), sizeof(uint16), 0);
			if (isThereNoDataOrError(receivedBytes) || receivedBytes == 1)  // segment doesn'tt obey protocol rules, there must be 2 bytes containing the segment size at front
				return true;
			if (receivedBytes == 0)	// connection was closed by the other side
				return false;
			mPartialPacket = new TCPPacket(ntohs(dataLength), mIdentifier);
		}
		
		//finish reassambling the partial packet
		int32 receivedBytes = recv(mSocketHandle, mPartialPacket->getPrivateData() + mReceivedPacketBytes, mPartialPacket->getDataLength() - mReceivedPacketBytes, 0);
		if (isThereNoDataOrError(receivedBytes))
			return true;
		assert(receivedBytes != 0);
		mReceivedPacketBytes += receivedBytes;

		if (mPartialPacket->getDataLength() == mReceivedPacketBytes) // packet is complete?
		{
			mReceivedPacketBytes = 0;
			mReceivedPackets.push(mPartialPacket);
			mPartialPacket = NULL;
		}
	}
}

bool TCPEnd::isThereNoDataOrError(int32 receivedBytes) const
{
	if (SOCKET_ERROR == receivedBytes)
	{
		int32 errorCode = getError();
		if (WOULD_BLOCK == errorCode)	// there is no data to receive anymore
			return true;
		else
			NetworkExceptionFactory::throwNetworkException("Cannot receive further packets.", errorCode);
	}

	return false;
}

void TCPEnd::stopSending() // close the connection from this side, do not send anymore -> FIN flag is sent to the other side
{
	shutdown(mSocketHandle, SD_SEND);
	mStoppedSending = true;
}

TCPPacket *TCPEnd::getPacket()
{
	assert(mReceivedPackets.size());
	TCPPacket *p = mReceivedPackets.front();
	mReceivedPackets.pop();
	return p;
}

TCPPacket *TCPEnd::createTCPPacket(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
								   uint16 senderIdentifier)
{
	assert(messageIdentifier >= TCPPacket::NUM_OF && serializables && numOfSerializables > 0);
	TCPPacketWriter writer;
	writer.write(messageIdentifier);
	for(uint32 i = 0; i < numOfSerializables; ++i)
		serializables[i].serialize(writer);
	return writer.endPacketAssembly(senderIdentifier);
}


bool TCPEnd::sendOnePacket(const TCPPacket &packet, uint32 &sentBytesThisCall)
{
	sentBytesThisCall = 0;
	if (!mReadyToSend || mStoppedSending)
		return false;

	while(packet.getLengthHeaderInclusive() > mSentPacketBytes)	// try to send until the complete packet is dispatched as long as there is free sending buffer space
	{
		int32 sentBytes = ::send(mSocketHandle, packet.getDataWithHeader() + mSentPacketBytes, packet.getLengthHeaderInclusive() - mSentPacketBytes, 0);
		if (SOCKET_ERROR == sentBytes) // error or full sending buffer
		{
			int32 errorCode = getError();
			if (WOULD_BLOCK == errorCode) // outgoing buffer is full, so stop sending
			{
				mReadyToSend = false;
				return false;
			}

			NetworkExceptionFactory::throwNetworkException("Cannot send further packets.", getError());
		}
		mSentPacketBytes += sentBytes;
		sentBytesThisCall += sentBytes;
	}
	
	mSentPacketBytes = 0;
	return true;
}