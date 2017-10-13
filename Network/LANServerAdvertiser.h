/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _LAN_SERVER_ADVERTISER_H_
#define _LAN_SERVER_ADVERTISER_H_

#include <queue>
#include <vector>
#include "ApplicationAddress.h"
#include "Network.h"
#include "UDPPacketWriter.h"

namespace Network
{
	class LANServerAdvertiser
	{
	public:
		LANServerAdvertiser(const ApplicationAddress &serverApp, uint16 udpPortNumber, uint32 IPAddress = 0);
		~LANServerAdvertiser();
		void sendResponses();
		void update();

	private:
		bool senderIsLANMember(uint32 IPAddress);

		UDPPacketWriter mPacketWriter;
		std::vector<LANInfo> mLANs;
		std::queue<ApplicationAddress> mReceivers;
		int32 mSocketHandle;
		bool mReadyToSend;
	};
}

#endif // _LAN_SERVER_ADVERTISER

