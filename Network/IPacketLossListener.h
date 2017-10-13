/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _I_PACKET_LOSS_LISTENER_H_
#define _I_PACKET_LOSS_LISTENER_H_

namespace Network
{
	class IPacketLossListener
	{
	public:
		virtual void notifyAboutPacketLoss() = 0;
	};
}

#endif // _I_PACKET_LOSS_LISTENER_H_