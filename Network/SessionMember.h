/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _SESSION_MEMBER_H_
#define _SESSION_MEMBER_H_

#include <string>
#include "Platform/DataTypes.h"
#include "MagicNetworkConstants.h"

namespace Network
{
	class SessionMember
	{
	public:
		SessionMember(uint32 IPAddress, uint16 UDPPortNumber, uint16 identifier, bool multicastSupport);
		bool operator <(const SessionMember &rhs) const { return mIdentifier < rhs.mIdentifier; }
		bool operator >(const SessionMember &rhs) const { return mIdentifier > rhs.mIdentifier; }
		
		uint16 getIdentifier() const { return mIdentifier; }
		// returns the IP address in host byte order
		uint32 getIPAddress() const { return mIPAddress; }
		void getIPAddress(std::string &IPAddress) const;
		bool getMulticastSupport() const { return mMulticastSupport; }
		uint16 getUDPPortNumber() const { return mUDPPortNumber; }
		
		void setIdentifier(uint16 identifier) { mIdentifier = identifier; }
		void setIPAddress(uint32 IPAddress) { mIPAddress = IPAddress; }
		void setUDPPortNumber(uint16 UDPPortNumber) { mUDPPortNumber = UDPPortNumber; }

	private:
		uint32 mIPAddress;
		uint16 mIdentifier;
		uint16 mUDPPortNumber;
		const bool mMulticastSupport;
	};
}

#endif // _SESSION_MEMBER_H_
