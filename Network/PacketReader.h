/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _PACKET_READER_H_
#define _PACKET_READER_H_

#include <cassert>
#include <string>
#include "Packet.h"
#include "IDeserializer.h"
#include "Network.h"

namespace Network
{
	class PacketReader : public Patterns::IDeserializer
	{
	public:
		PacketReader();
		virtual ~PacketReader() = 0;

		void chopOffReadMessages();
		virtual Packet *endReading();
		void endReadingAndDestroyPacket();
		MessageType getCurrentMessageType() const { return mCurrentMessageType; }
		uint16 getSenderIdentifier() const { return (mPacket) ? mPacket->getSenderIdentifier() : 0; }

		virtual bool readBool();
		virtual char readChar();
		virtual void readString(std::string &text);
		virtual int16 readInt16();
		virtual uint16 readUInt16();
		virtual int32 readInt32();
		virtual uint32 readUInt32();
		virtual Real readFloat();
		virtual double readDouble();

		void readNextMessageType();

	protected:
		void beginReading(Packet *packet);

		Packet *mPacket;
		uint32 mCurrentPosition;
		MessageType mCurrentMessageType;

		//forbidden
		PacketReader(const PacketReader &copy) { assert(false); }
		// forbidden
		PacketReader &operator =(const PacketReader &rhs) { assert(false); return *this; }
	};
}

#endif // _PACKET_READER_H_

