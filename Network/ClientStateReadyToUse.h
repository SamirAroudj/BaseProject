/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _CLIENT_STATE_READY_TO_USE_H_
#define _CLIENT_STATE_READY_TO_USE_H_

#ifdef _WINDOWS
#include <WinSock2.h>
#else 
#include <sys/select.h>
#endif // _WINDOWS

#include "ClientStateConnected.h"
#include "ISerializable.h"
#include "Network.h"
#include "MessageProvider.h"

namespace Network
{
	class ClientStateDisconnecting;
	class ClientStateReadyToUse : public ClientState
	{
	friend class ClientStateDisconnecting;
	public:
		// the previous state is freed by this constructor call
		ClientStateReadyToUse(ClientStateConnected *previousClientState);
		virtual ~ClientStateReadyToUse();

		void addTCPMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables)
		{
			mTCPClient->addMessage(messageIdentifier, serializables, numOfSerializables);
		}

		bool addUDPMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
			UDPPacket::Delivery delivery, IPacketLossListener *listener)
		{
			assert(numOfSerializables > 0 && serializables);
			return mUDPClient->addMessage(messageIdentifier, serializables, numOfSerializables, delivery, listener);
		}

		MessageType getCurrentTCPMessageIdentifier() const { uint16 dummy; return mMessageProvider.getCurrentTCPMessageIdentifier(dummy); }
		MessageType getCurrentUDPMessageIdentifier(uint16 &senderIdentifier) const { return mMessageProvider.getCurrentUDPMessageIdentifier(senderIdentifier); }
		void getTCPMessage(Patterns::ISerializable *serializables, uint32 numOfSerializables)
		{
			mMessageProvider.getTCPMessage(serializables, numOfSerializables);
		}
		void getUDPMessage(Patterns::ISerializable *serializables, uint32 numOfSerializables)
		{
			mMessageProvider.getUDPMessage(serializables, numOfSerializables);
		}
		Real getUDPMessageNetworkTime() const { return mMessageProvider.getUDPMessageNetworkTime(); }

		virtual ClientState::State getState() const { return ClientState::READY_TO_USE; }
		void kick(uint16 identifier);
		void send();
		virtual ClientState::State update();

	private:
		void processReceivedTCPPacket();

		MessageProvider mMessageProvider;
		fd_set mSockets;
		TCPClientEnd *mTCPClient;
		UDPClientSocket *mUDPClient;
		int32 mMaxSocketHandle;
		bool mDisconnecting;

		/// forbidden
		ClientStateReadyToUse(const ClientStateReadyToUse &copy) { assert(false); }
		/// forbidden
		ClientStateReadyToUse &operator =(const ClientStateReadyToUse &rhs) { assert(false); return *this; }
	};
}

#endif // _CLIENT_STATE_READY_TO_USE_H_
