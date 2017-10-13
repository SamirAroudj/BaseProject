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
#endif // _WINDOW

#include "LANServerAdvertiser.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"
#include "UDPPacketReader.h"
#include "UDPPacketWriter.h"

using namespace ExceptionHandling;
using namespace Network;

LANServerAdvertiser::LANServerAdvertiser(const ApplicationAddress &serverApp, uint16 udpPortNumber, uint32 IPAddress) : mReadyToSend(false)
{
	if (0 == IPAddress)	// primary network interface is used if no IP address of a network interface was specified
	{
		Network::getLANsMachineIsConnectedTo(mLANs);
		IPAddress = mLANs[0].getInterfaceIPAddress();
	}

	sockaddr_in address;	// configure advertisers address
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(IPAddress);
	address.sin_port = htons(udpPortNumber);
	memset(address.sin_zero, 0, 8);

	// create the socket, bind it to the given port and make it nonblocking
	if (INVALID_SOCKET == (mSocketHandle = socket(PF_INET, SOCK_DGRAM, 0)))
		NetworkExceptionFactory::throwNetworkException("Unable to create a udp socket to create responses for LAN server discoveries.", getError());
	char enableReusing = 1;
#ifdef _WINDOWS
	if (SOCKET_ERROR == setsockopt(mSocketHandle, SOL_SOCKET, SO_REUSEADDR, &enableReusing, sizeof(char)))
		NetworkExceptionFactory::throwNetworkException("Unable to enable reusing of addresses for binding.", getError());
#endif // _WINDOWS
	if (SOCKET_ERROR == bind(mSocketHandle, reinterpret_cast<sockaddr *>(&address), sizeof(sockaddr_in)))
		NetworkExceptionFactory::throwSocketBindException("Error, unable to bind the udp LAN server socket to a port.", getError(), udpPortNumber);
	changeToNonblockingMode(mSocketHandle);

	mPacketWriter.addLANServerResponseMessage(serverApp);
}

LANServerAdvertiser::~LANServerAdvertiser()
{
	closeSocket(mSocketHandle);
}

void LANServerAdvertiser::sendResponses()
{
	if (!mReadyToSend || mReceivers.empty())
		return;

	socklen_t addressLength = sizeof(sockaddr);	// prepare address parts
	sockaddr_in receiverAddress;
	receiverAddress.sin_family = AF_INET;
	memset(receiverAddress.sin_zero, 0, 8);

	for(uint32 i = 0; i < mReceivers.size(); ++i)	// send the server application address to all requesting LANServerFinder objects
	{
		receiverAddress.sin_port = htons(mReceivers.front().getPortNumber());
		receiverAddress.sin_addr.s_addr = htonl(mReceivers.front().getIPAddress());

		int32 bytesSent = sendto(mSocketHandle, mPacketWriter.getPacketData(), mPacketWriter.getPacketSize(), 0,
			reinterpret_cast<sockaddr *>(&receiverAddress), addressLength);
		if (SOCKET_ERROR == bytesSent)
		{
			int32 errorCode = getError();
			if (WOULD_BLOCK == errorCode)
				return;
			else
				NetworkExceptionFactory::throwNetworkException("Unable to send datagrams to the server.", errorCode);
		}
		mReceivers.pop(); 
	}
}

void LANServerAdvertiser::update()
{
	bool readable = false;
	Network::getSocketState(mSocketHandle, readable, mReadyToSend);
	if (!readable)
		return;

	char buffer[MAX_DATAGRAM_PAYLOAD_SIZE];
	sockaddr_in remoteAddress;
	socklen_t addressLength = sizeof(sockaddr_in);
	while(true)	// process all received datagrams
	{
		int32 packetLength = recvfrom(mSocketHandle, buffer, MAX_DATAGRAM_PAYLOAD_SIZE, 0, reinterpret_cast<sockaddr *>(&remoteAddress), &addressLength);
		if (SOCKET_ERROR == packetLength)
		{
			int32 errorCode = getError();
			if (WOULD_BLOCK == errorCode)
				break;
			NetworkExceptionFactory::throwNetworkException("Unable to receive datagrams containing LAN server discovery messages.", errorCode);
		}
		uint32 IPAddress = ntohl(remoteAddress.sin_addr.s_addr);
		if (!senderIsLANMember(IPAddress))
			continue;
	
		UDPPacket *packet = new UDPPacket(packetLength, buffer, 0);	/// is it only a LAN_SERVER_DISCOVERY message?
		UDPPacketReader reader;
		reader.beginReading(packet);
		bool rightFormat = reader.readLANServerDiscoveryMessage();
		reader.endReadingAndDestroyPacket();
		if (!rightFormat)
			continue;
		mReceivers.push(ApplicationAddress(IPAddress, ntohs(remoteAddress.sin_port)));
	}
}

bool LANServerAdvertiser::senderIsLANMember(uint32 IPAddress)
{
	for(uint32 i = 0; i < mLANs.size(); ++i)
		if ((mLANs[i].getNetMask() & mLANs[i].getInterfaceIPAddress()) == (mLANs[i].getNetMask() & IPAddress))
			return true;
	return false;
}