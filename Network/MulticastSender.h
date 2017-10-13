/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _MULTICAST_SENDER_H_
#define _MULTICAST_SENDER_H_

#include "UDPServerEnd.h"
#include "UDPSocket.h"

namespace Network
{
	class PacketListenersSendersPair
	{
	public:
		PacketListenersSendersPair(uint16 ackNumber, Real timeoutPeriod, std::vector<UDPServerEnd *> senders);

		PacketListeners mPacketListeners;
		std::vector<UDPServerEnd *> mMissingResponses;
	};

	class StoredReliableDataSendersPair
	{
	public:
		StoredReliableDataSendersPair(const char *data, uint32 dataSize, Real timeoutPeriod, uint16 ackNumber, std::vector<UDPServerEnd *> senders);

		StoredReliableData mStoredReliableData;
		std::vector<UDPServerEnd *> mMissingResponses;
	};

	class MulticastSender
	{
	public:
		MulticastSender();
		~MulticastSender();

		void addMulticastSender(UDPServerEnd *sender) { mSenders.push_back(sender); }
		bool addMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
			UDPPacket::Delivery delivery, IPacketLossListener *listener);
		void computePacketLossNotifications();
		void processAckResponses(const std::vector<SenderIdentifierAckPair> &responses);
		void removeSender(UDPServerEnd *sender);
		void setNetworkTime(Real networkTime)  { mWriter.setNetworkTime(networkTime); }
		void startNewSendPeriod() { mResidualPeriodBytes = MAX_BYTES_PER_SEND_PERIOD_TO_CLIENT - MIN_DATAGRAM_OVERHEAD; }
		bool tryToSendData(int32 socketHandle);

	protected:
		void processAckResponsesForLossListeners(const SenderIdentifierAckPair &response);
		void processAckResponsesForReliableData(const SenderIdentifierAckPair &response);
		void processDataToBeResent();
		void removeSenderFromPacketsListeners(UDPServerEnd *sender);
		void removeSenderFromReliablePackets(UDPServerEnd *sender);
		
		std::vector<PacketListenersSendersPair *> mPacketsListeners;
		std::vector<StoredReliableDataSendersPair *> mReliablePackets;
		std::vector<UDPServerEnd *> mSenders;
		UDPPacketWriter mWriter;
		uint16 mNextAckNumber;
		uint16 mResidualPeriodBytes;
	};
}

#endif // _MULTICAST_SENDER_H_

