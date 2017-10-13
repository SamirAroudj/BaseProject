/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _EVENT_LISTENER_H_
#define _EVENT_LISTENER_H_

#include "Platform/DataTypes.h"

namespace Network
{
	class EventListener
	{
	public:
		virtual ~EventListener() { }
		virtual int32 getSocketHandle() const = 0;
		virtual bool notifyReceivingIsPossible() = 0;
		virtual void notifySendingIsPossible() = 0;

		bool operator ==(const EventListener &rhs) const
		{
			return rhs.getSocketHandle() == this->getSocketHandle();
		}

	};
}

#endif // _EVENT_LISTENER_H_

