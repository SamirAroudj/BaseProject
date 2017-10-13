/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
#include <WinSock2.h>
#elif _LINUX
#include <arpa/inet.h>
#include <cstring>
#endif // _WINDOWS

#include "ClientTimeManager.h"
#include "UDPPacket.h"

using namespace Network;
using namespace Platform;

UDPPacket::UDPPacket(uint16 dataLength, const char *data, uint16 identifier) : Packet(dataLength, data, identifier), mNetworkTime(0)
{
	memcpy(&mNetworkTime, Packet::getData(), sizeof(Real));
	
	if (ClientTimeManager::exists())
	{
		if (mNetworkTime > ClientTimeManager::getSingleton().getNetworkTime()) // might be necessary due to small clock deviations 
			mNetworkTime = ClientTimeManager::getSingleton().getNetworkTime(); // when latency is very short - sending data over LAN or to the same computer
	}
	else
	{
		if (mNetworkTime > ApplicationTimer::getSingleton().getSystemTime())
			mNetworkTime = ApplicationTimer::getSingleton().getSystemTime();
	}
}