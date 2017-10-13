/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _RESOURCE_MANAGEMENT_MAGIC_CONSTANTS_H_
#define _RESOURCE_MANAGEMENT_MAGIC_CONSTANTS_H_
#ifdef MEMORY_MANAGEMENT

#include "Platform/DataTypes.h"

namespace ResourceManagement
{
	/** Memory guards (small memory areas) are directly in front and after requested memory blocks and filled with this pattern.
		They are used to find unwanted accesses of the surrounding memory areas of the requested block caused by semantic errors such as wrong loops. */
	const uint8 GUARD_PATTERN = 0xad;

	// define the bucket properties of MemoryManager's default memory pool
	// it is created if memory is requested although there is no existing memory pool
	/** This list stores the numbers of memory chunks the buckets of the default pool contain. */
	extern const uint16 DEFAULT_POOL_BUCKET_CAPACITIES[];

	/** This list stores the sizes of the memory chunks the buckts of the default pool contain. */
	extern const uint16 DEFAULT_POOL_BUCKET_GRANULARITIES[];

	/** Defines how many buckets are managed by the default memory pool. */
	extern const uint32 DEFAULT_POOL_BUCKET_NUMBER;
}

#endif // MEMORY_MANAGEMENT
#endif // _RESOURCE_MANAGEMENT_MAGIC_CONSTANTS_H_
