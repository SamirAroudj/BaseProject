/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _UDP_SENDER_H_
#define _UDP_SENDER_H_

#include <cassert>
#include <vector>
#include "IPacketLossListener.h"
#include "ISerializable.h"
#include "Network.h"
#include "StoredReliableData.h"
#include "UDPPacketWriter.h"

namespace Network
{
	class PacketListeners
	{
	public:
		PacketListeners(uint16 ackNumber, Real timeoutPeriod) : mPeriod(timeoutPeriod), mPacketAckNumber(ackNumber) { }

		Platform::TimePeriod mPeriod;
		std::vector<IPacketLossListener *> mListeners;
		const uint16 mPacketAckNumber;
	};

	class UDPSender
	{
	public:
		UDPSender(uint16 maxBytesPerSendPeriod);
		virtual ~UDPSender();

		bool addMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
			UDPPacket::Delivery delivery, IPacketLossListener *listener);
		void computePacketLossNotifications();
		// returns the time that has to elapse until a packet is considered as being lost
		virtual Real getTimeoutPeriod() const = 0;

		void setNetworkTime(Real networkTime)  { mWriter.setNetworkTime(networkTime); }

	protected:
		void processDataToBeResent();

		std::vector<PacketListeners *> mPacketsListeners;
		std::vector<StoredReliableData *> mReliablePackets;
		UDPPacketWriter mWriter;
		uint16 mNextAckNumber;
		uint16 mResidualPeriodBytes;

	private:
		// forbidden
		UDPSender(const UDPSender &copy) { assert(false); }
		// forbidden
		const UDPSender &operator=(const UDPSender &rhs) { assert(false); return *this; }
	};
}

#endif // _UDP_SENDER_H_