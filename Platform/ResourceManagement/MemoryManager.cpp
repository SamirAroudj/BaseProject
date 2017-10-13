/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef MEMORY_MANAGEMENT

#include <cstdlib>
#include <cstring>
#include "Platform/ResourceManagement/MemoryManager.h"

using namespace ResourceManagement;

MemoryManager	*MemoryManager::msManager = NULL;
void			*MemoryManager::msMemoryManagerMemory = NULL;

// overloaded delete operators
#ifdef _WINDOWS
	void operator delete(void *pointer)
#else
	void operator delete(void *pointer) noexcept
#endif // _WINDOWS
{
	ResourceManagement::MemoryManager::getSingleton().releaseMemory(pointer, false);
}

#ifdef _WINDOWS
	void operator delete[](void *pointer)
#else
	void operator delete[](void *pointer) noexcept
#endif // _WINDOWS
{
	ResourceManagement::MemoryManager::getSingleton().releaseMemory(pointer, true);
}

// overloaded new operators
void *operator new(size_t capacity)
{
	return ResourceManagement::MemoryManager::getSingleton().requestMemory(capacity, false);
}

void *operator new(std::size_t capacity, const std::nothrow_t& nothrow_value)
{
	return ResourceManagement::MemoryManager::getSingleton().requestMemory(capacity, false);
}

void *operator new [](size_t capacity)
{
	return ResourceManagement::MemoryManager::getSingleton().requestMemory(capacity, true);
}

void *operator new [](std::size_t capacity, const std::nothrow_t& nothrow_value)
{
	return ResourceManagement::MemoryManager::getSingleton().requestMemory(capacity, true);
}

void MemoryManager::addMemoryPool(const uint16 *bucketCapacities, const uint16 *bucketGranularities, uint32 numOfBuckets)
{
	// increase size of mMemoryPools if necessary
	if (mMaxNumOfMemoryPools <= mNumOfMemoryPools)	// reserve enough memory for the memory pool pointers
	{
		++mMaxNumOfMemoryPools;
		MemoryPool **newMemory = reinterpret_cast<MemoryPool **>(malloc(sizeof(MemoryPool *) * mMaxNumOfMemoryPools));

		if (mMemoryPools)
		{
			memcpy(newMemory, mMemoryPools, mNumOfMemoryPools * sizeof(MemoryPool *));
			free(mMemoryPools);
		}
		mMemoryPools = newMemory;
	}

	// there is no pool to manage the memory, so use malloc for the first one
	// usually created by getActiveMemoryPool & lazy initialization
	if (0 == mNumOfMemoryPools)
	{
		mMemoryPools[0] = reinterpret_cast<MemoryPool *>(malloc(sizeof(MemoryPool)));
		mMemoryPools[0] = new(mMemoryPools[0]) MemoryPool(bucketCapacities, bucketGranularities, numOfBuckets);
	}
	else	// a pool manages the memory
	{
		mMemoryPools[mNumOfMemoryPools] = new MemoryPool(bucketCapacities, bucketGranularities, numOfBuckets);
	}

	++mNumOfMemoryPools;
}

void MemoryManager::deleteMemoryPool(uint32 memoryPoolIndex)
{
	assert(memoryPoolIndex < mNumOfMemoryPools);
	

	if (0 == memoryPoolIndex)	// the pool with index 0 is always created by means of malloc
	{	// it keeps index 0 since all other pools must be freed before the pool with index 0
		mMemoryPools[0]->~MemoryPool();	// otherwise this destructor has a failing assert
		free(mMemoryPools[0]);
		mNumOfMemoryPools = 0;
		return;
	}

	MemoryPool *toBeDeleted = mMemoryPools[memoryPoolIndex];
	for (uint32 i = memoryPoolIndex; i < mNumOfMemoryPools - 1; ++i)
		mMemoryPools[i] = mMemoryPools[i + 1];
	--mNumOfMemoryPools;
	delete toBeDeleted;
}

MemoryManager *MemoryManager::getSingletonPointer()
{
	if (!msManager)
	{
		msMemoryManagerMemory = malloc(sizeof(MemoryManager));
		msManager = new(msMemoryManagerMemory) MemoryManager();
	}

	return msManager;
}

void MemoryManager::setActiveMemoryPool(uint32 memoryPoolIndex)
{
	assert(memoryPoolIndex < mNumOfMemoryPools);
	mActiveMemoryPool = memoryPoolIndex;
}

