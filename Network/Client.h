/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <cassert>
#include <string>
#include <queue>
#include "ApplicationAddress.h"
#include "ClientState.h"
#include "ClientTimeManager.h"
#include "IPacketLossListener.h"
#include "ISerializable.h"
#include "Patterns/Singleton.h"
#include "MagicNetworkConstants.h"
#include "Network.h"
#include "TCPPacket.h"
#include "UDPPacket.h"

namespace Network
{
	class Client : public Patterns::Singleton<Client>
	{
	public:
		Client();
		virtual ~Client();

		void addTCPMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables);
		bool addUDPMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
			UDPPacket::Delivery delivery = UDPPacket::UNRELIABLE, IPacketLossListener *listener = NULL);
		void connect(const ApplicationAddress &serverAddress, const std::string &password,
			Real connectTimeoutSeconds = DEFAULT_CONNECTION_REQUEST_TIMOUT_SECONDS);
		void disconnect();

		MessageType getCurrentTCPMessageIdentifier() const;
		MessageType getCurrentUDPMessageIdentifier(uint16 &senderIdentifier) const;

		ClientState::State getState() const { return mState->getState(); }

		void getTCPMessage(Patterns::ISerializable *serializables, uint32 numOfSerializables);
		void getUDPMessage(Patterns::ISerializable *serializables, uint32 numOfSerializables);
		Real getUDPMessageNetworkTime() const;

		void kick(uint16 identifier);
		
		void send();
		void update();

	private:
		void switchToConnectedState();
		void switchToDisconnectedState();
		void switchToDisconnectingState();

		ClientState *mState;

		//forbidden
		Client(const Client &copy) { assert(false); }
		//forbidden
		Client &operator =(const Client &rhs) { assert(false); return *this; }
	};
}

#endif // _CLIENT_H_

