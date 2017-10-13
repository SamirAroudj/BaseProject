/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _PACKET_WRITER_H_
#define _PACKET_WRITER_H_
#include <cassert>
#include <string>
#include "ISerializer.h"
#include "Packet.h"

namespace Network
{
	class PacketWriter : public Patterns::ISerializer
	{
	public:
		PacketWriter(uint32 maxPacketSize);
		virtual ~PacketWriter() = 0;
		uint32 getPacketSize() const { return mPacketSize; }
		uint32 getFreeMemorySize() const { return mMaxPacketSize - mPacketSize; }

	protected:
		void write(bool byte, char *buffer);
		void write(char byte, char *buffer);
		void write(const std::string &text, char *buffer);
		void write(int16 value, char *buffer);
		void write(uint16 value, char *buffer);
		void write(int32 value, char *buffer);
		void write(uint32 value, char *buffer);
		void write(Real value, char *buffer);
		void write(double value, char *buffer);

		uint32 mPacketSize;
		const uint32 mMaxPacketSize;

		// forbidden
		PacketWriter(const PacketWriter &copy) : mMaxPacketSize(0) { assert(false); }
		// forbidden
		PacketWriter &operator =(const PacketWriter &rhs) { assert(false); return *this; }
	};
}

#endif // _PACKET_WRITER_H_

