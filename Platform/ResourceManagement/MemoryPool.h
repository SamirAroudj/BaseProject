/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#include "Platform/DataTypes.h"
#include "Bucket.h"

namespace ResourceManagement
{
    /// Realizes memory management by means of pools which consist of Bucket objcts containing equally sized memory chunks.
	class MemoryPool
	{
	public:
        /** Creates a memory pool consisting of several Bucket objects for efficient memory requests.
        @param bucketCapacities Defines the chunk count for each Bucket object to be created.
        @param bucketGranularities Defines the chunk size in bytes for each Bucket object to be created.
        @param numOfBuckets Defines the number of buckets to be created. (= size of capacities & granularities) */
		MemoryPool(const uint16 *bucketCapacities, const uint16 *bucketGranularities, uint32 numOfBuckets);

        /** Releases this memory pool including all Bucket objects it allocated. */
		~MemoryPool();

        /** Returns NULL if the allocation fails.
        @param capacity Set this to the number of bytes you want.
        @return Returns Null on failure or a pointer to the first byte of the memory chunk you requested. */
		void *requestMemory(size_t capacity);

        /** Frees a piece of memory that was previously requested from this Memory Pool object.
        @param pointer You must set this to a pointer you got from a call to requestMemory.
            Function call does nothing if pointer is NULL. */
		void releaseMemory(void *pointer);

	private:
        Bucket  **mBuckets;             /// These container manage equally sized memory pieces per bucket.
        uint8   *mBaseMemory;           /// Buckets are placed in this memory and are used to implement an own new operator.
        uint32  mNumOfBuckets;          /// Defines the number of bucket pointers in mBuckets.

		#ifdef _DEBUG
			uint32 mNumOfRemainingFrees;    /// Tracks how many memory pieces that couldn't be retrieved from a Bucket must be freed.
		#endif // _DEBUG
	};
}

#endif // _MEMORY_POOL_H_
