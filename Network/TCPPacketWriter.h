/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _TCP_PACKET_WRITER_H_
#define _TCP_PACKET_WRITER_H_

#include "MagicNetworkConstants.h"
#include "PacketWriter.h"
#include "Session.h"
#include "TCPPacket.h"

namespace Network
{
	class TCPPacketWriter : public PacketWriter
	{
	public:
		TCPPacketWriter();
		virtual ~TCPPacketWriter();

		// ip address should be in member byte order
		bool addSessionIsFullMessage();
		bool addSessionNewMemberMessage(const SessionMember *member);
		bool addSessionRemoveMemberMessage(uint16 identifier);
		bool addSessionRequestMessage(uint32 IPAddress, uint16 ownUDPPortNumber, const std::string &password, bool multicastSupport);
		bool addSessionResponseMessage(Session *session, uint16 remoteIdentifier);
		bool addSessionWrongPasswordMessage();

		void beginPacketAssembly();
		/// returns NULL if no message was added
		virtual TCPPacket *endPacketAssembly(uint16 senderIdentifier);

		void write(TCPPacket::MessageType messageIdentifier) { PacketWriter::write(static_cast<MessageType>(messageIdentifier), mWorkingBuffer); }
		virtual void write(bool byte) { PacketWriter::write(byte, mWorkingBuffer); }
		virtual void write(char byte) { PacketWriter::write(byte, mWorkingBuffer); }
		virtual void write(const std::string &text) { PacketWriter::write(text, mWorkingBuffer); }
		virtual void write(int16 value) { PacketWriter::write(value, mWorkingBuffer); }
		virtual void write(uint16 value) { PacketWriter::write(value, mWorkingBuffer); }
		virtual void write(int32 value) { PacketWriter::write(value, mWorkingBuffer); }
		virtual void write(uint32 value) { PacketWriter::write(value, mWorkingBuffer); }
		virtual void write(Real value) { PacketWriter::write(value, mWorkingBuffer); }
		virtual void write(double value) { PacketWriter::write(value, mWorkingBuffer); }

	private:
		char mWorkingBuffer[TCP_PACKET_WRITER_BUFFER_SIZE];
	};
}

#endif // _TCP_PACKET_WRITER_H_

