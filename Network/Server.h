/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _SERVER_H_
#define _SERVER_H_

#ifdef _WINDOWS
#include <WinSock2.h>
#else
#include <sys/select.h>
#endif // _WINDOWS

#include <cassert>
#include "Platform/DataTypes.h"
#include "MessageProvider.h"
#include "Network.h"
#include "TCPPacketWriter.h"
#include "TCPServerEnd.h"
#include "UDPServerSocket.h"

namespace Network
{
	class Server
	{
	public:
		Server(const std::string &password = "", uint32 maxNumOfSessionClients = 0, uint32 IPAddress = 0);
		virtual ~Server();

		void addTCPMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables)
		{ 
			addTCPMessageToAllExceptOne(messageIdentifier, serializables, numOfSerializables, 0); 
		}

		void addTCPMessageToAllExceptOne(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
			uint16 excludedServerEndIdentifier);

		// data is send to all clients via explicit multicast messages if possible
		// returns the identifiers of the receivers the packets of which had NOT enough free space to include the specified message
		std::vector<uint16> addUDPMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
			UDPPacket::Delivery delivery = UDPPacket::UNRELIABLE, IPacketLossListener *listener = NULL)
		{
			assert(serializables && numOfSerializables > 0);
			return mUDPServer->addMessage(messageIdentifier, serializables, numOfSerializables, delivery, listener);
		}

		bool addUDPMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
			uint16 memberIdentifier, UDPPacket::Delivery delivery = UDPPacket::UNRELIABLE, IPacketLossListener *listener = NULL)
		{
			assert(serializables && numOfSerializables > 0);
			return mUDPServer->addMessage(messageIdentifier, serializables, numOfSerializables, memberIdentifier, delivery, listener);
		}

		void getAddress(ApplicationAddress &address) { address.fillValues(mListeningSocket); }

		MessageType getCurrentTCPMessageIdentifier(uint16 &senderIdentifier) const { return mMessageProvider.getCurrentTCPMessageIdentifier(senderIdentifier); }
		MessageType getCurrentUDPMessageIdentifier(uint16 &senderIdentifier) const { return mMessageProvider.getCurrentUDPMessageIdentifier(senderIdentifier); }

		/// includes pending connections with members that are not included by the network session
		uint32 getNumberOfConnections() const { return mConnections.size(); }

		void getTCPMessage(Patterns::ISerializable *serializables, uint32 numOfSerializables)
		{
			mMessageProvider.getTCPMessage(serializables, numOfSerializables);
		}
		void getUDPMessage(Patterns::ISerializable *serializables, uint32 numOfSerializables)
		{
			mMessageProvider.getUDPMessage(serializables, numOfSerializables);
		}
		Real getUDPMessageNetworkTime() const { return mMessageProvider.getUDPMessageNetworkTime(); }

		void listen(int32 maxNumOfPendingConnectionRequests);
		
		void send();
		virtual void update();

	private:
		void acceptConnection();
		void computeMaxSocketHandle();
		void computeSessionMemberReceivers(uint16 excludedServerEndIdentifier, std::vector<TCPServerEnd *> *receivers);
		void confirmConnection(TCPServerEnd *serverEnd, TCPPacketWriter &writer, uint32 remoteIPAddress, uint16 remoteUDPPortNumber, bool multicastSupport);
		void deregisterAndDestroyServerEnd(uint32 connectionIndex);
		void deregisterForSending(const EventListener *listener);
		void deregisterForReceiving(const EventListener *listener);
		void processClosedConnection(uint32 connectionIndex);
		void processReceivedPacket(TCPServerEnd *serverEnd);
		// returns false if the connection is rejected / closed
		void processSessionRemoveMemberMessage(TCPPacketReader &reader);
		void processSessionRequestMessage(TCPPacketReader &reader, TCPServerEnd *serverEnd);
		void registerServerEnd(TCPServerEnd *serverEnd);
		void registerForSending(const EventListener *listener);
		void registerForReceiving(const EventListener *listener);
		void updateConnections(const fd_set &readableSockets, const fd_set &writableSockets, int32 &numOfActiveSockets);

		MessageProvider mMessageProvider;
		std::vector<TCPServerEnd *> mConnections;
		UDPServerSocket *mUDPServer;
		fd_set mReadingSockets;
		fd_set mWritingSockets;
		int32 mListeningSocket;
		int32 mMaxSocketHandle;

		//forbidden
		Server(const Server &copy) { assert(false); }
		//forbidden
		Server &operator =(const Server &rhs) { assert(false); return *this; }
	};
}

#endif // _SERVER_H_

