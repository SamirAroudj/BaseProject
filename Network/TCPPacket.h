/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _TCP_PACKET_H_
#define _TCP_PACKET_H_

#include "Packet.h"

namespace Network
{
	class TCPEnd;
	class TCPPacketWriter;

	class TCPPacket : public Packet
	{
	friend class TCPEnd;
	friend class TCPPacketWriter;

	public:
		enum MessageType
		{
			NO_MESSAGE,
			SESSION_REQUEST,
			SESSION_POSITIVE_RESPONSE,
			SESSION_IS_FULL,
			SESSION_WRONG_PASSWORD,
			SESSION_NEW_MEMBER,
			SESSION_REMOVE_MEMBER,
			NUM_OF
		};

		TCPPacket(const TCPPacket *&packet1, const TCPPacket *&packet2);
		virtual ~TCPPacket() { }

	protected:
		TCPPacket(uint16 dataLength, uint16 identifier) : Packet(dataLength, identifier) { }
		// data is only read, not freed
		TCPPacket(uint16 dataLength, const char *data, uint16 identifier) : Packet(dataLength, data, identifier) { }
		/// destroys the referenced packet
		TCPPacket(TCPPacket *&packet);
		TCPPacket(const TCPPacket &copy) : Packet(copy) { }
	};
}

#endif // _TCP_PACKET_H_
