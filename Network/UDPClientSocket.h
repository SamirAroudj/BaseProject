/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _UDP_CLIENT_SOCKET_H_
#define _UDP_CLIENT_SOCKET_H_

#ifdef _WINDOWS
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif // _WINDOWS

#include "ClientTimeManager.h"
#include "MagicNetworkConstants.h"
#include "UDPSender.h"
#include "UDPSocket.h"

namespace Network
{
	class UDPClientSocket : public UDPSocket, public UDPSender
	{
	public:
		UDPClientSocket(uint32 ipAddress);
		virtual ~UDPClientSocket();

		bool getMulticastSupport() const { return mMulticastSupport; }
		virtual Real getTimeoutPeriod() const { return UDP_PACKET_RESEND_FACTOR * ClientTimeManager::getSingleton().getBadRoundTripTime(); }
		virtual bool notifyReceivingIsPossible();

		void send();
		void setServerAddress(sockaddr_in serverAddress) { mServerAddress = serverAddress; }
		virtual void update();

	private:
		void processAckResponses();

		sockaddr_in mServerAddress;
		bool mMulticastSupport;

		/// forbidden
		UDPClientSocket(const UDPClientSocket &copy) : UDPSocket(0), UDPSender(0) { assert(false); }
		/// forbidden
		UDPClientSocket &operator =(const UDPClientSocket &rhs) { assert(false);  return *this; }
	};
}

#endif // _UDP_CLIENT_SOCKET_H_

