/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _PACKET_H_
#define _PACKET_H_

#include <new>
#include "Platform/DataTypes.h"

namespace Network
{
	class Packet
	{
	public:
		Packet(uint16 identifier) : mIdentifier(identifier) { }
		Packet(uint16 dataLength, uint16 identifier);
		// data is only read, not freed
		Packet(uint16 dataLength, const char *data, uint16 identifier);
		Packet(const Packet &copy);

		virtual ~Packet() { delete [] mData; }

		Packet &operator =(const Packet &rhs);

		void chopOffFront(uint32 numOfBytes);

		virtual const char *getData() const { return mData + sizeof(uint16); }
		const char *getDataWithHeader() const { return mData; }
		virtual const uint16 getDataLength() const { return mDataLength; }
		const uint16 getSenderIdentifier() const { return mIdentifier; }
		const uint16 getLengthHeaderInclusive() const { return mDataLength + sizeof(uint16); }

	protected:
		char *getPrivateData() const { return mData + sizeof(uint16); }
		void initialize();

		char *mData;
		const uint16 mIdentifier;
		uint16 mDataLength;
	};
}

#endif // _PACKET_H_

