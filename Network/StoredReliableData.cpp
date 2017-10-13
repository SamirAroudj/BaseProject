/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _LINUX
#include <cstring>
#endif

#include <string>
#include "StoredReliableData.h"

using namespace Network;

StoredReliableData::StoredReliableData(const char *data, uint32 dataSize, Real timeoutPeriod, uint16 ackNumber) :
		mTimeOut(timeoutPeriod), mDataSize(dataSize), 
		mAckNumber(ackNumber),
		mData(new char[dataSize])
{
	memcpy(mData, data, mDataSize);
}

StoredReliableData::~StoredReliableData()
{
	delete [] mData;
}

void StoredReliableData::addData(const char *data, uint32 dataSize)
{
	uint32 newDataSize = mDataSize + dataSize;
	char *newData = new char[newDataSize];

	memcpy(newData, mData, mDataSize);
	memcpy(newData + mDataSize, data, dataSize);

	delete [] mData;
	mData = newData;
	mDataSize = newDataSize;
}
		
void StoredReliableData::reset(uint16 ackNumber)
{
	mTimeOut.reset();
	mAckNumber = ackNumber;
}

void StoredReliableData::reset(Real newTimePeriod, uint16 ackNumber)
{
	mTimeOut.reset(newTimePeriod);
	mAckNumber = ackNumber;
}