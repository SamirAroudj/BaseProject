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
#endif // _WINDOWS
#include "MagicNetworkConstants.h"
#include "MulticastSender.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"
#include "ServerTimeManager.h"

using namespace ExceptionHandling;
using namespace Network;
using namespace std;

PacketListenersSendersPair::PacketListenersSendersPair(uint16 ackNumber, Real timeoutPeriod, vector<UDPServerEnd *> senders) :
	mPacketListeners(ackNumber, timeoutPeriod), mMissingResponses(senders)
{
}

StoredReliableDataSendersPair::StoredReliableDataSendersPair(const char *data, uint32 dataSize, Real timeoutPeriod, uint16 ackNumber,
															 vector<UDPServerEnd *> senders) :
	mStoredReliableData(data, dataSize, timeoutPeriod, ackNumber), mMissingResponses(senders)
{
}

MulticastSender::MulticastSender() : mNextAckNumber(0), mResidualPeriodBytes(MAX_BYTES_PER_SEND_PERIOD_TO_CLIENT - MIN_DATAGRAM_OVERHEAD)
{
}

MulticastSender::~MulticastSender()
{
	uint32 numOfPackets = mPacketsListeners.size();
	for(uint32 i = 0; i < numOfPackets; ++i)
		delete mPacketsListeners[i];

	numOfPackets = mReliablePackets.size();
	for(uint32 i = 0; i < numOfPackets; ++i)
		delete mReliablePackets[i];
}

bool MulticastSender::addMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
	UDPPacket::Delivery delivery, IPacketLossListener *listener)
{
	assert(messageIdentifier >= UDPPacket::NUM_OF);
	if (mSenders.empty())
		return true;

	uint32 requiredMemorySize;
	if (!mWriter.isEnoughMemoryFree(requiredMemorySize, serializables, numOfSerializables, delivery))
		return false;
	if (requiredMemorySize > mResidualPeriodBytes)
		return false;

	if ((delivery == UDPPacket::LOSS_INFO || delivery == UDPPacket::RELIABLE) && !mWriter.containsAckRequestMessage())
			mWriter.addAckRequestMessage(mNextAckNumber++);
	uint32 previousPacketSize = mWriter.getPacketSize();
	mWriter.write(messageIdentifier);
	for(uint32 i = 0; i < numOfSerializables; ++i)
		serializables[i].serialize(mWriter);
	uint32 packetSize = mWriter.getPacketSize();
	mWriter.setUserDataAdded();
	mResidualPeriodBytes -= requiredMemorySize;

	switch(delivery)
	{
	case UDPPacket::UNRELIABLE:
		break;

	case UDPPacket::RELIABLE:
		if (mReliablePackets.empty() || mReliablePackets.back()->mStoredReliableData.getAckNumber() != mWriter.getAckRequestNumber()) // new packet?
			mReliablePackets.push_back(new StoredReliableDataSendersPair(mWriter.getPacketData() + previousPacketSize, packetSize - previousPacketSize, 
				ServerTimeManager::getSingleton().getMeanBadRoundTripTime() * UDP_PACKET_RESEND_FACTOR, mWriter.getAckRequestNumber(), mSenders));
		else
			mReliablePackets.back()->mStoredReliableData.addData(mWriter.getPacketData() + previousPacketSize, packetSize - previousPacketSize);
		break;

	case UDPPacket::LOSS_INFO:
		assert(listener);
		if (mPacketsListeners.empty() || mPacketsListeners.back()->mPacketListeners.mPacketAckNumber != mWriter.getAckRequestNumber())	// new packet?
			mPacketsListeners.push_back(new PacketListenersSendersPair(mWriter.getAckRequestNumber(),
				ServerTimeManager::getSingleton().getMeanBadRoundTripTime() * UDP_PACKET_RESEND_FACTOR, mSenders));
		mPacketsListeners.back()->mPacketListeners.mListeners.push_back(listener);
		break;

	default:
		assert(false);
	}
		
	return true;
}

