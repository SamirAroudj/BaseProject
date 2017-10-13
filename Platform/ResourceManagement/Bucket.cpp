/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cassert>
#include <cstring>
#include <new>
#include "Bucket.h"
#include "Platform/Logging/LogManager.h"

using namespace Logging;
using namespace ResourceManagement;
using namespace std;

Bucket::Bucket(uint16 granularity, uint16 capacity) :
	mCapacity(capacity), mGranularity(granularity), mNumOfFreeChunks(capacity)
{
	assert(capacity > 0 && granularity > 0);
    mFreeChunks  = reinterpret_cast<uint16 *>(this + 1); //(uint16 *) (((uint16 *) this) + sizeof(Bucket));
    mBasePointer = reinterpret_cast<uint8 *>(mFreeChunks + mCapacity); //((uint8 *) m_freeChunks) + capacity * sizeof(uint16);

	for (uint32 i = 0; i < mCapacity; ++i)
		mFreeChunks[i] = i;
	
	#ifdef LOGGING
		char buffer[200];
		sprintf(buffer, "Bucket created, granularity: %d, capacity: %d", granularity, capacity);
		LogManager::log(LogManager::TRACE, buffer);
	#endif // LOGGING
}

Bucket::~Bucket()
{
	#ifdef LOGGING
		if (mNumOfFreeChunks < mCapacity)															// output all chunk indices of chunks which were not freed
		{
			char buffer[200];
			uint32 unfreedChunks = mCapacity - mNumOfFreeChunks;

			sprintf(buffer, "Not every chunk of a bucket was freed, capacity: %d, granularity: %d,\
							unfreed chunks count: %d", mCapacity, mGranularity, unfreedChunks);			// general bucket info
			LogManager::log(LogManager::WARNING, buffer);
			LogManager::log(LogManager::WARNING, "Indices of forgotten chunks:");

			uint32	memorySize	= sizeof(bool) * mCapacity;												// find unfreed chunks							
			bool	*freed		= (bool *) malloc(memorySize);
			memset(freed, false, memorySize);
			for (uint32 i = 0; i < mNumOfFreeChunks; ++i)
				freed[mFreeChunks[i]] = true;

			for (uint32 i = 0; i < mCapacity; ++i)														// output unfreed chunks
			{
				if (!freed[i])
				{
					sprintf(buffer, "chunk index: %d", i);
					LogManager::log(LogManager::WARNING, buffer);
				}
			}

			free(freed);
		}
	#endif // LOGGING
	
	assert(mNumOfFreeChunks == mCapacity);

    #ifdef ACTIVE_MEMORY_DESTRUCTION	// destroy all data in memory to make sure that it is not accidently used anymore
        memset(this, 0xcd, sizeof(Bucket) + mCapacity * (sizeof(uint16) + mGranularity));
    #endif // ACTIVE_MEMORY_DESTRUCTION
}

const uint32 Bucket::getChunkSize(void *pointer) const
{
    size_t index = (reinterpret_cast<uint8 *>(pointer) - mBasePointer) / mGranularity;
	if (index >= mCapacity)
		return 0;
	else
		return getGranularity();
}

void *Bucket::requestMemory(size_t capacity)
{
	assert(capacity <= mGranularity && mNumOfFreeChunks > 0);
    if (0 == mNumOfFreeChunks)
        return NULL; // todo log this
    if (capacity > mGranularity)
        return NULL; // todo log this

	--mNumOfFreeChunks;
	return reinterpret_cast<void *>(mBasePointer + mGranularity * mFreeChunks[mNumOfFreeChunks]);
}

bool Bucket::releaseMemory(void *pointer)
{
    size_t diff  = reinterpret_cast<uint8 *>(pointer) - mBasePointer;
    size_t index = diff / mGranularity;
	if (index >= mCapacity)
		return false;
    assert(0 == static_cast<uint32>(diff) % mGranularity);

    #ifdef ACTIVE_MEMORY_DESTRUCTION
        memset(pointer, 0xcd, mGranularity);
    #endif // ACTIVE_MEMORY_DESTRUCTION

    /*#ifdef _DEBUG
        for (uint32 i = 0; i < mNumOfFreeChunks; ++i)
            assert(mFreeChunks[i] != index);
    #endif // _DEBUG*/

	mFreeChunks[mNumOfFreeChunks++] = static_cast<uint16>(index);
	return true;
}
