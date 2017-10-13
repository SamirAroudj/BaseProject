/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _STORED_RELIABLE_DATA_H_
#define _STORED_RELIABLE_DATA_H_

#include "Platform/TimePeriod.h"

namespace Network
{
	class StoredReliableData
	{
	public:
		StoredReliableData(const char *data, uint32 dataSize, Real timeoutPeriod, uint16 ackNumber);
		~StoredReliableData();

		void addData(const char *data, uint32 dataSize);

		uint16 getAckNumber() const { return mAckNumber; }
		const char *getData() const { return mData; }
		uint32 getDataSize() const { return mDataSize; }
		bool hasAckExpired() const { return mTimeOut.hasExpired(); }

		// resets time out period and sets a new ack number
		void reset(uint16 ackNumber);
		// resets time out, assigns a new period length and sets a new ack number
		void reset(Real newTimePeriod, uint16 ackNumber);

	private:
		Platform::TimePeriod mTimeOut;
		uint32 mDataSize;
		uint16 mAckNumber;
		char *mData;
	};
}
#endif // _STORED_RELIABLE_DATA_H_