void MulticastSender::computePacketLossNotifications()
{
	uint32 numOfPackets = mPacketsListeners.size();
	for(uint32 j = 0; j < numOfPackets; ++j)
	{
		assert(!mPacketsListeners[j]->mMissingResponses.empty());
		PacketListeners &candidate = mPacketsListeners[j]->mPacketListeners;
		if (!candidate.mPeriod.hasExpired())	// no response for this packet in time?
			continue;

		uint32 numOfMethods = candidate.mListeners.size();	// notify all listeners of this packet
		for(uint32 i = 0; i < numOfMethods; ++i)
			candidate.mListeners[i]->notifyAboutPacketLoss();
		delete mPacketsListeners[j];
		mPacketsListeners[j] = mPacketsListeners.back();
		mPacketsListeners.pop_back();
		--numOfPackets;
		--j;
	}
}

void MulticastSender::processAckResponses(const vector<SenderIdentifierAckPair> &responses)
{
	uint32 numOfResponses = responses.size();
	for(uint32 i = 0; i < numOfResponses; ++i)
	{
		processAckResponsesForLossListeners(responses[i]);
		processAckResponsesForReliableData(responses[i]);
	}
}

void MulticastSender::processAckResponsesForLossListeners(const SenderIdentifierAckPair &response)
{
	uint32 numOfPackets = mPacketsListeners.size();
	for(uint32 j = 0; j < numOfPackets; ++j)	// find the corresponding packet
	{
		PacketListenersSendersPair *packet = mPacketsListeners[j];
		if (packet->mPacketListeners.mPacketAckNumber != response.mAckNumber)
			continue;

		uint32 numOfSenders = packet->mMissingResponses.size();
		for(uint32 k = 0; k < numOfSenders; ++k)	// find the corresponding sender
		{
			if (packet->mMissingResponses[k]->getIdentifier() != response.mSenderIdentifier)
				continue;

			packet->mMissingResponses[k] = packet->mMissingResponses.back();
			packet->mMissingResponses.pop_back();
			if (packet->mMissingResponses.empty())	// every receiver got it
			{
				delete packet;
				mPacketsListeners[j] = mPacketsListeners.back();
				mPacketsListeners.pop_back();
				--numOfPackets;
				--j;	// do not increase j since the last packet is now at index j
			}
			break;
		}
		break;
	}
}

void MulticastSender::processAckResponsesForReliableData(const SenderIdentifierAckPair &response)
{
	uint32 numOfPackets = mReliablePackets.size();
	for(uint32 j = 0; j < numOfPackets; ++j)	// find the corresponding packet
	{
		StoredReliableDataSendersPair *packet = mReliablePackets[j];
		if (packet->mStoredReliableData.getAckNumber() != response.mAckNumber)
			continue;

		uint32 numOfSenders = packet->mMissingResponses.size();
		for(uint32 k = 0; k < numOfSenders; ++k)	// find the corresponding sender
		{
			if (packet->mMissingResponses[k]->getIdentifier() != response.mSenderIdentifier)
				continue;

			packet->mMissingResponses[k] = packet->mMissingResponses.back();
			packet->mMissingResponses.pop_back();
			if (packet->mMissingResponses.empty())	// every receiver got it
			{
				delete packet;
				mReliablePackets[j] = mReliablePackets.back();
				mReliablePackets.pop_back();
				--numOfPackets;
				--j;	// do not increase j since the last packet is now at index j
			}
			break;
		}
		break;
	}
}

void MulticastSender::removeSender(UDPServerEnd *sender)
{
	uint32 numOfSenders = mSenders.size();	// remove the sender
	for(uint32 i = 0; i < numOfSenders; ++i)
	{
		if (mSenders[i] == sender)
		{
			mSenders[i] = mSenders.back();
			mSenders.pop_back();
			break;
		}
	}

	removeSenderFromPacketsListeners(sender);
	removeSenderFromReliablePackets(sender);
}

