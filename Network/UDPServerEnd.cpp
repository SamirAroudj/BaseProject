/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"
#include "StoredReliableData.h"
#include "UDPServerEnd.h"

using namespace ExceptionHandling;
using namespace Network;
using namespace std;

UDPServerEnd::UDPServerEnd(uint32 IPAddress, uint16 portNumber, uint16 identifier) :
	UDPSender(MAX_BYTES_PER_SEND_PERIOD_TO_CLIENT - MIN_DATAGRAM_OVERHEAD),
	mIPAddressNetworkByteOrder(htonl(IPAddress)), mPortNumberNetworkByteOrder(htons(portNumber)), mIdentifier(identifier)
{
}

UDPServerEnd::~UDPServerEnd()
{
}

bool UDPServerEnd::addLostData(const StoredReliableData &data)
{
	uint32 requiredMemorySize = data.getDataSize() + (mWriter.containsAckRequestMessage() ? 0 : mWriter.getAckRequestMessageSize());
	if (requiredMemorySize > mResidualPeriodBytes || requiredMemorySize > mWriter.getFreeMemorySize())
		return false;

	if (!mWriter.containsAckRequestMessage())
			mWriter.addAckRequestMessage(mNextAckNumber++);
	mWriter.setUserDataAdded();
	mResidualPeriodBytes -= requiredMemorySize;

	if (mReliablePackets.empty() || mReliablePackets.back()->getAckNumber() != mWriter.getAckRequestNumber()) // new packet?
		mReliablePackets.push_back(new StoredReliableData(data.getData(), data.getDataSize(), getTimeoutPeriod(), mWriter.getAckRequestNumber()));
	else
		mReliablePackets.back()->addData(data.getData(), data.getDataSize());
	return true;
}

void UDPServerEnd::processAckRequests(vector<SenderIdentifierAckPair> &ackRequests)
{
	uint32 numOfRequests = ackRequests.size();
	for(uint32 i = 0; i < numOfRequests; )
	{
		if (ackRequests[i].mSenderIdentifier != mIdentifier)	// request for this server end?
		{
			++i;
			continue;
		}

		uint16 bytesAdded = mWriter.addAckResponseMessage(ackRequests[i].mAckNumber);	// try to add a response
		if (0 == bytesAdded)
			return;
		ackRequests[i] = ackRequests.back();
		ackRequests.pop_back();
		--numOfRequests;
		mResidualPeriodBytes = (mResidualPeriodBytes > bytesAdded) ? mResidualPeriodBytes - bytesAdded : 0;
	}
}

void UDPServerEnd::processAckResponses(std::vector<SenderIdentifierAckPair> &ackResponses)
{
	uint32 numOfResponses = ackResponses.size();
	for(uint32 i = 0; i < numOfResponses; )
	{
		if (ackResponses[i].mSenderIdentifier != mIdentifier)	// response for this server end?
		{
			++i;
			continue;
		}

		updatePacketLossInfoAndStoredReliableData(ackResponses[i].mAckNumber);
		ackResponses[i] = ackResponses.back();	// remove the response anyway (it could be a late response which does not match to any candidate)
		ackResponses.pop_back();
		--numOfResponses;
	}
}

void UDPServerEnd::updatePacketLossInfoAndStoredReliableData(uint16 ackResponseNumber)
{
	uint32 numOfReliablePackets = mReliablePackets.size();	// is there data which does not need to be resent anymore?
	for(uint32 j = 0; j < numOfReliablePackets; ++j)
	{
		if (mReliablePackets[j]->getAckNumber() == ackResponseNumber)
		{
			delete mReliablePackets[j];
			mReliablePackets[j] = mReliablePackets.back();
			mReliablePackets.pop_back();
			break;
		}
	}

	uint32 numOfLossInfoPackets = mPacketsListeners.size(); // is there a loss notification method which does not need to be called anymore?
	for(uint32 j = 0; j < numOfLossInfoPackets; ++j)
	{
		if (mPacketsListeners[j]->mPacketAckNumber == ackResponseNumber)
		{
			delete mPacketsListeners[j];
			mPacketsListeners[j] = mPacketsListeners.back();
			mPacketsListeners.pop_back();
			break;
		}
	}
}

bool UDPServerEnd::tryToSendData(int32 socketHandle, sockaddr_in &receiver)
{
	processDataToBeResent();
	if (UDPPacket::HEADER_SIZE == mWriter.getPacketSize())
		return true;

	receiver.sin_addr.s_addr = mIPAddressNetworkByteOrder;
	receiver.sin_port = mPortNumberNetworkByteOrder;
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
