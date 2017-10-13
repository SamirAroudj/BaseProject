/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Session.h"
#include "ServerTimeManager.h"
#include "Utilities.h"

using namespace Network;

ClientRoundTripTimes::ClientRoundTripTimes(uint16 identifier) :
	mBadRoundTripTime(1.0f), mNextRoundTripTimeToUpdate(0), mIdentifier(identifier), mReadyToUse(false)
{
	for(uint32 i = 0; i < MAX_NUM_OF_RECENT_ROUND_TRIP_TIMES; ++i)
		mRecentRoundTripTimes[i] = 0.0f;
}

void ClientRoundTripTimes::addRoundTripTime(Real roundTripTime)
{
	Real orderedRoundTripTimes[MAX_NUM_OF_RECENT_ROUND_TRIP_TIMES];
	mRecentRoundTripTimes[mNextRoundTripTimeToUpdate++] = roundTripTime;

	if (MAX_NUM_OF_RECENT_ROUND_TRIP_TIMES == mNextRoundTripTimeToUpdate)
	{
		mReadyToUse = true;
		mNextRoundTripTimeToUpdate = 0;
	}

	Utilities::insertionSort<Real>(mRecentRoundTripTimes, orderedRoundTripTimes, MAX_NUM_OF_RECENT_ROUND_TRIP_TIMES);
	mBadRoundTripTime = orderedRoundTripTimes[FIRST_INDEX_BAD_ROUND_TRIP_TIMES];
}

ServerTimeManager::ServerTimeManager()
{
	Session::getSingleton().addListener(this);
}

ServerTimeManager::~ServerTimeManager()
{
	uint32 numOfClients = mRoundTripTimes.size();
	for(uint32 i = 0; i < numOfClients; ++i)
		delete mRoundTripTimes[i];
	Session::getSingleton().removeListener(this);
}

void ServerTimeManager::addRoundTripTime(uint16 sessionMemberIdentifier, Real roundTripTime)
{
	uint32 numOfClients = mRoundTripTimes.size();
	for(uint32 i = 0; i < numOfClients; ++i)
	{
		if (sessionMemberIdentifier == mRoundTripTimes[i]->getIdentifier())
		{
			mRoundTripTimes[i]->addRoundTripTime(roundTripTime);
			return;
		}
	}

	assert(false);
}

Real ServerTimeManager::getBadRoundTripTime(uint16 sessionMemberIdentifier)
{
	uint32 numOfClients = mRoundTripTimes.size();
	for(uint32 i = 0; i < numOfClients; ++i)
		if (sessionMemberIdentifier == mRoundTripTimes[i]->getIdentifier())
			return mRoundTripTimes[i]->getBadRoundTripTime();

	assert(false);
	return 0.0f;
}

Real ServerTimeManager::getMeanBadRoundTripTime()
{
	const uint32 numOfClients = mRoundTripTimes.size();
	Real mean = 0.0f;
	const Real one_numOfClients = 1.0f / numOfClients;

	for(uint32 i = 0; i < numOfClients; ++i)
		mean += mRoundTripTimes[i]->getBadRoundTripTime();
	return mean * one_numOfClients; 
}

void ServerTimeManager::notifyNewMember(const SessionMember &newMember)
{
	if (0 == newMember.getIdentifier())
		return;

	ClientRoundTripTimes *clientTimes = new ClientRoundTripTimes(newMember.getIdentifier());
	mRoundTripTimes.push_back(clientTimes);
}

void ServerTimeManager::notifyRemovedMember(uint16 identifier)
{	
	uint32 numOfClients = mRoundTripTimes.size();
	for(uint32 i = 0; i < numOfClients; ++i)
	{
		if (mRoundTripTimes[i]->getIdentifier() == identifier)
		{
			delete mRoundTripTimes[i];
			mRoundTripTimes[i] = mRoundTripTimes.back();
			mRoundTripTimes.pop_back();
			return;
		}
	}
}
