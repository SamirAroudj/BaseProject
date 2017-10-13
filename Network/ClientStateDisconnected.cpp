/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "ClientStateDisconnected.h"
#include "Session.h"

using namespace Network;

ClientStateDisconnected::ClientStateDisconnected()
{
	if (Session::exists())
		delete Session::getSingletonPointer();
}


ClientStateDisconnected::~ClientStateDisconnected()
{
}


ClientState::State ClientStateDisconnected::update()
{
	return ClientState::NO_STATE_CHANGE;
}
