/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _BUCKET_H_
#define _BUCKET_H_

#include "Platform/DataTypes.h"
namespace ResourceManagement
{
    /// A Bucket object contains and manages memory chunks of equal size in a contiguous space of memory.
	class Bucket
	{
	public:
        /** Initializes chunk management.
        @param granularity Defines the sizes of each chunk in bytes.
        @param capacity Defines how many chunks are managed by this bucket. */
		Bucket(uint16 granularity, uint16 capacity);

        /** Tests whether requested memory was properly used and relesed.
            Does active memory destruction if the preprocessor flag is set. */
		~Bucket();

        /** Returns the number of chunks managed by this Bucket object.
        @return Returns the number of chunks managed by this Bucket object. */
		const uint16 getCapacity() const { return mCapacity; }

        /** Queries the chunk size of a pointer and tests whether it is managed by this Bucket object.
        @param pointer Set this to a pointer to test whether it is managed by this bucket and get its asociated memory size.
        @return Returns 0 if pointer is not managed by this bucket. Otherwise the chunk size / granularity of this bucket is returned. */
        const uint32 getChunkSize(void *pointer) const;

        /** Returns the size of each chunk in bytes.
        @return All chunks managed by this bucket are equally sized. Their size in bytes is returned. */
		const uint16 getGranularity() const { return mGranularity; }

        /** Queries whether all chunks of this Bucket object were requested and are currently in use.
        @return Returns true if there is no free memory chunk left. Return false if there are free chunks that can be used. */
		bool isFull() const { return mNumOfFreeChunks == 0; }

        /** Serves a memory request. The bucket must not be full and size must be smaller equal than bucket's granularity.
        @param size Size must not be greater than the granularity / chunk size of this bucket. (unit: bytes)
        @return Returns a pointer to a free memory chunk to serve the request or
                NULL if the bucket is full or if size is larger than the granularity of this Bucket object.*/
        void *requestMemory(size_t size);

        /** Returns true if pointer is successfully freed as it points to a chunk managed by this bucket.
            Returns false if the memory piece to be freed is not owned by this Bucket object.
        @param pointer Set this only to a pointer referring to a memory chunk managed by this Bucket object.
            This referred chunk is freed and can be used for another call to request().
            If pointer is not owned by this object then its associated memory cannot be freed.
        @return Returns true if pointer is successfully freed as it points to a chunk managed by this bucket.
            Returns false if the memory piece to be freed is not owned by this Bucket object. */
		bool releaseMemory(void *pointer);

	private:
		unsigned char	*mBasePointer;		/// contains the memory chunks which are requested by bucket users
		uint16			*mFreeChunks;		/// list containing indexes which determines which memory chunks from mBasePointer are free to use
		const uint16	mCapacity;			/// total number of memory chunks
		const uint16	mGranularity;		/// size of each memory chunk
		uint16			mNumOfFreeChunks;	/// number of free memory chunks left in mFreeChunks
	};
}

#endif // _BUCKET_H_
