/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "UDPSocket.h"
#include "Platform/ApplicationTimer.h"
#include "ClientTimeManager.h"
#include "MagicNetworkConstants.h"
#include "Network.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"
#include "ServerTimeManager.h"
#include "Session.h"
#include "UDPPacketWriter.h"

using namespace ExceptionHandling;
using namespace Network;
using namespace Platform;

UDPSocket::UDPSocket(bool usesBroadcasts) :  
	mSendPeriod(SEND_PERIOD_LENGTH), mSocketHandle(0), mReadyToSend(false)
{
	mSocketHandle = socket(PF_INET, SOCK_DGRAM, 0); // create socket
	if (mSocketHandle == INVALID_SOCKET) // valid socket?
		NetworkExceptionFactory::throwInvalidSocketException("Unable to create a UDP socket.", getError());
	changeToNonblockingMode(mSocketHandle);
    
#ifdef _WINDOWS	// do not fragment datagrams
	char val = 1;
	if (SOCKET_ERROR == setsockopt(mSocketHandle, IPPROTO_IP, IP_DONTFRAGMENT, &val, sizeof(val)))
        NetworkExceptionFactory::throwNetworkException("Unable to disable datagram fragmentation.", getError());
#elif _LINUX
    int val = IP_PMTUDISC_DO;
    if (SOCKET_ERROR == setsockopt(mSocketHandle, SOL_IP, IP_MTU_DISCOVER, &val, sizeof(int)))
        NetworkExceptionFactory::throwNetworkException("Unable to disable datagram fragmentation.", getError());
#endif // _WINDOWS

	if (!usesBroadcasts)
		return;
	if (SOCKET_ERROR == setsockopt(mSocketHandle, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val)))
		NetworkExceptionFactory::throwNetworkException("Unable to enable UDP broadcasts.", getError());
}

UDPSocket::~UDPSocket()
{
	uint32 numOfPackets = mPackets.size();
	for(uint32 i = 0; i < numOfPackets; ++i)
		delete mPackets[i];

	closeSocket(mSocketHandle);
}


void UDPSocket::bind(uint32 IPAddress, uint16 portNumber)
{	
	sockaddr_in socketAddress;	// bind the udp socket to this IP address and the defined port number
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = htons(portNumber);
	socketAddress.sin_addr.s_addr = htonl(IPAddress);
	memset(socketAddress.sin_zero, 0, 8 * sizeof(char));

	if (SOCKET_ERROR == ::bind(mSocketHandle, reinterpret_cast<sockaddr *>(&socketAddress), sizeof(sockaddr_in)))
		NetworkExceptionFactory::throwSocketBindException("Error, unable to bind the udp socket to any free port.", getError(), 0);
}

UDPPacket *UDPSocket::getPacket()
{
	UDPPacket *packet = mPackets.back();
	mPackets.pop_back();
	return packet;
}

uint16 UDPSocket::getPortNumber() const
{
	sockaddr_in socketAddress;
	socklen_t socketAddressLength = sizeof(sockaddr);
	if (SOCKET_ERROR == getsockname(mSocketHandle, reinterpret_cast<sockaddr *>(&socketAddress), &socketAddressLength))
		NetworkExceptionFactory::throwNetworkException("Unable to retrieve udp socket port number.", getError());

	return ntohs(socketAddress.sin_port);
}

bool UDPSocket::notifyReceivingIsPossible()
{
	sockaddr_in remoteAddress;
	socklen_t addressLength = sizeof(sockaddr);
	char buffer[MAX_DATAGRAM_PAYLOAD_SIZE];
	while(true)	// receive every datagram which has been sent to this application
	{
		int32 packetLength = recvfrom(mSocketHandle, buffer, MAX_DATAGRAM_PAYLOAD_SIZE, 0, reinterpret_cast<sockaddr *>(&remoteAddress), &addressLength);
		if (SOCKET_ERROR == packetLength)
		{
			int32 errorCode = getError();
			if (WOULD_BLOCK == errorCode)
				return true;
			NetworkExceptionFactory::throwNetworkException("Unable to receive datagrams.", errorCode);
		}
		if (sizeof(sockaddr) != addressLength)
			continue;

		uint16 identifier;	// filter datagrams,  only datagrams of session members are accepted
		uint32 IPAddress = ntohl(remoteAddress.sin_addr.s_addr);
		uint16 portNumber = ntohs(remoteAddress.sin_port);
		if (Session::getSingleton().isMember(IPAddress, portNumber, identifier))
			processReceivedPacket(new UDPPacket(packetLength, buffer, identifier), remoteAddress);
	}
}

void UDPSocket::processReceivedPacket(UDPPacket *packet, sockaddr_in &sender)
{
	UDPPacketReader reader;
	reader.beginReading(packet);
	while(true)
	{
		if (reader.getCurrentMessageType() >= MIN_USER_UDP_MESSAGE_IDENTIFIER)
		{			
			reader.chopOffReadMessages();	// chop off data which is only relevant for network management
			packet = reader.endReading();
			mPackets.push_back(packet);
			return;
		}

		switch(reader.getCurrentMessageType())
		{
		case UDPPacket::NO_MESSAGE:
			reader.endReadingAndDestroyPacket();
			return;

		case UDPPacket::TIME_RESPONSE:
			ClientTimeManager::getSingleton().processReceivedServerResponse(reader);
			break;

		case UDPPacket::TIME_INITIAL_REQUEST:
			processTimeRequestMessage(reader, sender, true);
			break;

		case UDPPacket::TIME_UPDATE_REQUEST:
			processTimeRequestMessage(reader, sender, false);
			break;

		case UDPPacket::ACK_REQUEST:
		{
			SenderIdentifierAckPair request;
			request.mSenderIdentifier = reader.getSenderIdentifier();
			request.mAckNumber = reader.readAckRequestMessage();
			mAckRequests.push_back(request);
			break;
		}

		case UDPPacket::ACK_RESPONSE:
		{
			SenderIdentifierAckPair response;
			response.mSenderIdentifier = reader.getSenderIdentifier();
			response.mAckNumber = reader.readAckResponseMessage();
			mAckResponses.push_back(response);
			break;
		}

		default:
			assert(false);
		}
	}
}

void UDPSocket::processTimeRequestMessage(UDPPacketReader &reader, sockaddr_in &sender, bool initialTimeRequest)
{	
	UDPPacketWriter writer; // send the response with server clock time as fast as possible
	Real clientRequestTime;

	reader.readTimeRequestMessage(clientRequestTime);
	Real systemTime = Platform::ApplicationTimer::getSingleton().getSystemTime();
	writer.addTimeResponseMessage(clientRequestTime, systemTime);
	sendto(mSocketHandle, writer.getPacketData(), writer.getPacketSize(), 0, reinterpret_cast<sockaddr *>(&sender), sizeof(sockaddr));

	if (!initialTimeRequest)	// compute round trip time
	{
		Real roundTripTime;
		if (clientRequestTime > systemTime)
			roundTripTime = 0.0f;	// might be necessary thanks to drifting clocks, providing that round trip time is very short
		else
			roundTripTime = 2 * (systemTime - clientRequestTime);
		if (roundTripTime < 1.0f)	// do not add round trip times which are much too long
			ServerTimeManager::getSingleton().addRoundTripTime(reader.getSenderIdentifier(), roundTripTime);
	}

	writer.endPacketAssembly();
}