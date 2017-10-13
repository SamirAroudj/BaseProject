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
#endif // _WINDOWS

#include "PacketReader.h"

using namespace Network;
using std::string;

PacketReader::PacketReader() : mPacket(NULL), mCurrentPosition(0), mCurrentMessageType(0)
{ }


PacketReader::~PacketReader()
{	// no memory leak?
	assert((NULL == mPacket) && (0 == mCurrentPosition) && (0 == mCurrentMessageType));
}

void PacketReader::chopOffReadMessages()
{
	assert(mCurrentPosition != mPacket->getDataLength());	// do not chop off data when everything or no message was read
	if (0 == mCurrentPosition - sizeof(MessageType))
		return;
	mPacket->chopOffFront(mCurrentPosition - sizeof(MessageType));
}

Packet *PacketReader::endReading()
{
	Packet *p = mPacket;	// go to initial state and return the packet
	mPacket = NULL;
	mCurrentPosition = 0;
	mCurrentMessageType = 0;

	return p;
}

void PacketReader::endReadingAndDestroyPacket()
{
	delete mPacket;	// go to initial state and free the packet
	mPacket = NULL;
	mCurrentPosition = 0;
	mCurrentMessageType = 0;
}

bool PacketReader::readBool()
{
	bool byte;
	memcpy(&byte, mPacket->getData() + mCurrentPosition, sizeof(bool));
	mCurrentPosition += sizeof(bool);

	assert(mCurrentPosition <= mPacket->getDataLength());
	return byte;
}

char PacketReader::readChar()
{
	char byte;
	memcpy(&byte, mPacket->getData() + mCurrentPosition, sizeof(char));
	mCurrentPosition += sizeof(char);
	
	assert(mCurrentPosition <= mPacket->getDataLength());
	return byte;
}

void PacketReader::readString(string &text)
{
	uint32 textLength = 0;	// read one null-terminated string
	while(mPacket->getData()[mCurrentPosition + textLength] != '\0')
		++textLength;

	text.assign(mPacket->getData() + mCurrentPosition, textLength);
	mCurrentPosition += textLength + sizeof(char);
	assert(mCurrentPosition <= mPacket->getDataLength());
}

int16 PacketReader::readInt16()
{
	int16 value;
	memcpy(&value, mPacket->getData() + mCurrentPosition, sizeof(int16));
	value = ntohs(value);
	mCurrentPosition += sizeof(int16);

	assert(mCurrentPosition <= mPacket->getDataLength());
	return value;
}

uint16 PacketReader::readUInt16()
{
	uint16 value;
	memcpy(&value, mPacket->getData() + mCurrentPosition, sizeof(uint16));
	value = ntohs(value);
	mCurrentPosition += sizeof(uint16);

	assert(mCurrentPosition <= mPacket->getDataLength());
	return value;
}

int32 PacketReader::readInt32()
{
	int32 value;
	memcpy(&value, mPacket->getData() + mCurrentPosition, sizeof(int32));
	value = ntohl(value);
	mCurrentPosition += sizeof(int32);

	assert(mCurrentPosition <= mPacket->getDataLength());
	return value;
}

uint32 PacketReader::readUInt32()
{
	uint32 value;
	memcpy(&value, mPacket->getData() + mCurrentPosition, sizeof(uint32));
	value = ntohl(value);
	mCurrentPosition += sizeof(uint32);

	assert(mCurrentPosition <= mPacket->getDataLength());
	return value;
}

Real PacketReader::readFloat()
{
	Real value;
	memcpy(&value, mPacket->getData() + mCurrentPosition, sizeof(Real));
	mCurrentPosition += sizeof(Real);

	assert(mCurrentPosition <= mPacket->getDataLength());
	return value;
}

double PacketReader::readDouble()
{
	double value;
	memcpy(&value, mPacket->getData() + mCurrentPosition, sizeof(double));
	mCurrentPosition += sizeof(double);

	assert(mCurrentPosition <= mPacket->getDataLength());
	return value;
}

void PacketReader::beginReading(Packet *packet)
{
	assert((NULL == mPacket) && (0 == mCurrentPosition) && (0 == mCurrentMessageType));
	mPacket = packet;	// you must finish any previous packet before beginning to analyse the next one since someone must take care of freeing the packet memory
}

void PacketReader::readNextMessageType()
{
	if (mPacket->getDataLength() == mCurrentPosition)
	{
		mCurrentMessageType = 0;
		return;
	}

	memcpy(&mCurrentMessageType, mPacket->getData() + mCurrentPosition, sizeof(MessageType));
	if (sizeof(MessageType) == sizeof(uint16))
		mCurrentMessageType = static_cast<MessageType>(ntohs(mCurrentMessageType));
	if (sizeof(MessageType) == sizeof(uint32))
		mCurrentMessageType = static_cast<MessageType>(ntohl(mCurrentMessageType));
	mCurrentPosition += sizeof(MessageType);
}