/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _SERVER_TIME_MANAGER_H_
#define _SERVER_TIME_MANAGER_H_

#include <vector>
#include "Platform/DataTypes.h"
#include "ISessionListener.h"
#include "MagicNetworkConstants.h"
#include "Patterns/Singleton.h"

namespace Network
{
	class ClientRoundTripTimes
	{
	public:
		ClientRoundTripTimes(uint16 identifier);
	
		void addRoundTripTime(Real roundTripTime);
		Real getBadRoundTripTime() const { return (!mReadyToUse) ? DEFAULT_BAD_ROUND_TRIP_TIME : mBadRoundTripTime; }
		uint16 getIdentifier() const { return mIdentifier; }

	private:
		Real mRecentRoundTripTimes[MAX_NUM_OF_RECENT_ROUND_TRIP_TIMES];
		Real mBadRoundTripTime;
		uint32 mNextRoundTripTimeToUpdate;
		uint16 mIdentifier;
		bool mReadyToUse;
	};

	class ServerTimeManager : public Patterns::Singleton<ServerTimeManager>, public ISessionListener
	{
	public:
		ServerTimeManager();
		virtual ~ServerTimeManager();

		void addRoundTripTime(uint16 sessionMemberIdentifier, Real roundTripTime);
		// returns the round trip time of slower packets in seconds
		Real getBadRoundTripTime(uint16 sessionMemberIdentifier);
		// returns the mean round trip time of slower packets in seconds (mean between all clients)
		Real getMeanBadRoundTripTime();

		virtual void notifyNewMember(const SessionMember &newMember);
		virtual void notifyRemovedMember(uint16 identifier);

	private:
		std::vector<ClientRoundTripTimes *> mRoundTripTimes;
	};
}

#endif // _SERVER_TIME_MANAGER_H_
