/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _LINUX
#include <cstring>
#endif // _LINUX

#include "ClientTimeManager.h"
#include "Utilities.h"

using namespace Network;
using namespace Platform;

ClientTimeManager::ClientTimeManager() : 
	mRequestTimePeriod(REQUEST_TIME_PERIOD),
	mClientsLastUpdateTime(ApplicationTimer::getSingleton().getSystemTime()),
	mServersLastUpdateTime(0),
	mNextRoundTripTimeToUpdate(0),
	mBadRoundTripTime(0.0f),
	mInitialRequest(true),
	mReadyToUse(false)
{
	memset(mRecentRoundTripTimes, 0, sizeof(uint32) * MAX_NUM_OF_RECENT_ROUND_TRIP_TIMES);
}

uint16 ClientTimeManager::addTimeRequestMessage(UDPPacketWriter &packetWriter)
{
	if (packetWriter.containsTimeRequestMessage())
		return 0;

	Real clientRequestTime = Platform::ApplicationTimer::getSingleton().getSystemTime() - mClientsLastUpdateTime + mServersLastUpdateTime;
	uint16 bytesAdded = packetWriter.addTimeRequestMessage(clientRequestTime, mInitialRequest);
	if (0 != bytesAdded)
		mRequestTimePeriod.reset();

	return bytesAdded;
}

void ClientTimeManager::processReceivedServerResponse(UDPPacketReader &reader)
{
	assert(UDPPacket::TIME_RESPONSE == reader.getCurrentMessageType());

	Real clientRequestTime, serverTime;	// compute the round trip time of this time request message 
	Real systemTime = Platform::ApplicationTimer::getSingleton().getSystemTime();
	Real clientReceiptTime = systemTime - mClientsLastUpdateTime + mServersLastUpdateTime;
	reader.readTimeResponseMessage(clientRequestTime, serverTime);
	uint32 roundTripTime = static_cast<uint32>(0.5f + 1000.0f * (clientReceiptTime - clientRequestTime));
	if (roundTripTime >= 1000 * REQUEST_TIME_PERIOD)// this is a response that was delayed by the network for a long time
		return;								// therefore, it is useless
	mRecentRoundTripTimes[mNextRoundTripTimeToUpdate++] = roundTripTime;

	if (mInitialRequest)
	{
		synchronizeClock(serverTime, clientReceiptTime, clientRequestTime, systemTime);
		mInitialRequest = false;
	}

	if (MAX_NUM_OF_RECENT_ROUND_TRIP_TIMES == mNextRoundTripTimeToUpdate)
	{
		mNextRoundTripTimeToUpdate = 0;
		mReadyToUse = true;
	}

	if (!mReadyToUse)	// try to synchronize the client's clock accuracy
		return;
	uint32 orderedRoundTripTimes[MAX_NUM_OF_RECENT_ROUND_TRIP_TIMES]; // create an array containing the round-trip times in ascending order
	Utilities::insertionSort<uint32>(mRecentRoundTripTimes, orderedRoundTripTimes, MAX_NUM_OF_RECENT_ROUND_TRIP_TIMES);
	
	mBadRoundTripTime = 0.001f * orderedRoundTripTimes[FIRST_INDEX_BAD_ROUND_TRIP_TIMES];

	if (orderedRoundTripTimes[LAST_INDEX_BEST_ROUND_TRIP_TIMES] < roundTripTime)	// was this packet fairly fast to provide time data that is accurate enough to synchronise the network clock?
		return;

	synchronizeClock(serverTime, clientReceiptTime, clientRequestTime, systemTime);
}

void ClientTimeManager::synchronizeClock(Real serverTime, Real clientReceiptTime, Real clientRequestTime, Real systemTime)
{
	Real deviation = 0;	// what is the difference between the client clock and the server clock?
	Real desiredNetworkTime = serverTime + (clientReceiptTime - clientRequestTime) * 0.5f;
	if (desiredNetworkTime > clientReceiptTime)
		deviation = 1000.0f * (desiredNetworkTime - clientReceiptTime);
	else
		deviation = 1000.0f * (clientReceiptTime - desiredNetworkTime);
	if (deviation > MAX_CLIENT_CLOCK_DEVIATION)	// only update the clock if there is too much difference to avoid jittering of the clock
	{
		mServersLastUpdateTime = desiredNetworkTime;
		mClientsLastUpdateTime = systemTime;
	}
}