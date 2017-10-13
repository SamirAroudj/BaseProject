/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _LAN_SERVER_FINDER_H_
#define _LAN_SERVER_FINDER_H_

#include <vector>
#include "ApplicationAddress.h"
#include "Platform/DataTypes.h"
#include "LANInfo.h"

namespace Network
{
	class LANServerFinder
	{
	public:
		LANServerFinder(uint16 LANServerAdvertiserUDPPort);
		~LANServerFinder();

		void clearDiscoveredServers() { mDiscoveredServers.clear(); }

		const std::vector<ApplicationAddress> &getDiscoveredServers() const { return mDiscoveredServers; }
		
		void sendDiscoveryMessage();
		void update();

	private:
		void addLANServer(const ApplicationAddress &serverApp);

		std::vector<ApplicationAddress> mDiscoveredServers;
		std::vector<LANInfo> mLANs;
		int32 mSocketHandle;
		const uint16 mLANServerAdvertiserUDPPort;
		bool mReadyToSend;
	};
}

#endif // _LAN_SERVER_FINDER_H_

