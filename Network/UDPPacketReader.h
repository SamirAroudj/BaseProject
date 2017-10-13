/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _UDP_PACKET_READER_H_
#define _UDP_PACKET_READER_H_

#include "ApplicationAddress.h"
#include "PacketReader.h"
#include "UDPPacket.h"

namespace Network
{
	class UDPPacketReader : public PacketReader
	{
	public:
		UDPPacketReader();
		virtual ~UDPPacketReader();

		virtual void beginReading(UDPPacket *&packet);
		virtual UDPPacket *endReading()
		{
			return (UDPPacket *) PacketReader::endReading();
		}

		// returns the synchronized network time when the packet was sent
		Real getPacketNetworkTime() const
		{
			assert(mPacket);
			return ((UDPPacket *) mPacket)->getNetworkTime();
		}

		uint16 readAckRequestMessage();
		uint16 readAckResponseMessage();
		/// returns false if the format is corrupted
		bool readLANServerDiscoveryMessage();
		/// returns false if the format is corrupted
		bool readLANServerResponseMessage(ApplicationAddress &serverApp);
		void readTimeRequestMessage(Real &clientRequestTime);
		void readTimeResponseMessage(Real &clientRequestTime, Real &serverTime);
	};
}

#endif // _UDP_PACKET_READER_H_
