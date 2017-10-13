/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _CLIENT_STATE_CONNECTED_H_
#define _CLIENT_STATE_CONNECTED_H_

#include "ClientStateConnecting.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"
#include "TCPClientEnd.h"
#include "UDPClientSocket.h"

namespace Network
{
	class ClientStateDisconnecting;
	class ClientStateReadyToUse;

	class ClientStateConnected : public ClientState
	{
	friend class ClientStateDisconnecting;
	friend class ClientStateReadyToUse;
	public:
		/// the previous client state is automatically deleted
		ClientStateConnected(ClientStateConnecting *previousClientState);
		virtual ~ClientStateConnected();
		
		const std::string &getPassword() const { return mPassword; }
		ExceptionHandling::ConnectionCreationException::Type getRejectionReason() const { return mRejectionReason; }
		const ApplicationAddress &getServerAddress() const { return mServerAddress; }
		virtual ClientState::State getState() const { return ClientState::CONNECTED; }

		virtual ClientState::State update();

	private:
		ClientState::State processReceivedPacket();
		
		const std::string mPassword;
		TCPClientEnd *mTCPClient;
		UDPClientSocket *mUDPClient;
		const ApplicationAddress mServerAddress;
		ExceptionHandling::ConnectionCreationException::Type mRejectionReason;

		/// forbidden
		ClientStateConnected(const ClientStateConnected &copy) { assert(false); }
		/// forbidden
		ClientStateConnected &operator =(const ClientStateConnected &rhs) { assert(false); return *this; }
	};
}

#endif // _CLIENT_STATE_CONNECTED_H_
