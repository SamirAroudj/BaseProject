/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _I_SESSION_LISTENER_H_
#define _I_SESSION_LISTENER_H_

#include "SessionMember.h"

namespace Network
{
	class ISessionListener
	{
	public:
		virtual void notifyNewMember(const SessionMember &newMember) = 0;
		virtual void notifyRemovedMember(uint16 identifier) = 0;
	};
}

#endif // _I_SESSION_LISTENER_H_