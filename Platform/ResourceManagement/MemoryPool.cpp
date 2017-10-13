/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <new>
#include "MemoryPool.h"

using namespace ResourceManagement;

MemoryPool::MemoryPool(const uint16 *bucketCapacities, const uint16 *bucketGranularities, uint32 numOfBuckets) :
	mNumOfBuckets(numOfBuckets)
{
	assert(bucketCapacities && bucketGranularities && numOfBuckets > 0);
	#ifdef _DEBUG
		mNumOfRemainingFrees = 0;
	#endif // _DEBUG

	uint32 sizeOfBaseMemory = sizeof(Bucket *) * mNumOfBuckets; // compute size of memory required by the buckets
	for (uint32 i = 0; i < mNumOfBuckets; ++i)
		sizeOfBaseMemory += sizeof(Bucket) + bucketCapacities[i] * (sizeof(uint16) + bucketGranularities[i]);
	
	mBaseMemory = reinterpret_cast<unsigned char *>(malloc(sizeOfBaseMemory)); // memory needed by buckets
	#ifdef ACTIVE_MEMORY_DESTRUCTION
		memset(mBaseMemory, 0xcd, sizeOfBaseMemory);
	#endif // ACTIVE_MEMORY_DESTRUCTION

	mBuckets = reinterpret_cast<Bucket **>(mBaseMemory);
	unsigned char *bucketStoragePosition = mBaseMemory + sizeof(Bucket *) * mNumOfBuckets;
	for (uint32 i = 0; i < mNumOfBuckets; ++i)
	{
		mBuckets[i] = new(bucketStoragePosition) Bucket(bucketGranularities[i], bucketCapacities[i]);
		bucketStoragePosition += sizeof(Bucket) + bucketCapacities[i] * (sizeof(uint16) + bucketGranularities[i]);
	}
} 

MemoryPool::~MemoryPool()
{
	#ifdef _DEBUG 	// all chunks that were requested by malloc instead of a bucket must also be freed
		assert(0 == mNumOfRemainingFrees);
	#endif // _DEBUG

	#ifdef ACTIVE_MEMORY_DESTRUCTION	// buckets already memset the memory they manage, but this is also done to destroy admin's and buckets' data
		uint32 sizeOfBaseMemory = sizeof(Bucket *) * mNumOfBuckets;
		for (uint32 i = 0; i < mNumOfBuckets; ++i)
			sizeOfBaseMemory += sizeof(Bucket) + mBuckets[i]->getCapacity() * (sizeof(uint16) + mBuckets[i]->getGranularity());
	#endif // ACTIVE_MEMORY_DESTRUCTION

	for (uint32 i = 0; i < mNumOfBuckets; ++i)
		mBuckets[i]->~Bucket();

	#ifdef ACTIVE_MEMORY_DESTRUCTION
		memset(mBaseMemory, 0xcd, sizeOfBaseMemory);
	#endif // ACTIVE_MEMORY_DESTRUCTION

	free(mBaseMemory);
}

void *MemoryPool::requestMemory(size_t capacity)
{
	for (uint32 i = 0; i < mNumOfBuckets; ++i)	// can a bucket handle this request?
		if (mBuckets[i]->getGranularity() >= capacity && !mBuckets[i]->isFull())
			return mBuckets[i]->requestMemory(capacity);

	void *memory = malloc(capacity);	// malloc is necessary
    #ifdef _DEBUG
        ++mNumOfRemainingFrees;
    #endif // _DEBUG
    #ifdef ACTIVE_MEMORY_DESTRUCTION
        memset(memory, 0xcd, capacity);
    #endif // ACTIVE_MEMORY_DESTRUCTION
	return memory;
}

void MemoryPool::releaseMemory(void *pointer)
{
    if (NULL == pointer)
        return;

	for (uint32 i = 0; i < mNumOfBuckets; ++i)	// is a bucket responsible?
		if (mBuckets[i]->releaseMemory(pointer))
            return;

    #ifdef _DEBUG	// memory was requested by malloc
        assert(mNumOfRemainingFrees > 0);
        --mNumOfRemainingFrees;
    #endif // _DEBUG

	free(pointer);
}
