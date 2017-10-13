/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_
#ifdef MEMORY_MANAGEMENT

#include <cassert>
#include <new>
#include "Platform/ResourceManagement/MagicConstants.h"
#include "Platform/ResourceManagement/MemoryPool.h"

// globally overloaded new and delete operators for own memory management
#ifdef _WINDOWS
	void operator delete(void *pointer);
	void operator delete[](void *pointer);
#else
	void operator delete(void *pointer) noexcept;
	void operator delete[](void *pointer) noexcept;
#endif // _WINDOWS

void *operator new(size_t capacity);
void *operator new(std::size_t capacity, const std::nothrow_t& nothrow_value);
void *operator new [](size_t capacity);
void *operator new [](std::size_t capacity, const std::nothrow_t& nothrow_value);

/// Contains code to overload new and delete operators as well as base classes for Resources.
namespace ResourceManagement
{
	class MemoryManager
	{
	#ifdef _WINDOWS
		friend void ::operator delete(void *pointer);
		friend void ::operator delete[](void *pointer);
	#else
		friend void ::operator delete(void *pointer) noexcept;
		friend void ::operator delete[](void *pointer) noexcept;
	#endif // _WINDOWS

	friend void *::operator new(size_t capacity);
	friend void *::operator new(std::size_t capacity, const std::nothrow_t& nothrow_value);
	friend void *::operator new [](size_t capacity);
	friend void *::operator new [](std::size_t capacity, const std::nothrow_t& nothrow_value);

	public:
		/** Creates a new memory pool.
		 The default memory pool is created if memory is requested and if there is no pool.
		 There is no pool responsible for the first memory pool which is created. */
		void addMemoryPool(const uint16 *bucketCapacities, const uint16 *bucketGranularities, uint32 numOfBuckets);

		/** Deletes a pool which must have released all of its requested memory first.
		 Make sure that the pool is active which is responsible for the pool which is going to be deleted.
		 Relative order of the other pools is preserved that is if there are the pools 0, 1, 2, 3, 4 and pool 2 is deleted then 0->0, 1->1, 3->2, 4->3*/
		void deleteMemoryPool(uint32 memoryPoolIndex);

		/** Provides access to memory management functionality
		@return Returns a reference to the one and only MemoryManager object. */
		static MemoryManager &getSingleton();

		/** Provides access to memory management functionality
		@return Returns a pointer to the one and only MemoryManager object. */
		static MemoryManager *getSingletonPointer();

		/** Use a pool specified by an index for further memory allocations and deallocations.
		Pools are indexed according to their order of creation. See deleteMemoryPool comment to know how pool deletion changes pool indices. 
		@param memoryPoolIndex The pool currently identified by memoryPoolIndex becomes active and will be responsible for delete and new calls.*/
		void setActiveMemoryPool(uint32 memoryPoolIndex);

		/** Must be called at the end of a program to free all remainng memory.
			Calls the destructor of the Memorymanager. There must not be more than one MemoryPool left. */
		static void shutDown();

	private:
		/** Creates a memory manager. There can only be one MemoryManager object at once. */
		MemoryManager();

		/** Destroys the only MemoryManager object. There must not be more than one MemoryPool object left. */
		~MemoryManager();


		#ifdef CORRECT_DELETE_OPERATOR_AND_BOUNDS_CHECK
			/** Checks whether the entered guard is filled with the pattern GUARD_PATTERN, see ResourceManagement::GUARD_PATTERN.
				An assertion fails if the guard contains a byte with another value than GUARD_PATTERN.
			@param guard Is checked to contain only bytes matching GUARD_PATTERN, see ResourceManagement::GUARD_PATTERN. */
			void checkGuard(uint32 guard);
		#endif // CORRECT_DELETE_OPERATOR_AND_BOUNDS_CHECK

		/** Acess the currently active MemoryPool object set by setActiveMemoryPool.
		@return The returned pool is responsible for current new and delete calls. */
		MemoryPool &getActiveMemoryPool();

		/** Releases a memory block. It is used by delete operators.
			Checks for correct operator usage ("" or "[]") and accidental boundary accesses if preprocessor flag CORRECT_DELETE_OPERATOR_AND_BOUNDS_CHECK is set.
		@param pointer Set this to the pointer referring to a usable memory block.
			Should have been returned by a call of ResourceManagement::requestMemory whereas the same MemoryPool must be active.
		@param arrayOperator Set this to the same value of arrayOperator which was used for ResourceManagement::requestMemory to retrieve the memory to be released.
			(delete [] must be used for new [] and delete for new.) */
		void releaseMemory(void *pointer, bool arrayOperator);

		/** Requests a memory block. It is used by new operators.
			Also adds data to the memory block to check for correct operator usage ("" or "[]") and
			accidental boundary accesses if preprocessor flag CORRECT_DELETE_OPERATOR_AND_BOUNDS_CHECK is set.
		@param capacity Set this to the number of wanted bytes.
			This should be set to the value of the capacity parameter of the new operator.
		@param arrayOperator Set this to true for the array new operator (new []) and false for the simple new operator (new without []).
			(delete [] must be used for new [] and delete for new later.)
		@return The returned pointer refers to a usable memory block of capacity bytes length.*/
		void *requestMemory(size_t capacity, bool arrayOperator);

    private:
        static MemoryManager *msManager;                /// Pointer to the one and only memory manager object.
        static void          *msMemoryManagerMemory;    /// This is the memory where the MemoryManager is placed.

		MemoryPool	**mMemoryPools;			/// array of all dynamically created MemoryPool objects
		uint32		mActiveMemoryPool;		/// index of the currently activie / responsible MemoryPool object in mMemoryPools
		uint32		mMaxNumOfMemoryPools;	/// size of the array mMemoryPools
		uint32		mNumOfMemoryPools;		/// actual number of exisiting pools in mMemoryPools
	};
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline ResourceManagement::MemoryManager &ResourceManagement::MemoryManager::getSingleton()
{
	return *getSingletonPointer();
}

#endif // MEMORY_MANAGEMENT
#endif // _MEMORY_MANAGER_H_