void MulticastSender::removeSenderFromPacketsListeners(UDPServerEnd *sender)
{
	uint32 numOfPackets = mPacketsListeners.size();	// remove sender from loss notification requiring data which had not been completely acknowledged so far
	for(uint32 i = 0; i < numOfPackets; ++i)
	{
		PacketListenersSendersPair *candidate = mPacketsListeners[i];
		uint32 numOfSenders = candidate->mMissingResponses.size();
		for(uint32 j = 0; j < numOfSenders; ++j)
		{
			if (candidate->mMissingResponses[j] == sender)
			{
				candidate->mMissingResponses[j] = candidate->mMissingResponses.back();
				candidate->mMissingResponses.pop_back();
				break;
			}
		}
		if (candidate->mMissingResponses.empty())
		{
			delete candidate;
			mPacketsListeners[i] = mPacketsListeners.back();
			mPacketsListeners.pop_back();
			--numOfPackets;
			--i;
		}
	}	
}

void MulticastSender::removeSenderFromReliablePackets(UDPServerEnd *sender)
{
	uint32 numOfPackets = mReliablePackets.size();	// remove the sender from reliable data which had not been completely acknowledged so far
	for(uint32 i = 0; i < numOfPackets; ++i)
	{
		StoredReliableDataSendersPair *candidate = mReliablePackets[i];
		uint32 numOfSenders = candidate->mMissingResponses.size();
		for(uint32 j = 0; j < numOfSenders; ++j)
		{
			if (candidate->mMissingResponses[j] == sender)
			{
				candidate->mMissingResponses[j] = candidate->mMissingResponses.back();
				candidate->mMissingResponses.pop_back();
				break;
			}
		}
		if (candidate->mMissingResponses.empty())
		{
			delete candidate;
			mReliablePackets[i] = mReliablePackets.back();
			mReliablePackets.pop_back();
			--numOfPackets;
			--i;
		}
	}
}


bool MulticastSender::tryToSendData(int32 socketHandle)
{
	if (mSenders.empty())
		return true;
	processDataToBeResent();
	if (UDPPacket::HEADER_SIZE == mWriter.getPacketSize())
		return true;

	sockaddr_in receiver;
	receiver.sin_family = AF_INET;
	receiver.sin_addr.s_addr = htonl(MULTICAST_ADDRESS);
	receiver.sin_port = htons(MULTICAST_PORT_NUMBER);
	memset(&receiver.sin_zero, 0, 8);

	if (SOCKET_ERROR == sendto(socketHandle, mWriter.getPacketData(), mWriter.getPacketSize(), 0,
		reinterpret_cast<sockaddr *>(&receiver), sizeof(sockaddr)))
	{
		int32 errorCode = getError();
		if (WOULD_BLOCK == errorCode)
			return false;

		NetworkExceptionFactory::throwNetworkException("Unable to send datagrams to clients.", errorCode);
	}

	mWriter.endPacketAssembly();
	mResidualPeriodBytes = (mResidualPeriodBytes > MIN_DATAGRAM_OVERHEAD) ? mResidualPeriodBytes - MIN_DATAGRAM_OVERHEAD : 0u;
	return true;
}

void MulticastSender::processDataToBeResent()
{
	uint32 numOfPackets = mReliablePackets.size();
	for(uint32 i = 0; i < numOfPackets; ++i)
	{
		StoredReliableDataSendersPair *candidate = mReliablePackets[i];
		if (!candidate->mStoredReliableData.hasAckExpired())
			continue;

		vector<UDPServerEnd *> failedAdds;	// some of the clients didn't receive this data so resend it to them
		uint32 numOfMissingResponses = candidate->mMissingResponses.size();
		for(uint32 j = 0; j < numOfMissingResponses; ++j)
			if (!candidate->mMissingResponses[j]->addLostData(candidate->mStoredReliableData))
				failedAdds.push_back(candidate->mMissingResponses[j]);
		if (failedAdds.empty())
		{
			delete candidate;
			mReliablePackets[i] = mReliablePackets.back();
			mReliablePackets.pop_back();
			--numOfPackets;
			--i;
			continue;
		}
		candidate->mMissingResponses = failedAdds;	// may be, the response is received next time; otherwise try to add it to an ServerEnd again next frame
	}
}
