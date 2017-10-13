/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _CLIENT_STATE_DISCONNECTED_H_
#define _CLIENT_STATE_DISCONNECTED_H_

#include "ClientState.h"

namespace Network
{
	class ClientStateDisconnected : public ClientState
	{
	public:
		ClientStateDisconnected();
		virtual ~ClientStateDisconnected();
		virtual ClientState::State getState() const { return ClientState::DISCONNECTED; }
		virtual ClientState::State update();

	private:

		/// forbidden
		ClientStateDisconnected(const ClientStateDisconnected &copy) { assert(false); }
		/// forbidden
		ClientStateDisconnected &operator =(const ClientStateDisconnected &rhs) { assert(false); return *this; }
	};
}

#endif // _CLIENT_STATE_DISCONNECTED_H_
