/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _LAN_INFO_H_
#define _LAN_INFO_H_

#include "Platform/DataTypes.h"
#include <iostream>

namespace Network
{
	class LANInfo
	{
	public:
		LANInfo(uint32 interfaceIPAddress, uint32 netMask) : mInterfaceIPAddress(interfaceIPAddress), mNetMask(netMask) { }

		uint32 getInterfaceIPAddress() const { return mInterfaceIPAddress; }
		uint32 getNetMask() const { return mNetMask; }
		uint32 getBroadcastAddress() const { return mInterfaceIPAddress | (~mNetMask); }

	private:
		uint32 mInterfaceIPAddress;
		uint32 mNetMask;
	};

	std::ostream &operator <<(std::ostream &os, const LANInfo &info);
}

#endif // _LAN_INFO_H_

