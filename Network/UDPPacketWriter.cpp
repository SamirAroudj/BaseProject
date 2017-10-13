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
#include <arpa/inet.h>
#include <cstring>
#endif // _WINDOWS

#include "ClientTimeManager.h"
#include "Network.h"
#include "UDPPacketWriter.h"

using namespace Network;
using namespace Platform;

UDPPacketWriter::UDPPacketWriter() :
	PacketWriter(MAX_DATAGRAM_PAYLOAD_SIZE),
	mAckRequestNumber(0),
	mContainsAckRequest(false),
	mContainsTimeRequest(false),
	mContainsUserData(false)
{
	mPacketSize = UDPPacket::HEADER_SIZE;
	memset(mWorkingBuffer, 0, UDPPacket::HEADER_SIZE);
}

void UDPPacketWriter::addAckRequestMessage(uint16 ackNumber)
{
	assert(!mContainsAckRequest && getFreeMemorySize() >= ACK_MESSAGE_SIZE);
	mContainsAckRequest = true;
	mAckRequestNumber = ackNumber;
	insertAckMessage(UDPPacket::ACK_REQUEST, ackNumber, mContainsUserData);
}
		
uint16 UDPPacketWriter::addAckResponseMessage(uint16 ackNumber)
{
	if (getFreeMemorySize() < sizeof(ackNumber) + sizeof(MessageType))
		return 0;
	insertAckMessage(UDPPacket::ACK_RESPONSE, ackNumber, mContainsUserData);
	return ACK_MESSAGE_SIZE;
}

void UDPPacketWriter::insertAckMessage(MessageType messageType, uint16 ackNumber, bool afterHeader)
{
	if (!afterHeader)
	{
		write(messageType);
		write(ackNumber);
		return;
	}

	uint32 tempDataSize = mPacketSize - UDPPacket::HEADER_SIZE;	// put the ack between packet header and remaining data
	char tempData[MAX_DATAGRAM_PAYLOAD_SIZE];
	memcpy(tempData, mWorkingBuffer + UDPPacket::HEADER_SIZE, tempDataSize);

	mPacketSize = UDPPacket::HEADER_SIZE;
	write(messageType);
	write(ackNumber);

	memcpy(mWorkingBuffer + mPacketSize, tempData, tempDataSize);
	mPacketSize += tempDataSize;
}

// data is in Network Byte Order and the structure is: message identifier / message data / message identifier ...
bool UDPPacketWriter::addFormattedUserData(const char *data, uint32 size)
{
	if (size > getFreeMemorySize())
		return false;

	memcpy(mWorkingBuffer + mPacketSize, data, size);
	mPacketSize += size;
	mContainsUserData = true;

	return true;
}

void UDPPacketWriter::addLANServerDiscoveryMessage()
{
	write(UDPPacket::LAN_SERVER_DISCOVERY);
}

void UDPPacketWriter::addLANServerResponseMessage(const ApplicationAddress &serverApp)
{
	write(UDPPacket::LAN_SERVER_RESPONSE);
	write(serverApp.getIPAddress());
	write(serverApp.getPortNumber());
}

uint16 UDPPacketWriter::addTimeRequestMessage(Real clientTime, bool initialRequestMessage)
{
	uint16 additionalMemory = sizeof(MessageType) + sizeof(Real);
	if (UDPPacket::HEADER_SIZE == mPacketSize)
	{
		write((initialRequestMessage) ? UDPPacket::TIME_INITIAL_REQUEST : UDPPacket::TIME_UPDATE_REQUEST);
		write(clientTime);
		mContainsTimeRequest = true;
		return additionalMemory;
	}
	if (getFreeMemorySize() < additionalMemory)
		return 0;

	char buffer[MAX_DATAGRAM_PAYLOAD_SIZE]; // make sure that this is the first message in the packet in order to be quickly processed by the server
	uint32 oldPayloadSize = mPacketSize - UDPPacket::HEADER_SIZE;
	
	memcpy(buffer, mWorkingBuffer + UDPPacket::HEADER_SIZE, oldPayloadSize);
	mPacketSize = UDPPacket::HEADER_SIZE;
	write((initialRequestMessage) ? UDPPacket::TIME_INITIAL_REQUEST : UDPPacket::TIME_UPDATE_REQUEST);
	write(clientTime);
	memcpy(mWorkingBuffer + mPacketSize, buffer, oldPayloadSize);
	mPacketSize += oldPayloadSize;
	mContainsTimeRequest = true;
	return additionalMemory;
}

void UDPPacketWriter::addTimeResponseMessage(Real clientRequestTime, Real serverTime)
{
	assert(UDPPacket::HEADER_SIZE == mPacketSize);
	write(UDPPacket::TIME_RESPONSE);
	write(clientRequestTime);
	write(serverTime);
}
		
void UDPPacketWriter::endPacketAssembly()
{
	mPacketSize = UDPPacket::HEADER_SIZE;
	mContainsUserData = mContainsAckRequest = mContainsTimeRequest = false;
}

const char *UDPPacketWriter::getPacketData() const
{
	if (UDPPacket::HEADER_SIZE == mPacketSize)
		return NULL;

	return mWorkingBuffer;
}

bool UDPPacketWriter::isEnoughMemoryFree(uint32 &requiredMemorySize, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
										 UDPPacket::Delivery delivery) const
{
	requiredMemorySize = sizeof(MessageType);
	for(uint32 i = 0; i < numOfSerializables; ++i)
		requiredMemorySize += serializables[i].getRequiredSerializationMemory();
	if (delivery == UDPPacket::LOSS_INFO || delivery == UDPPacket::RELIABLE)
		requiredMemorySize += (mContainsAckRequest) ? 0 : ACK_MESSAGE_SIZE;
	return getFreeMemorySize() >= requiredMemorySize;
}

void UDPPacketWriter::setNetworkTime(Real networkTime)
{
	memcpy(mWorkingBuffer, &networkTime, sizeof(networkTime));
}