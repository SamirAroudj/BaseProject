/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _LINUX
#include <cstring>
#endif // _LINUX

#include <cassert>
#include <string>
#include "TCPPacket.h"

using namespace Network;


TCPPacket::TCPPacket(const TCPPacket *&packet1, const TCPPacket *&packet2) : Packet(packet1->getDataLength() + packet2->getDataLength(), packet1->getSenderIdentifier())
{
	assert(packet1 && packet2 && packet1 != packet2);
	memcpy(getPrivateData(), packet1->getData(), packet1->getDataLength());
	memcpy(getPrivateData() + packet1->getDataLength(), packet2->getData(), packet2->getDataLength());

	delete packet1;
	delete packet2;
	packet1 = NULL;
	packet2 = NULL;
}

TCPPacket::TCPPacket(TCPPacket *&packet) : Packet(packet->mIdentifier)
{
	mDataLength = packet->mDataLength;
	mData = packet->mData;
	packet = NULL;
}