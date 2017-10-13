/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "UDPPacketReader.h"

using namespace Network;
using namespace Platform;

UDPPacketReader::UDPPacketReader()
{
}


UDPPacketReader::~UDPPacketReader()
{
}

void UDPPacketReader::beginReading(UDPPacket *&packet)
{
	PacketReader::beginReading(packet);
	packet = NULL;

	readNextMessageType();
}

uint16 UDPPacketReader::readAckRequestMessage()
{
	assert(UDPPacket::ACK_REQUEST == mCurrentMessageType);
	uint16 ackRequestNumber = readUInt16();
	readNextMessageType();
	return ackRequestNumber;
}

uint16 UDPPacketReader::readAckResponseMessage()
{
	assert(UDPPacket::ACK_RESPONSE == mCurrentMessageType);
	uint16 ackResponseNumber = readUInt16();
	readNextMessageType();
	return ackResponseNumber;
}

bool UDPPacketReader::readLANServerDiscoveryMessage()
{
	return mPacket->getDataLength() == sizeof(MessageType) &&
		UDPPacket::LAN_SERVER_DISCOVERY == mCurrentMessageType;
}

bool UDPPacketReader::readLANServerResponseMessage(ApplicationAddress &serverApp)
{
	if (mPacket->getDataLength() != (sizeof(MessageType) + sizeof(uint32) + sizeof(uint16)) || 
		UDPPacket::LAN_SERVER_RESPONSE != mCurrentMessageType)
		return false;
	serverApp.setIPAddress(readUInt32());
	serverApp.setPortNumber(readUInt16());
	return true;
}

void UDPPacketReader::readTimeRequestMessage(Real &clientRequestTime)
{
	assert((mCurrentMessageType == UDPPacket::TIME_INITIAL_REQUEST) || (mCurrentMessageType == UDPPacket::TIME_UPDATE_REQUEST));
	clientRequestTime = readFloat();
	readNextMessageType();
}

void UDPPacketReader::readTimeResponseMessage(Real &clientRequestTime, Real &serverTime)
{
	assert(mCurrentMessageType == UDPPacket::TIME_RESPONSE);
	clientRequestTime = readFloat();
	serverTime = readFloat();
	readNextMessageType();
}
