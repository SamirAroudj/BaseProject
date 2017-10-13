/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _CLIENT_STATE_H_
#define _CLIENT_STATE_H_

#include <cassert>

namespace Network
{
	/// state machine used by the client
	class ClientState
	{
	public:
		enum State {DISCONNECTED, CONNECTING, CONNECTED, READY_TO_USE, DISCONNECTING, NO_STATE_CHANGE, NUM_OF};

		ClientState();
		virtual ~ClientState();
		virtual State getState() const = 0;
		virtual State update() = 0;

	private:

		/// forbidden
		ClientState(const ClientState &copy) { assert(false); }
		/// forbidden
		ClientState &operator =(const ClientState &rhs) { assert(false); return *this; }
	};
}

#endif // _CLIENT_STATE_H_

