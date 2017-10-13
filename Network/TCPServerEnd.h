/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _TCP_SERVER_END_H_
#define _TCP_SERVER_END_H_

#include <deque>
#include <vector>
#include "ISerializable.h"
#include "TCPEnd.h"

namespace Network
{
	class TCPServerEnd;

	class PendingPacket
	{
	public:
		PendingPacket(const TCPPacket *packet, std::vector<TCPServerEnd *> *senders) : mSenders(senders), mPacket(packet) { }
		~PendingPacket() { delete mPacket; delete mSenders; }
		const TCPPacket &getPacket() const { return *mPacket; }
		
		std::vector<TCPServerEnd *> &getSenders() { return *mSenders;}
		const std::vector<TCPServerEnd *> &getSenders() const { return *mSenders; }

	private:
		std::vector<TCPServerEnd *> *mSenders;
		const TCPPacket *mPacket;

		// forbidden
		PendingPacket(const PendingPacket &rhs) { assert(false); }
		//forbidden
		PendingPacket &operator =(const PendingPacket *rhs) { assert(false); return *this; }
	};

	class TCPServerEnd : public TCPEnd
	{
	public:
		TCPServerEnd(int32 socketHandle);
		virtual ~TCPServerEnd();

		static void destroyAllPendingPackets();
		static void addTo(TCPPacket *packet, TCPServerEnd *sender);
		static void addToSeveralServerEnds(TCPPacket *packet, std::vector<TCPServerEnd *> *&senders);
		static void addToSeveralServerEnds(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
			std::vector<TCPServerEnd *> *senders);
		static void send();

	private:
		static std::deque<PendingPacket *> msPendingPackets;

		//forbidden
		TCPServerEnd(const TCPServerEnd &copy) : TCPEnd(0) { assert(false); }
		TCPServerEnd &operator =(const TCPServerEnd &rhs) { assert(false); return *this; }
	};
}

#endif // _TCP_SERVER_END_H_
