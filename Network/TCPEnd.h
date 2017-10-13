/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _TCP_END_H_
#define _TCP_END_H_

#include <cassert>
#include <queue>
#include "EventListener.h"
#include "ISerializable.h"
#include "Network.h"
#include "TCPPacket.h"

namespace Network
{
	class TCPEnd : public EventListener
	{
	public:
		TCPEnd(int32 socketHandle);
		virtual ~TCPEnd() = 0;
		
		uint16 getIdentifier() const { return mIdentifier; }
		uint32 getIPAddress() const;
		TCPPacket *getPacket();
		virtual int32 getSocketHandle() const { return mSocketHandle; }
		bool isPacketAvailable() const { return !mReceivedPackets.empty(); }
		bool isPending() const { return 0 == mIdentifier; }
		virtual void notifySendingIsPossible();
		virtual bool notifyReceivingIsPossible();
		void setIdentifier(uint16 identifier) { mIdentifier = identifier; }
		void stopSending();

	protected:
		static TCPPacket *createTCPPacket(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
			uint16 senderIdentifier);
		bool isThereNoDataOrError(int32 receivedBytes) const;
		// returns false if the other side closed the connection
		bool receive();
		bool sendOnePacket(const TCPPacket &packet, uint32 &sentBytes);

		std::queue<TCPPacket *> mReceivedPackets;
		TCPPacket *mPartialPacket;
		int32 mSocketHandle;
		uint32 mReceivedPacketBytes;
		uint32 mSentPacketBytes;
		uint16 mIdentifier;
		bool mReadyToSend;
		bool mStoppedSending;

		// forbidden
		TCPEnd(const TCPEnd &copy) { assert(false); }
		// forbidden
		TCPEnd &operator=(const TCPEnd &rhs) { assert(false); return *this; } 
	};
}

#endif // _TCP_END_H_

