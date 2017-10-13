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

#include <cassert>
#include "Packet.h"

using namespace Network;

Packet::Packet(uint16 dataLength, uint16 identifier) : mDataLength(dataLength), mIdentifier(identifier)
{
	initialize();
}

Packet::Packet(uint16 dataLength, const char *data, uint16 identifier) : mDataLength(dataLength), mIdentifier(identifier)
{
	initialize();
	memcpy(mData + sizeof(uint16), data, mDataLength);
}

void Packet::initialize()
{
	mData = new char[mDataLength + sizeof(uint16)]; // create a packet with length = header length + data length
	uint16 dataLength = htons(mDataLength);				// header structure: length (2 bytes)
	memcpy(mData, &dataLength, sizeof(uint16));
}

Packet::Packet(const Packet &copy) : mIdentifier(copy.mIdentifier), mDataLength(copy.mDataLength)
{
	mData = new char[mDataLength + sizeof(uint16)];
	memcpy(mData, copy.mData, mDataLength + sizeof(uint16));
}

Packet &Packet::operator =(const Packet &rhs)
{
	if (this == &rhs)
		return *this;

	delete [] mData;
	mDataLength = rhs.mDataLength;
	mData = new char[mDataLength + sizeof(uint16)];
	memcpy(mData, rhs.mData, mDataLength + sizeof(uint16));

	return *this;
}

void Packet::chopOffFront(uint32 numOfBytes)
{
	assert(numOfBytes < mDataLength);
	char *newData = new char[mDataLength - numOfBytes + sizeof(uint16)];
	memcpy(newData, mData, sizeof(uint16));
	memcpy(newData + sizeof(uint16), mData + sizeof(uint16) + numOfBytes, mDataLength - numOfBytes);
	mDataLength -= numOfBytes;

	delete [] mData;
	mData = newData;
}