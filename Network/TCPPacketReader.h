/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _TCP_PACKET_READER_H_
#define _TCP_PACKET_READER_H_

#include "TCPPacket.h"
#include "PacketReader.h"
#include "Session.h"

namespace Network
{
	class TCPPacketReader : public PacketReader
	{
	public:
		TCPPacketReader();
		virtual ~TCPPacketReader();
		
		virtual void beginReading(TCPPacket *&packet);
		virtual TCPPacket *endReading()
		{
			return (TCPPacket *) PacketReader::endReading();
		}
		
		void readSessionIsFullMessage();
		void readSessionNewMemberMessage(uint32 &IPAddress, uint16 &UDPPortNumber, uint16 &identifier, bool &multicastSupport);
		void readSessionRemoveMemberMessage(uint16 &identifier);
		void readSessionRequestMessage(uint32 &IPAddress, uint16 &remoteUDPPortNumber, std::string &password, bool &multicastSupport);
		void readSessionResponseMessageAndCreateSession();
		void readSessionWrongPasswordMessage();
	};
}

#endif // _TCP_PACKET_READER_H_
