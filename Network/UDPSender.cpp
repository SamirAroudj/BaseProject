/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "UDPSender.h"

using namespace Network;
using namespace std;

UDPSender::UDPSender(uint16 maxBytesPerSendPeriod) : mNextAckNumber(0), mResidualPeriodBytes(maxBytesPerSendPeriod)
{
}

UDPSender::~UDPSender()
{
	uint32 numOfReliablePackets = mReliablePackets.size();
	for(uint32 i = 0; i < numOfReliablePackets; ++i)
		delete mReliablePackets[i];
}

bool UDPSender::addMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
						   UDPPacket::Delivery delivery, IPacketLossListener *listener)
{
	assert(messageIdentifier >= UDPPacket::NUM_OF);
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
		if (mReliablePackets.empty() || mReliablePackets.back()->getAckNumber() != mWriter.getAckRequestNumber()) // new packet?
			mReliablePackets.push_back(new StoredReliableData(mWriter.getPacketData() + previousPacketSize, packetSize - previousPacketSize, 
				getTimeoutPeriod(), mWriter.getAckRequestNumber()));
		else
			mReliablePackets.back()->addData(mWriter.getPacketData() + previousPacketSize, packetSize - previousPacketSize);
		break;

	case UDPPacket::LOSS_INFO:
		assert(listener);
		if (mPacketsListeners.empty() || mPacketsListeners.back()->mPacketAckNumber != mWriter.getAckRequestNumber())	// new packet?
			mPacketsListeners.push_back(new PacketListeners(mWriter.getAckRequestNumber(), getTimeoutPeriod()));
		mPacketsListeners.back()->mListeners.push_back(listener);
		break;

	default:
		assert(false);
	}
		
	return true;
}

void UDPSender::computePacketLossNotifications()
{
	uint32 numOfPackets = mPacketsListeners.size();
	for(uint32 j = 0; j < numOfPackets; )
	{
		if (!mPacketsListeners[j]->mPeriod.hasExpired())	// no response for this packet in time?
		{
			++j;
			continue;
		}

		uint32 numOfMethods = mPacketsListeners[j]->mListeners.size();	// notify all listeners of this packet
		for(uint32 i = 0; i < numOfMethods; ++i)
			mPacketsListeners[j]->mListeners[i]->notifyAboutPacketLoss();
		delete mPacketsListeners[j];
		mPacketsListeners[j] = mPacketsListeners.back();
		mPacketsListeners.pop_back();
		--numOfPackets;
	}
}

void UDPSender::processDataToBeResent()
{
	uint32 numOfReliablePackets = mReliablePackets.size();
	for(vector<StoredReliableData *>::iterator it = mReliablePackets.begin(); it != mReliablePackets.end(); ++it)
	{
		if (!(*it)->hasAckExpired())	// resent necessary?
			continue;

		uint32 requiredMemorySize = (*it)->getDataSize();	// enough space?
		if (!mWriter.containsAckRequestMessage())
			requiredMemorySize += UDPPacketWriter::ACK_MESSAGE_SIZE;
		if (mWriter.getFreeMemorySize() < requiredMemorySize)
			continue;

		mWriter.addFormattedUserData((*it)->getData(), (*it)->getDataSize());

		if (!mWriter.containsAckRequestMessage())
		{
			mWriter.addAckRequestMessage(mNextAckNumber);
			(*it)->reset(mNextAckNumber++);
			return;
		}

		(*it)->reset(mWriter.getAckRequestNumber());
		if (mReliablePackets.back()->getAckNumber() != mWriter.getAckRequestNumber())
			return;
		mReliablePackets.back()->addData((*it)->getData(), (*it)->getDataSize()); // do not store two with the same ack numOfSerializables
		delete *it;
		mReliablePackets.erase(it);
	}
}