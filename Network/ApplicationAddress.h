/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _APPLICATION_ADDRESS_H_
#define _APPLICATION_ADDRESS_H_

#include <iostream>
#include <string>
#include "Platform/DataTypes.h"
#include "Network.h"

namespace Network
{
	class ApplicationAddress
	{
	public:
		ApplicationAddress() : mIPAddress(0), mPortNumber(0) { }
		ApplicationAddress(uint32 IPAddress, uint16 portNumber) : mIPAddress(IPAddress), mPortNumber(portNumber) { }

		bool operator ==(const ApplicationAddress &rhs) const { return mIPAddress == rhs.mIPAddress && mPortNumber == rhs.mPortNumber; }
		bool operator !=(const ApplicationAddress &rhs) const { return !(*this == rhs); }

		void fillValues(int32 socketHandle);

		std::string getIPAddressAsString() const { return convertAddressHostByteOrderToASCII(mIPAddress); }
		uint32 getIPAddress() const { return mIPAddress; }
		uint16 getPortNumber() const { return mPortNumber; }

		void setIPAddress(uint32 IPAddress) { mIPAddress = IPAddress; }
		void setPortNumber(uint16 portNumber) { mPortNumber = portNumber; }

	private:
		uint32 mIPAddress;
		uint16 mPortNumber;
	};

	std::ostream &operator <<(std::ostream &os, const ApplicationAddress &address);
}
#endif // _APPLICATION_ADDRESS_H_