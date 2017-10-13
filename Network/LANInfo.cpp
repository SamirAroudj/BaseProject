/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "LANInfo.h"
#include "Network.h"

using namespace Network;
using namespace std;

ostream &Network::operator <<(ostream &os, const LANInfo &info)
{
	os << "LAN information:\n";
	os << "IP address of the machine connected to the LAN:\n" << 
		convertAddressHostByteOrderToASCII(info.getInterfaceIPAddress()) << "\n";
	os << "Net mask:\n" << convertAddressHostByteOrderToASCII(info.getNetMask()) << "\n";
	os << "Broadcast address:\n" << convertAddressHostByteOrderToASCII(info.getBroadcastAddress()) << endl;

	return os;
}
