/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _CLIENT_STATE_DISCONNECTING_H_
#define _CLIENT_STATE_DISCONNECTING_H_

#include "ClientStateConnected.h"
#include "ClientStateReadyToUse.h"

namespace Network
{
	class ClientStateDisconnecting : public ClientState
	{
	public:
		/// the previous client state is freed by this constructor
		ClientStateDisconnecting(ClientStateConnected *previousClientState);
		ClientStateDisconnecting(ClientStateReadyToUse *previousClientState);
		virtual ~ClientStateDisconnecting();
		virtual ClientState::State getState() const { return ClientState::DISCONNECTING; }
		virtual ClientState::State update();

	private:

		TCPClientEnd *mTCPClient;

		/// forbidden
		ClientStateDisconnecting(const ClientStateDisconnecting &copy) { assert(false); }
		/// forbidden
		ClientStateDisconnecting &operator =(const ClientStateDisconnecting &rhs) { assert(false); return *this; }
	};
}

#endif // _CLIENT_STATE_DISCONNECTING_H_
