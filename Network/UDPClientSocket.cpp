/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "MagicNetworkConstants.h"
#include "Network.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"
#include "Session.h"
#include "StoredReliableData.h"
#include "UDPClientSocket.h"

using namespace Network;
using ExceptionHandling::NetworkExceptionFactory;
using namespace std;

UDPClientSocket::UDPClientSocket(uint32 IPAddress) :
	UDPSocket(false),
	UDPSender(MAX_BYTES_PER_SEND_PERIOD_TO_SERVER - MIN_DATAGRAM_OVERHEAD),
	mMulticastSupport(0)
{
	mServerAddress.sin_addr.s_addr = mServerAddress.sin_port = mServerAddress.sin_family = 0;
	ip_mreq multicastRequest;;
	multicastRequest.imr_multiaddr.s_addr = htonl(MULTICAST_ADDRESS);
	multicastRequest.imr_interface.s_addr = htonl(IPAddress);

	if (SOCKET_ERROR != setsockopt(mSocketHandle, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char *)&multicastRequest, sizeof(multicastRequest)))
	{	
		mMulticastSupport = true;
		bind(IPAddress, MULTICAST_PORT_NUMBER);
		return;
	}

	if (NO_MULTICAST_SUPPORT == getError())// explicit multicasting is not supported
	{
		mMulticastSupport = false;
		bind(IPAddress);
		return;
	}

	NetworkExceptionFactory::throwNetworkException("Could not detect multicast support.", getError()); // unspecified error
}

UDPClientSocket::~UDPClientSocket()
{

}

bool UDPClientSocket::notifyReceivingIsPossible()
{
	UDPSocket::notifyReceivingIsPossible();

	while(!mAckRequests.empty())	// process ack requests -> create responses
	{
		assert(mAckRequests.front().mSenderIdentifier == 0);
		uint16 bytesAdded = mWriter.addAckResponseMessage(mAckRequests.front().mAckNumber);
		if (0 == bytesAdded)
			break;
		mResidualPeriodBytes = (mResidualPeriodBytes > bytesAdded) ? mResidualPeriodBytes - bytesAdded : 0;
		mAckRequests[0] = mAckRequests.back();
		mAckRequests.pop_back();
	}

	processAckResponses();

	return true;
}

void UDPClientSocket::processAckResponses()
{
	while(!mAckResponses.empty())
	{
		assert(mAckResponses.back().mSenderIdentifier == SERVER_IDENTIFIER);

		uint32 numOfReliablePackets = mReliablePackets.size();
		for(uint32 i = 0; i < numOfReliablePackets; ++i)
		{
			if (mAckResponses.back().mAckNumber == mReliablePackets[i]->getAckNumber())
			{
				delete mReliablePackets[i];
				mReliablePackets[i] = mReliablePackets.back();
				mReliablePackets.pop_back();
				break;
			}
		}

		uint32 numOfLossInfoPackets = mPacketsListeners.size();
		for(uint32 i = 0; i < numOfLossInfoPackets; ++i)
		{
			if (mAckResponses.back().mAckNumber == mPacketsListeners[i]->mPacketAckNumber)
			{
				delete mPacketsListeners[i];
				mPacketsListeners[i] = mPacketsListeners.back();
				mPacketsListeners.pop_back();
				break;
			}
		}

		mAckResponses.pop_back();	// remove the response anyway (it could be a late response which does not match to any candidate)
	}
}

void UDPClientSocket::send()
{
	if (!mReadyToSend)
		return;

	processDataToBeResent();
	if (ClientTimeManager::getSingleton().requiresServerUpdate())	// add time request?
		mResidualPeriodBytes -= ClientTimeManager::getSingleton().addTimeRequestMessage(mWriter);

	if (UDPPacket::HEADER_SIZE == mWriter.getPacketSize())
		return;

	int32 bytesSent = sendto(mSocketHandle, mWriter.getPacketData(), mWriter.getPacketSize(), 0,	// try to send and check for errors
		reinterpret_cast<sockaddr *>(&mServerAddress), sizeof(sockaddr));
	
	if (SOCKET_ERROR == bytesSent)
	{
		int32 errorCode = getError();
		if (WOULD_BLOCK == errorCode)
		{
			mReadyToSend = false;
			return;
		}
		NetworkExceptionFactory::throwNetworkException("Unable to send datagrams to the server.", errorCode);
	}
	// data has been sent so clear the packet writer
	mWriter.endPacketAssembly();
	mResidualPeriodBytes = (mResidualPeriodBytes > MIN_DATAGRAM_OVERHEAD) ? mResidualPeriodBytes - MIN_DATAGRAM_OVERHEAD : 0u;
}

void UDPClientSocket::update()
{
	computePacketLossNotifications();
	if (!mSendPeriod.hasExpired())	// new send period started? -> udpate timer / residual bytes
		return;

	mSendPeriod.reset();
	mResidualPeriodBytes = MAX_BYTES_PER_SEND_PERIOD_TO_SERVER - MIN_DATAGRAM_OVERHEAD;
	setNetworkTime(ClientTimeManager::getSingleton().getNetworkTime());
}