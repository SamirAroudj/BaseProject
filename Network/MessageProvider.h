/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _MESSAGE_PROVIDER_H_
#define _MESSAGE_PROVIDER_H_

#include <queue>
#include <vector>
#include "ISerializable.h"
#include "TCPPacketReader.h"
#include "UDPPacketReader.h"

namespace Network
{
	class MessageProvider
	{
	public:
		MessageProvider();
		~MessageProvider();

		void add(TCPPacket *&packet);
		void add(UDPPacket *&packet);

		MessageType getCurrentTCPMessageIdentifier(uint16 &senderIdentifier) const;
		MessageType getCurrentUDPMessageIdentifier(uint16 &senderIdentifier) const;
		void getTCPMessage(Patterns::ISerializable *serializables, uint32 numOfSerializables);
		void getUDPMessage(Patterns::ISerializable *serializables, uint32 numOfSerializables);
		Real getUDPMessageNetworkTime() const { return mUDPPacketReader.getPacketNetworkTime(); }

	private:
		void beginTCPPacketReading();
		void beginUDPPacketReading();
		void readMessage(Patterns::ISerializable *serializables, uint32 numOfSerializables, PacketReader &packetReader, bool &finishedPacket);

		std::queue<TCPPacket *> mTCPPackets;
		std::vector<UDPPacket *> mUDPPackets;
		TCPPacketReader mTCPPacketReader;
		UDPPacketReader mUDPPacketReader;

		///forbidden
		MessageProvider(const MessageProvider &copy) { assert(false); }
		///forbidden
		MessageProvider &operator =(const MessageProvider &rhs) { assert(false); return *this; }
	};
}

#endif // _MESSAGE_PROVIDER_H_