void MemoryManager::shutDown()
{
	assert(msManager);
	msManager->~MemoryManager();
	free(msMemoryManagerMemory);
	msMemoryManagerMemory = msManager = NULL;
}

MemoryManager::MemoryManager() :
	mMemoryPools(NULL),
	mActiveMemoryPool(0),
	mMaxNumOfMemoryPools(0),
	mNumOfMemoryPools(0)
{
	assert(!msManager);
}

MemoryManager::~MemoryManager()
{
	assert(msManager);
	msManager = NULL;

	// free last pool - others must be freed manually
	// -> developer is required to have an overview of existing memory pools
	if (1 == mNumOfMemoryPools)
		deleteMemoryPool(0);
	else
		assert(0 == mNumOfMemoryPools);

	free(mMemoryPools);
	mMemoryPools = NULL;
}

void MemoryManager::releaseMemory(void *pointer, bool arrayOperator)
{
	if (NULL == pointer)
		return;

	#ifdef CORRECT_DELETE_OPERATOR_AND_BOUNDS_CHECK
		// check whether delete operator is correct
		uint32 *memory1 = reinterpret_cast<uint32 *>(pointer) - 2;
		assert(memory1[0] == (arrayOperator ? 2 : 1));

		// get actual memory block length
		size_t *memory0 = reinterpret_cast<size_t *>(memory1) - 1;
		size_t capacity = *memory0;

		// check whether memory boundary guards are untouched
		uint8 *memoryEnd	= reinterpret_cast<uint8 *>(memory0) + capacity;
		uint32 startGuard	= memory1[1];
		uint32 endGuard		= reinterpret_cast<uint32 *>(memoryEnd)[-1];

		checkGuard(startGuard);
		checkGuard(endGuard);

		getActiveMemoryPool().releaseMemory(memory0);

	#else
		getActiveMemoryPool().releaseMemory(pointer);

	#endif // CORRECT_DELETE_OPERATOR_AND_BOUNDS_CHECK
}

#ifdef CORRECT_DELETE_OPERATOR_AND_BOUNDS_CHECK
	void MemoryManager::checkGuard(uint32 guard)
	{
		uint8 *memory = reinterpret_cast<uint8 *>(&guard);

		for (uint32 j = 0; j < sizeof(guard); ++j)
			assert(GUARD_PATTERN == memory[j]);
	}
#endif // CORRECT_DELETE_OPERATOR_AND_BOUNDS_CHECK

void *MemoryManager::requestMemory(size_t capacity, bool arrayOperator)
{
	#ifdef CORRECT_DELETE_OPERATOR_AND_BOUNDS_CHECK
		// extra space for memory length, boundary guards and operator identifier
		capacity += sizeof(size_t) + sizeof(uint32) + 2 * sizeof(uint32);

		void *memory = getActiveMemoryPool().requestMemory(capacity);

		// set actual memory block length
		size_t *memory0 = reinterpret_cast<size_t *>(memory);
		memory0[0] = capacity;

		// set which operator should be used to free this block
		uint32 *memory1 = reinterpret_cast<uint32 *>(memory0 + 1);
		memory1[0] = (arrayOperator ? 2 : 1);

		// create boundary guards around provided memory block
		void *startPos	= memory1 + 1;
		void *endPos	= reinterpret_cast<uint8 *>(memory) + (capacity - sizeof(uint32));
		memset(startPos, GUARD_PATTERN, sizeof(uint32));
		memset(endPos, GUARD_PATTERN, sizeof(uint32));

		return memory1 + 2;

	#else
		return getActiveMemoryPool().requestMemory(capacity);

	#endif // CORRECT_DELETE_OPERATOR_AND_BOUNDS_CHECK
}

MemoryPool &MemoryManager::getActiveMemoryPool()
{
	// lazy initialization
	if (0 == mNumOfMemoryPools)
		addMemoryPool(DEFAULT_POOL_BUCKET_CAPACITIES, DEFAULT_POOL_BUCKET_GRANULARITIES, DEFAULT_POOL_BUCKET_NUMBER);
	assert(mNumOfMemoryPools > mActiveMemoryPool);
	return *mMemoryPools[mActiveMemoryPool];
}

#endif // MEMORY_MANAGEMENT
