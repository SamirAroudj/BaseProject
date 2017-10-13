/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _UDP_SERVER_SOCKET_H_
#define _UDP_SERVER_SOCKET_H_

#include "MulticastSender.h"
#include "Session.h"
#include "UDPServerEnd.h"
#include "UDPSocket.h"

namespace Network
{
	class UDPServerSocket : public UDPSocket, public ISessionListener
	{
	public:
		UDPServerSocket(uint32 IPAddress);
		virtual ~UDPServerSocket();

		// returns the identifiers of the receivers the packets of which had NOT enough free space to include the specified message
		std::vector<uint16> addMessage(MessageType messageIdentifier,
			const Patterns::ISerializable *serializables, uint32 numOfSerializables,
			UDPPacket::Delivery delivery, IPacketLossListener *listener);
		bool addMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
			uint16 memberIdentifier, UDPPacket::Delivery delivery, IPacketLossListener *listener);

                bool getMulticastSupport() const { return mMulticastSupport; }
                
		virtual void notifyNewMember(const SessionMember &newMember);
		virtual bool notifyReceivingIsPossible();
		virtual void notifyRemovedMember(uint16 identifier);

		void send();
		virtual void update();

	private:
		std::vector<UDPServerEnd *> mSenders;
		MulticastSender mMulticastSender;
		uint32 mLastSender;
                bool mMulticastSupport;
	};
}

#endif // _UDP_SERVER_SOCKET_H_

