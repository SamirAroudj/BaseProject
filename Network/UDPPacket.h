/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _UDP_PACKET_H_
#define _UDP_PACKET_H_

#include "Platform/ApplicationTimer.h"
#include "Packet.h"

namespace Network
{
	class LANServerAdvertiser;
	class LANServerFinder;
	class UDPSocket;

	class UDPPacket : public Packet
	{
	friend class LANServerAdvertiser;
	friend class LANServerFinder;
	friend class UDPSocket;
	public:
		enum MessageType
		{
			NO_MESSAGE,
			LAN_SERVER_DISCOVERY,
			LAN_SERVER_RESPONSE,
			TIME_INITIAL_REQUEST,
			TIME_UPDATE_REQUEST,
			TIME_RESPONSE,
			ACK_REQUEST,
			ACK_RESPONSE,
			NUM_OF
		};

		enum Delivery
		{
			UNRELIABLE,
			RELIABLE,
			LOSS_INFO
		};

		static const uint32 HEADER_SIZE = sizeof(Real);

		virtual ~UDPPacket() { }
		
		virtual const char *getData() const { return Packet::getData() + HEADER_SIZE; }
		virtual const uint16 getDataLength() const { return mDataLength - HEADER_SIZE; }
		Real getNetworkTime() const { return mNetworkTime; }

	private:
		UDPPacket(uint16 dataLength, const char *data, uint16 identifier);

		Real mNetworkTime;
	};
}

#endif // _UDP_PACKET_H_

