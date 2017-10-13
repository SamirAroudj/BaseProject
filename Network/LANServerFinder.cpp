/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
#include <WinSock2.h>
#elif _LINUX
#include <cstring>
#endif // _WINDOWS

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

#include "LANServerFinder.h"
#include "Network.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"
#include "UDPPacketReader.h"
#include "UDPPacketWriter.h"

using namespace ExceptionHandling;
using namespace Network;
using namespace std;

LANServerFinder::LANServerFinder(uint16 LANServerAdvertiserUDPPort) :
	mSocketHandle(INVALID_SOCKET), mLANServerAdvertiserUDPPort(LANServerAdvertiserUDPPort), mReadyToSend(false)
{ 
	// create the socket and make it nonblocking
	if (INVALID_SOCKET == (mSocketHandle = socket(PF_INET, SOCK_DGRAM, 0)))
		NetworkExceptionFactory::throwNetworkException("Unable to create a udp socket to send LAN server discovery messages.", getError());
	changeToNonblockingMode(mSocketHandle);
	char value = 1;
	setsockopt(mSocketHandle, SOL_SOCKET, SO_BROADCAST, &value, sizeof(char));

	getLANsMachineIsConnectedTo(mLANs);
}

LANServerFinder::~LANServerFinder()
{
	closeSocket(mSocketHandle);
}

void LANServerFinder::sendDiscoveryMessage()
{
	if (!mReadyToSend)
		return;

	UDPPacketWriter writer;	// prepare receiver address and message to send
	writer.addLANServerDiscoveryMessage();
	sockaddr_in broadcastAddress;
	broadcastAddress.sin_family = AF_INET;
	broadcastAddress.sin_port = htons(mLANServerAdvertiserUDPPort);
	memset(broadcastAddress.sin_zero, 0, 8);

	for(uint32 i = 0; i < mLANs.size(); ++i)	// send to all LANs the machine is connected to
	{
		broadcastAddress.sin_addr.s_addr = htonl(mLANs[i].getBroadcastAddress());
		if (SOCKET_ERROR == sendto(mSocketHandle, writer.getPacketData(), writer.getPacketSize(), 0,
			reinterpret_cast<sockaddr *>(&broadcastAddress), sizeof(sockaddr)))
		{
			int32 errorCode = getError();
			if (WOULD_BLOCK == errorCode)
			{
				mReadyToSend = false;
				return;
			}

			NetworkExceptionFactory::throwNetworkException("Unable to send LAN server discovery messages", errorCode);
		}
	}
}

void LANServerFinder::update()
{
	bool readable;
	Network::getSocketState(mSocketHandle, readable, mReadyToSend);
	if (!readable)
		return;
	
	ApplicationAddress serverApp;
	UDPPacketReader reader;
	char buffer[MAX_DATAGRAM_PAYLOAD_SIZE];
	sockaddr_in remoteAddress;
	socklen_t addressLength = sizeof(sockaddr);
	while(true) // read data from the socket for every received datagram
	{	
		int32 numOfReceivedBytes = recvfrom(mSocketHandle, buffer, MAX_DATAGRAM_PAYLOAD_SIZE, 0, reinterpret_cast<sockaddr *>(&remoteAddress), &addressLength);
		if (SOCKET_ERROR == numOfReceivedBytes)
		{
			int32 errorCode = getError();
			if (WOULD_BLOCK == errorCode)
				return;
			NetworkExceptionFactory::throwNetworkException("Unable to receive LAN server advertisement messages.", errorCode);
		}
		UDPPacket *packet = new UDPPacket(numOfReceivedBytes, buffer, 0);	// create UDPPacket and check its message format
		reader.beginReading(packet);
		bool rightFormat = reader.readLANServerResponseMessage(serverApp);
		reader.endReadingAndDestroyPacket();
		if (!rightFormat)
			continue;

		addLANServer(serverApp);
	}
}

void LANServerFinder::addLANServer(const ApplicationAddress &serverApp)
{
	for(uint32 i = 0; i < mDiscoveredServers.size(); ++i)
		if (serverApp == mDiscoveredServers[i])
			return;

	mDiscoveredServers.push_back(serverApp);
#ifdef _DEBUG
		cout << "Found a server application.\n" << serverApp << endl;
#endif // _DEBUG
}
