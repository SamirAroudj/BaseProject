/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _UDP_PACKET_WRITER_H_
#define _UDP_PACKET_WRITER_H_

#include "ApplicationAddress.h"
#include "Platform/ApplicationTimer.h"
#include "ISerializable.h"
#include "MagicNetworkConstants.h"
#include "PacketWriter.h"
#include "UDPPacket.h"

namespace Network
{
	class UDPPacketWriter : public PacketWriter
	{
	public:
		static const uint16 ACK_MESSAGE_SIZE = sizeof(MessageType) + sizeof(uint16);

		UDPPacketWriter();
		virtual ~UDPPacketWriter() {  }

		void addAckRequestMessage(uint16 ackNumber);
		uint16 addAckResponseMessage(uint16 ackNumber);
		// data is in Network Byte Order and the structure is: message identifier / message data / message identifier ...
		bool addFormattedUserData(const char *data, uint32 size);
		void addLANServerDiscoveryMessage();
		void addLANServerResponseMessage(const ApplicationAddress &serverApp);
		uint16 addTimeRequestMessage(Real clientRequestTime, bool initialRequestMessage);
		void addTimeResponseMessage(Real clientRequestTime, Real serverTime);
		
		bool containsAckRequestMessage() const { return mContainsAckRequest; }
		bool containsTimeRequestMessage() const { return mContainsTimeRequest; }

		/// the message can only be added if it fits in one datagram and if its size is smaller than or equal to residualBufferSize
		void endPacketAssembly();
		uint32 getAckRequestMessageSize() const { return ACK_MESSAGE_SIZE; }
		uint16 getAckRequestNumber() const { return mAckRequestNumber; }
		const char *getPacketData() const;
		bool isEnoughMemoryFree(uint32 &requiredMemorySize, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
			UDPPacket::Delivery delivery) const;

		void setNetworkTime(Real time);
		void setUserDataAdded() { mContainsUserData = true; }

		void write(UDPPacket::MessageType messageIdentifier) { PacketWriter::write(static_cast<MessageType>(messageIdentifier), mWorkingBuffer); }
		void write(bool byte) { PacketWriter::write(byte, mWorkingBuffer); }
		void write(char byte) { PacketWriter::write(byte, mWorkingBuffer); }
		void write(const std::string &text) { PacketWriter::write(text, mWorkingBuffer); }
		void write(int16 value) { PacketWriter::write(value, mWorkingBuffer); }
		void write(uint16 value) { PacketWriter::write(value, mWorkingBuffer); }
		void write(int32 value) { PacketWriter::write(value, mWorkingBuffer); }
		void write(uint32 value) { PacketWriter::write(value, mWorkingBuffer); }
		void write(Real value) { PacketWriter::write(value, mWorkingBuffer); }
		void write(double value) { PacketWriter::write(value, mWorkingBuffer); }

	private:
		void insertAckMessage(MessageType messageType, uint16 ackNumber, bool afterHeader);

		char mWorkingBuffer[MAX_DATAGRAM_PAYLOAD_SIZE];
		uint16 mAckRequestNumber;
		bool mContainsAckRequest;
		bool mContainsTimeRequest;
		bool mContainsUserData;

		/// forbidden
		UDPPacketWriter(const UDPPacketWriter &copy) : PacketWriter(0) { assert(false); }
		/// forbidden
		UDPPacketWriter &operator =(const UDPPacketWriter &rhs) { assert(false); return *this; }
	};
}

#endif // _UDP_PACKET_WRITER_H_

