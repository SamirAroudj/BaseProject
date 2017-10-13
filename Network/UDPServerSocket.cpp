/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Session.h"
#include "UDPServerSocket.h"

using namespace Network;
using namespace Platform;
using namespace std;

UDPServerSocket::UDPServerSocket(uint32 IPAddress) : mLastSender(0)
{
    char loop = 0;
	bind(IPAddress);
    mMulticastSupport = Network::getMulticastSupport(mSocketHandle, IPAddress);
    setsockopt(mSocketHandle, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));  // multicast packets are not looped back to the sending socket
}

UDPServerSocket::~UDPServerSocket()
{
	for(uint32 i = 0; i < mSenders.size(); ++i)
		delete mSenders[i];
}

vector<uint16> UDPServerSocket::addMessage(MessageType messageIdentifier,
										   const Patterns::ISerializable *serializables, uint32 numOfSerializables,
										   UDPPacket::Delivery delivery, IPacketLossListener *listener)
{
	vector<uint16> failedReceiverIdentifiers;
	const vector<const SessionMember *> &members = Session::getSingleton().getMembers();

	if (!mMulticastSupport) // server does not support explicit multicast messages? -> send copies to update each client
	{
		uint32 numOfSenders = mSenders.size();
		for(uint32 i = 0; i < numOfSenders; ++i)
			if (!mSenders[i]->addMessage(messageIdentifier, serializables, numOfSerializables, delivery, listener))
				failedReceiverIdentifiers.push_back(mSenders[i]->getIdentifier());
		return failedReceiverIdentifiers;
	}

	bool multicastSuccess = mMulticastSender.addMessage(messageIdentifier, serializables, numOfSerializables, delivery, listener);
	uint32 numOfMembers = members.size();
	for(uint32 i = 1; i < numOfMembers; ++i) // do not send to the server ;)
		if (members[i]->getMulticastSupport() && !multicastSuccess)
				failedReceiverIdentifiers.push_back(members[i]->getIdentifier());
		else if (!members[i]->getMulticastSupport() &&
			!addMessage(messageIdentifier, serializables, members[i]->getIdentifier(), numOfSerializables, delivery, listener))
				failedReceiverIdentifiers.push_back(members[i]->getIdentifier());
	return failedReceiverIdentifiers;
}

bool UDPServerSocket::addMessage(MessageType messageIdentifier, const Patterns::ISerializable *serializables, uint32 numOfSerializables,
								 uint16 memberIdentifier, UDPPacket::Delivery delivery, IPacketLossListener *listener)
{
	for(uint32 i = 0; i < mSenders.size(); ++i)
		if (memberIdentifier == mSenders[i]->getIdentifier())
			return mSenders[i]->addMessage(messageIdentifier, serializables, numOfSerializables, delivery, listener);

	assert(false);
	return false;
}

void UDPServerSocket::notifyNewMember(const SessionMember &newMember)
{
	if (SERVER_IDENTIFIER == newMember.getIdentifier()) // the server shouldn't send data to itself ;)
		return;
	
	UDPServerEnd *newSender = new UDPServerEnd(newMember.getIPAddress(), newMember.getUDPPortNumber(), newMember.getIdentifier());
	mSenders.push_back(newSender);
	if (newMember.getMulticastSupport())
		mMulticastSender.addMulticastSender(newSender);
}

bool UDPServerSocket::notifyReceivingIsPossible()
{
	UDPSocket::notifyReceivingIsPossible();

	uint32 numOfSenders = mSenders.size();
	for(uint32 i = 0; !mAckRequests.empty() && (i < numOfSenders); ++i)
		mSenders[i]->processAckRequests(mAckRequests);
	mMulticastSender.processAckResponses(mAckResponses);
	for(uint32 i = 0; !mAckResponses.empty() && (i < numOfSenders); ++i)
		mSenders[i]->processAckResponses(mAckResponses);

	return true;
}

void UDPServerSocket::notifyRemovedMember(uint16 identifier)
{
	uint32 numOfSenders = mSenders.size();
	for(uint32 i = 0; i < numOfSenders; ++i)
	{
		if (mSenders[i]->getIdentifier() == identifier)
		{
			if (Session::getSingleton().getMember(identifier)->getMulticastSupport())
				mMulticastSender.removeSender(mSenders[i]);
			delete mSenders[i];
			mSenders[i] = mSenders.back();
			mSenders.pop_back();
			return;
		}
	}

	assert(false);
}

void UDPServerSocket::send()
{
	if (!mReadyToSend)
		return;
	mMulticastSender.tryToSendData(mSocketHandle);

	sockaddr_in receiverAddress;
	receiverAddress.sin_family = AF_INET;
	memset(receiverAddress.sin_zero, 0, 8);

	for(uint32 i = mLastSender; i < mSenders.size(); ++i)	// if sending to a receiver failed previously due to WOULD_BLOCK (e.g. a busy UDP layer)
	{															// then this is the first receiver to start sending in order to avoid discrimination against rear receivers
		if (!mSenders[i]->tryToSendData(mSocketHandle, receiverAddress))
		{
			mLastSender = i;
			mReadyToSend = false;
			return;
		}
	}

	for(uint32 i = 0; i < mLastSender; ++i)	// just continue sending so that every sender tried to send, unless there is a WOULD_BLOCK
	{
		if (!mSenders[i]->tryToSendData(mSocketHandle, receiverAddress))
		{
			mLastSender = i;
			mReadyToSend = false;
			return;
		}
	}
	mLastSender = 0;
}

void UDPServerSocket::update()
{
	Real networkTime = ApplicationTimer::getSingleton().getSystemTime();
	mMulticastSender.setNetworkTime(networkTime);
	mMulticastSender.computePacketLossNotifications();
	for(uint32 i = 0; i < mSenders.size(); ++i)
	{
		mSenders[i]->setNetworkTime(networkTime);
		mSenders[i]->computePacketLossNotifications();
	}

	if (!mSendPeriod.hasExpired())
		return;
	mSendPeriod.reset();	// reset the limit of bytes which can be sent
	mMulticastSender.startNewSendPeriod();
	for(uint32 i = 0; i < mSenders.size(); ++i)
		mSenders[i]->startNewSendPeriod();
}
