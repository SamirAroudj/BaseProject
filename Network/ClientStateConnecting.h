/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _CLIENT_STATE_CONNECTING_H_
#define _CLIENT_STATE_CONNECTING_H_

#include "ClientState.h"
#include "TCPClientEnd.h"
#include "Platform/TimePeriod.h"

namespace Network
{
	class ClientStateConnected;

	class ClientStateConnecting : public ClientState
	{
	friend class ClientStateConnected;
	public:
		ClientStateConnecting(const ApplicationAddress &serverAddress, const std::string &password, Real timeoutSeconds);
		virtual ~ClientStateConnecting();

		const ApplicationAddress &getServerAddress() const { return mServerAddress; }
		virtual ClientState::State getState() const { return ClientState::CONNECTING; }
		virtual ClientState::State update();

	private:
		Platform::TimePeriod mTimeout;
		TCPClientEnd *mTCPClient;
		const std::string mPassword;
		const ApplicationAddress mServerAddress;

		/// forbidden
		ClientStateConnecting(const ClientStateConnecting &copy) : mTimeout(0.0f) { assert(false); }
		/// forbidden
		ClientStateConnecting &operator =(const ClientStateConnecting &rhs) { assert(false); return *this; }
	};
}

#endif // _CLIENT_STATE_CONNECTING_H_
