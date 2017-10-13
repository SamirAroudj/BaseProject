/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _WINDOWS
#include <sys/socket.h>
#include <sys/types.h>
#include "Network.h"
#endif // _WINDOWS

#include "ClientStateDisconnecting.h"

using namespace Network;

ClientStateDisconnecting::ClientStateDisconnecting(ClientStateConnected *previousClientState) :
	mTCPClient(previousClientState->mTCPClient)
{
	mTCPClient->stopSending();
	previousClientState->mTCPClient = NULL;	// now this state uses the tcp client
	delete previousClientState;
}

ClientStateDisconnecting::ClientStateDisconnecting(ClientStateReadyToUse *previousClientState) :
	mTCPClient(previousClientState->mTCPClient)
{
	mTCPClient->stopSending();
	previousClientState->mTCPClient = NULL;	// now this state uses the tcp client
	delete previousClientState;
}

ClientStateDisconnecting::~ClientStateDisconnecting()
{
	if (mTCPClient)
		delete mTCPClient;
}

ClientState::State ClientStateDisconnecting::update()
{
	timeval immediately = { 0, 0 }; // don't wait until something is readable or writable, just check the state
	fd_set readableSockets;	// only check for the FIN flag which is expected to be received -> read 0 bytes with recv
	FD_ZERO(&readableSockets);
	FD_SET(mTCPClient->getSocketHandle(), &readableSockets);

	if (0 < select(mTCPClient->getSocketHandle() + 1, &readableSockets, NULL, NULL, &immediately)) // socket is readable -> closed connection message or pending network data 
		if (!mTCPClient->notifyReceivingIsPossible())	// also the remote side closed the connection
			return ClientState::DISCONNECTED;

	return ClientState::NO_STATE_CHANGE;
}
