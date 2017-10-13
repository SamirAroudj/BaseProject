/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif // _WINDOWS

#include "ClientStateReadyToUse.h"
#include "ClientTimeManager.h"
#include "TCPPacketReader.h"
#include "TCPPacketWriter.h"

using namespace Network;
using namespace std;
 
ClientStateReadyToUse::ClientStateReadyToUse(ClientStateConnected *previousClientState) :
	mTCPClient(previousClientState->mTCPClient),
	mUDPClient(previousClientState->mUDPClient),
	mMaxSocketHandle((mTCPClient->getSocketHandle() > mUDPClient->getSocketHandle()) ? mTCPClient->getSocketHandle() : mUDPClient->getSocketHandle()),
	mDisconnecting(false)
{
	previousClientState->mTCPClient = NULL;	// free the previous state
	previousClientState->mUDPClient = NULL;	// this state continues to use the clients, hence they must not be deleted
	delete previousClientState;

	FD_ZERO(&mSockets);
	FD_SET(mTCPClient->getSocketHandle(), &mSockets);
	FD_SET(mUDPClient->getSocketHandle(), &mSockets);

	const SessionMember *server = Session::getSingleton().getMember(0);	// give UDP client the server address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = ntohl(server->getIPAddress());
	serverAddress.sin_port = ntohs(server->getUDPPortNumber());
	memset(serverAddress.sin_zero, 0, sizeof(char) * 8);
	mUDPClient->setServerAddress(serverAddress);
	
	ClientTimeManager *clientTimeManager = new ClientTimeManager();
}

ClientStateReadyToUse::~ClientStateReadyToUse()
{
	FD_ZERO(&mSockets);
	if (mTCPClient)
		delete mTCPClient;
	if (mUDPClient)
		delete mUDPClient;
	if (ClientTimeManager::getSingleton().exists())
		delete ClientTimeManager::getSingletonPointer();
}

void ClientStateReadyToUse::kick(uint16 identifier)
{
#ifdef _DEBUG
	assert(Session::getSingleton().getMember(identifier));
#endif

	TCPPacketWriter writer;
	writer.beginPacketAssembly();
	writer.addSessionRemoveMemberMessage(identifier);
	const TCPPacket *packet = writer.endPacketAssembly(mTCPClient->getIdentifier());
	mTCPClient->addPacket(packet, true);
}

void ClientStateReadyToUse::send()
{
	mTCPClient->send();
	mUDPClient->send();
}

ClientState::State ClientStateReadyToUse::update()
{
	mUDPClient->update();

	timeval immediately = { 0, 0 }; // don't wait until something is readable or writable, just check the state
	fd_set readableSockets = mSockets;
	fd_set writableSockets = mSockets;
	if (0 == select(mMaxSocketHandle + 1, &readableSockets, &writableSockets, NULL, &immediately)) // check for sending / receiving
		return ClientState::NO_STATE_CHANGE;
	
	if (FD_ISSET(mTCPClient->getSocketHandle(), &writableSockets))	// free sending buffer space?
		mTCPClient->notifySendingIsPossible();
	if (FD_ISSET(mUDPClient->getSocketHandle(), &writableSockets))
		mUDPClient->notifySendingIsPossible();

	if (FD_ISSET(mUDPClient->getSocketHandle(), &readableSockets))	// data received?
		mUDPClient->notifyReceivingIsPossible();
	if (FD_ISSET(mTCPClient->getSocketHandle(), &readableSockets))
		if (!mTCPClient->notifyReceivingIsPossible())
			mDisconnecting = true;	// server closed connection

	while(mUDPClient->isPacketAvailable())
	{
		UDPPacket *p = mUDPClient->getPacket();
		mMessageProvider.add(p);
	}
	while(mTCPClient->isPacketAvailable())	// check for network relevant data
		processReceivedTCPPacket();

	if (mDisconnecting)
		return ClientState::DISCONNECTING;

	return ClientState::NO_STATE_CHANGE;
}

void ClientStateReadyToUse::processReceivedTCPPacket()
{
	TCPPacketReader reader;
	TCPPacket *packet = mTCPClient->getPacket();
	uint16 identifier = 0;
	reader.beginReading(packet);

	while(true)
	{
		if (reader.getCurrentMessageType() >= MIN_USER_TCP_MESSAGE_IDENTIFIER)
		{
			reader.chopOffReadMessages();	// chop off the data which is only relevant for the network management
			packet = reader.endReading();
			mMessageProvider.add(packet);
			return;
		}

		switch(reader.getCurrentMessageType())
		{
		case TCPPacket::SESSION_NEW_MEMBER:
		{
			uint32 IPAddress;
			uint16 UDPPortNumber;
			bool multicastSupport;
			reader.readSessionNewMemberMessage(IPAddress, UDPPortNumber, identifier, multicastSupport);
			Session::getSingleton().addMember(IPAddress, UDPPortNumber, identifier, multicastSupport);
			break;
		}
			
		case TCPPacket::SESSION_REMOVE_MEMBER:
			reader.readSessionRemoveMemberMessage(identifier);
			Session::getSingleton().removeMember(identifier);

			if (Session::getSingleton().getOwnIdentifier() == identifier)
				mDisconnecting = true;
			break;

		case TCPPacket::NO_MESSAGE:	// only network relevant data is contained
			reader.endReadingAndDestroyPacket();
			return;
			
		default:	// these protocol message must not be received in this state
			assert(false);
			break;
		}
	}
}