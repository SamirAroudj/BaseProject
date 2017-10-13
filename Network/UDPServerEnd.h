/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _UDP_SERVER_END_H_
#define _UDP_SERVER_END_H_

#ifdef _WINDOWS
#include <WinSock2.h>
#endif // _WINDOWS

#include "Platform/ApplicationTimer.h"
#include "MagicNetworkConstants.h"
#include "ServerTimeManager.h"
#include "UDPSender.h"
#include "UDPSocket.h"

namespace Network
{
	class UDPServerEnd : public UDPSender
	{
	public:
		UDPServerEnd(uint32 IPAddress, uint16 portNumber, uint16 identifier);
		virtual ~UDPServerEnd();

		bool addLostData(const StoredReliableData &data);

		uint16 getIdentifier() const { return mIdentifier; }
		virtual Real getTimeoutPeriod() const { return UDP_PACKET_RESEND_FACTOR * ServerTimeManager::getSingleton().getBadRoundTripTime(mIdentifier); }

		void processAckRequests(std::vector<SenderIdentifierAckPair> &ackRequests);
		void processAckResponses(std::vector<SenderIdentifierAckPair> &ackResponses);

		void startNewSendPeriod() { mResidualPeriodBytes = MAX_BYTES_PER_SEND_PERIOD_TO_CLIENT - MIN_DATAGRAM_OVERHEAD; }
		bool tryToSendData(int32 socketHandle, sockaddr_in &receiver);

	private:
		void updatePacketLossInfoAndStoredReliableData(uint16 ackResponseNumber);
		const uint32 mIPAddressNetworkByteOrder;
		const uint16 mPortNumberNetworkByteOrder;
		const uint16 mIdentifier;

		// forbidden
		UDPServerEnd(const UDPServerEnd &copy) : UDPSender(0), mIPAddressNetworkByteOrder(0), mPortNumberNetworkByteOrder(0), mIdentifier(0)
		{ 
			assert(false); 
		}
		// forbidden
		UDPServerEnd &operator=(const UDPServerEnd &rhs) { assert(false); return *this; }
	};
}

#endif // _UDP_SERVER_END_H_

