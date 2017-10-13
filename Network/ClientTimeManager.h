/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _CLIENT_TIME_MANAGER_H_
#define _CLIENT_TIME_MANAGER_H_

#include "Platform/ApplicationTimer.h"
#include "MagicNetworkConstants.h"
#include "UDPPacketReader.h"
#include "UDPPacketWriter.h"

namespace Network
{
	class ClientTimeManager : public Patterns::Singleton<ClientTimeManager>
	{
	public:
		ClientTimeManager();
		virtual ~ClientTimeManager() { }

		Real getNetworkTime() const
		{ 
			return (!mReadyToUse) ? 0 : Platform::ApplicationTimer::getSingleton().getSystemTime() - mClientsLastUpdateTime + mServersLastUpdateTime;
		}
		uint16 addTimeRequestMessage(UDPPacketWriter &packetWriter);

		// returns the round trip time of a slow packet in seconds
		Real getBadRoundTripTime() const { return(!mReadyToUse) ? DEFAULT_BAD_ROUND_TRIP_TIME : mBadRoundTripTime; }

		void processReceivedServerResponse(UDPPacketReader &reader);
		bool requiresServerUpdate() const { return mReadyToUse ? mRequestTimePeriod.hasExpired() : true; }

	private:
		void synchronizeClock(Real serverTime, Real clientReceiptTime, Real clientRequestTime, Real systemTime);

		Platform::TimePeriod mRequestTimePeriod;
		/// <summary> contains recent round trip times in milliseconds </summary>
		uint32 mRecentRoundTripTimes[MAX_NUM_OF_RECENT_ROUND_TRIP_TIMES];
		Real mClientsLastUpdateTime;
		Real mServersLastUpdateTime;
		uint32 mNextRoundTripTimeToUpdate;
		Real mBadRoundTripTime;
		bool mInitialRequest;
		bool mReadyToUse;
	};
}

#endif // _CLIENT_TIME_MANAGER_H_