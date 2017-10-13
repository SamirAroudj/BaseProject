/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "MessageProvider.h"

using namespace Network;

MessageProvider::MessageProvider()
{
}

MessageProvider::~MessageProvider()
{
	while(!mTCPPackets.empty())	// free received packets
	{
		delete mTCPPackets.front();
		mTCPPackets.pop();
	}

	while(!mUDPPackets.empty())
	{
		delete mUDPPackets.back();
		mUDPPackets.pop_back();
	}

	mTCPPacketReader.endReadingAndDestroyPacket();
	mUDPPacketReader.endReadingAndDestroyPacket();
}

void MessageProvider::add(TCPPacket *&packet)
{
	if (TCPPacket::NO_MESSAGE == mTCPPacketReader.getCurrentMessageType())
		mTCPPacketReader.beginReading(packet);
	else
		mTCPPackets.push(packet);
	packet = NULL;
}

void MessageProvider::add(UDPPacket *&packet)
{
	if (UDPPacket::NO_MESSAGE == mUDPPacketReader.getCurrentMessageType())
		mUDPPacketReader.beginReading(packet);
	else
		mUDPPackets.push_back(packet);
	packet = NULL;
}

MessageType MessageProvider::getCurrentTCPMessageIdentifier(uint16 &senderIdentifier) const
{
	senderIdentifier = mTCPPacketReader.getSenderIdentifier();
	return mTCPPacketReader.getCurrentMessageType();
}

MessageType MessageProvider::getCurrentUDPMessageIdentifier(uint16 &senderIdentifier) const
{
	senderIdentifier = mUDPPacketReader.getSenderIdentifier();
	return mUDPPacketReader.getCurrentMessageType();
}

void MessageProvider::getTCPMessage(Patterns::ISerializable *serializables, uint32 numOfSerializables)
{
	assert(TCPPacket::NO_MESSAGE != mTCPPacketReader.getCurrentMessageType() && numOfSerializables > 0);
	bool finishedPacket = false;
	readMessage(serializables, numOfSerializables, mTCPPacketReader, finishedPacket);
	if (finishedPacket && !mTCPPackets.empty())
		beginTCPPacketReading();
}

void MessageProvider::getUDPMessage(Patterns::ISerializable *serializables, uint32 numOfSerializables)
{
	assert(UDPPacket::NO_MESSAGE != mUDPPacketReader.getCurrentMessageType() && numOfSerializables > 0);
	bool finishedPacket = false;
	readMessage(serializables, numOfSerializables, mUDPPacketReader, finishedPacket);
	if (finishedPacket && !mUDPPackets.empty())
		beginUDPPacketReading();
}

void MessageProvider::readMessage(Patterns::ISerializable *serializables, uint32 numOfSerializables, PacketReader &packetReader, bool &finishedPacket)
{
	assert(serializables && numOfSerializables > 0);
	bool finished = false;
	for(uint32 i = 0; i < numOfSerializables; ++i)
		serializables[i].deserialize(packetReader);
	packetReader.readNextMessageType();
	if (0 != packetReader.getCurrentMessageType())
		return;

	packetReader.endReadingAndDestroyPacket();
	finishedPacket = true;
}

void MessageProvider::beginTCPPacketReading()
{
	TCPPacket *packet = mTCPPackets.front();
	mTCPPackets.pop();
	mTCPPacketReader.beginReading(packet);
}

void MessageProvider::beginUDPPacketReading()
{
	UDPPacket *packet = mUDPPackets.back();
	mUDPPackets.pop_back();
	mUDPPacketReader.beginReading(packet);
}