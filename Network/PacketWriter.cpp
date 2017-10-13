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

#include "PacketWriter.h"

using namespace Network;
using namespace std;

PacketWriter::PacketWriter(uint32 maxPacketSize) : mPacketSize(0), mMaxPacketSize(maxPacketSize)
{ }

PacketWriter::~PacketWriter()
{

}

void PacketWriter::write(bool byte, char *buffer)
{
	memcpy(buffer + mPacketSize, &byte, sizeof(bool));
	mPacketSize += sizeof(bool);
	assert(mMaxPacketSize >= mPacketSize);
}

void PacketWriter::write(char byte, char *buffer)
{
	memcpy(buffer + mPacketSize, &byte, sizeof(char));
	mPacketSize += sizeof(char);
	assert(mMaxPacketSize >= mPacketSize);
}

void PacketWriter::write(const std::string &text, char *buffer)
{	// paste a text into the packet and terminate it with '\0'
	memcpy(buffer + mPacketSize, text.c_str(), text.length());
	mPacketSize += text.length();
	buffer[mPacketSize] = '\0';
	++mPacketSize;
	assert(mMaxPacketSize >= mPacketSize);
}		

void PacketWriter::write(int16 value, char *buffer)
{
	value = htons(value);
	memcpy(buffer + mPacketSize, &value, sizeof(int16));
	mPacketSize += sizeof(int16);
	assert(mMaxPacketSize >= mPacketSize);
}

void PacketWriter:: write(uint16 value, char *buffer)
{
	value = htons(value);
	memcpy(buffer + mPacketSize, &value, sizeof(uint16));
	mPacketSize += sizeof(uint16);
	assert(mMaxPacketSize >= mPacketSize);
}

void PacketWriter::write(int32 value, char *buffer)
{
	value = htonl(value);
	memcpy(buffer + mPacketSize, &value, sizeof(int32));
	mPacketSize += sizeof(int32);
	assert(mMaxPacketSize >= mPacketSize);
}

void PacketWriter::write(uint32 value, char *buffer)
{
	value = htonl(value);
	memcpy(buffer + mPacketSize, &value, sizeof(uint32));
	mPacketSize += sizeof(uint32);
	assert(mMaxPacketSize >= mPacketSize);
}

void PacketWriter::write(Real value, char *buffer)
{
	memcpy(buffer + mPacketSize, &value, sizeof(Real));
	mPacketSize += sizeof(Real);
	assert(mMaxPacketSize >= mPacketSize);
}

void PacketWriter::write(double value, char *buffer)
{
	memcpy(buffer + mPacketSize, &value, sizeof(double));
	mPacketSize += sizeof(double);
	assert(mMaxPacketSize >= mPacketSize);
}