/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _UDP_SOCKET_H_
#define _UDP_SOCKET_H_

#ifdef _WINDOWS
#include <WinSock2.h>
#include <Ws2ipdef.h>
#else
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#endif 

#include <cassert>
#include <vector>
#include "EventListener.h"
#include "Platform/TimePeriod.h"
#include "UDPPacket.h"
#include "UDPPacketReader.h"

namespace Network
{
	struct SenderIdentifierAckPair
	{
		uint16 mSenderIdentifier;
		uint16 mAckNumber;
	};

	class UDPSocket : public EventListener
	{
	public:
		UDPSocket(bool usesBroadcasts = false);
		virtual ~UDPSocket();

		void bind(uint32 IPAddress, uint16 portNumber = 0);
		
		UDPPacket *getPacket();
		uint16 getPortNumber() const;
		virtual int32 getSocketHandle() const { return mSocketHandle; }
		bool isPacketAvailable() const { return !mPackets.empty(); }
		bool isReadyToSend() const { return mReadyToSend; }

		virtual bool notifyReceivingIsPossible();
		virtual void notifySendingIsPossible() { mReadyToSend = true; }

		virtual void update() = 0;

	protected:
		void processAckResponse(UDPPacketReader &reader);
		void processReceivedPacket(UDPPacket *packet, sockaddr_in &sender);
		void processTimeRequestMessage(UDPPacketReader &reader, sockaddr_in &sender, bool initialTimeRequest);

		std::vector<SenderIdentifierAckPair> mAckRequests;
		std::vector<SenderIdentifierAckPair> mAckResponses;
		std::vector<UDPPacket *> mPackets;
		Platform::TimePeriod mSendPeriod;
		int32 mSocketHandle;
		bool mReadyToSend;

	private:
		/// forbidden
		UDPSocket(const UDPSocket &copy) : mSendPeriod(0.0f) { assert(false); }
		/// forbidden
		UDPSocket &operator =(const UDPSocket &rhs) { assert(false); return *this;}
	};
}

#endif // _UDP_SOCKET_H_

