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
#include <sys/select.h>
#endif // _WINDOWS
#include "ClientStateConnected.h"
#include "TCPPacketReader.h"
#include "TCPPacketWriter.h"

using namespace Network;
using namespace ExceptionHandling;

ClientStateConnected::ClientStateConnected(ClientStateConnecting *previousClientState):
	mPassword(previousClientState->mPassword),
	mServerAddress(previousClientState->mServerAddress),
	mTCPClient(previousClientState->mTCPClient),
	mUDPClient(NULL),
	mRejectionReason(::ConnectionCreationException::NUM_OF)
{
	previousClientState->mTCPClient = NULL; // the new state continues to use the TCP client
	delete previousClientState;

	TCPPacketWriter writer;	// send session request message to become fully accepted
	uint32 IPAddress = mTCPClient->getIPAddress();
	mUDPClient = new UDPClientSocket(IPAddress);

	writer.beginPacketAssembly();
	writer.addSessionRequestMessage(IPAddress, mUDPClient->getPortNumber(), mPassword, mUDPClient->getMulticastSupport());
	const TCPPacket *packet = writer.endPacketAssembly(mTCPClient->getIdentifier());
	mTCPClient->addPacket(packet, true);
		
	mTCPClient->notifySendingIsPossible();
	try
	{
		mTCPClient->send();
	}
	catch(NetworkException &exception)	// broken connection, cannot negotiate network session acceptance anymore
	{
		delete mUDPClient;
		delete mTCPClient;

		throw exception;
	}
}


ClientStateConnected::~ClientStateConnected()
{
	if (mTCPClient)
		delete mTCPClient;
	if (mUDPClient)
		delete mUDPClient;
}

ClientState::State ClientStateConnected::update()
{
	timeval immediately = { 0, 0 }; // don't wait until something is readable or writable, just check the state

	fd_set readableSockets, writableSockets;	// check for free sending buffer space / received data
	FD_ZERO(&readableSockets);
	FD_ZERO(&writableSockets);
	FD_SET(mTCPClient->getSocketHandle(), &readableSockets);
	FD_SET(mTCPClient->getSocketHandle(), &writableSockets);

	if (0 == select(mTCPClient->getSocketHandle() + 1, &readableSockets, &writableSockets, NULL, &immediately))
		return ClientState::NO_STATE_CHANGE;
	
	if (FD_ISSET(mTCPClient->getSocketHandle(), &writableSockets))
		mTCPClient->notifySendingIsPossible();
	if (FD_ISSET(mTCPClient->getSocketHandle(), &readableSockets))
		mTCPClient->notifyReceivingIsPossible();

	if (mTCPClient->isPacketAvailable())	// network relevant data for session management received?
		return processReceivedPacket();

	return ClientState::NO_STATE_CHANGE;
}

ClientState::State ClientStateConnected::processReceivedPacket()
{
	TCPPacketReader reader;
	TCPPacket *packet = mTCPClient->getPacket();
	reader.beginReading(packet);

	switch(reader.getCurrentMessageType())
	{
	case TCPPacket::SESSION_POSITIVE_RESPONSE:
		reader.readSessionResponseMessageAndCreateSession();
		reader.endReadingAndDestroyPacket();
		mTCPClient->setIdentifier(Session::getSingleton().getOwnIdentifier());
		return ClientState::READY_TO_USE;

	case TCPPacket::SESSION_IS_FULL:
		reader.readSessionIsFullMessage();
		mTCPClient->stopSending();
		mRejectionReason = ::ConnectionCreationException::SESSION_IS_FULL;
		reader.endReadingAndDestroyPacket();
		return ClientState::DISCONNECTING;

	case TCPPacket::SESSION_WRONG_PASSWORD:
		reader.readSessionWrongPasswordMessage();
		mTCPClient->stopSending();
		mRejectionReason = ::ConnectionCreationException::WRONG_PASSWORD;
		reader.endReadingAndDestroyPacket();
		return ClientState::DISCONNECTING;

	default:
		assert(false);
	}

	return ClientState::DISCONNECTING;
}
