/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _UTILITIES_ARRAY_H_
#define _UTILITIES_ARRAY_H_

#include <vector>

namespace Utilities
{
	template <class T>
	class Array
	{
	public:
		/** Searches for an element in a container, deletes its first occurance and swaps it with the last element in the container. (O(n) for linear search and O(1) for deletion)
			Does not preserve the order of the elements.
		@param container The container is searched for the first occurance of element which is replaced by the last container element if the search is successful.
		@param element The first occurance of element is searched and replaced by the last container element.
		@param startIdx Defines where to start the search. (container[startIdx] is the first element to be checked.)
		@return Returns the index of the deleted / replaced container element or ((size_t) -1) if element was not found in container. */
		static size_t deleteFirstBySwapWithBack(std::vector<T> &container, const T &element, const size_t startIdx = 0);
		
		/** Compacts (filters) the vector according to offsets.
		@param buffer This container is filtered / compacted according to offsets. Each element with offsets[i] != offsets[i + 1] is deleted / filtered.
		@param offsets Must have buffer.size() + 1 entries whereas each offset[i] defines how many elements should be filtered / skipped before element i.
			This means targetBuffer[i - offset[i]] = sourceBuffer[i] if offsets[i] == offsets[i + 1]. */
		static void compaction(std::vector<T> &buffer, const uint32 *offsets);
		
		/** Compacts (filters) an array according to offsets.
		@param targetBuffer This buffer is filled with the compacted (filtered) data.
			Must have space for sourceCount - offsets[sourceCount] elements.
		@param sourceBuffer Elements from this buffer are copied into target buffer whereas some elements are skipped for compaction / filtering.
			Must contain sourceCount elements.
		@param offsets Must have sourceCount + 1 entries whereas each offset[i] defines how many elements should be filtered / skipped before element i.
			This means targetBuffer[i - offset[i]] = sourceBuffer[i] if offsets[i] == offsets[i + 1]. 
		@param sourceCount Set this to the number of elements in sourceBuffer. There must also be sourceCount + 1 offsets in offsets and
			space for n = sourceCount - offsets[sourceCount] elements in targetBuffer. */
		static void compaction(T *targetbuffer, const T *sourceBuffer,
			const uint32 *offsets, const uint32 sourceCount);
		
		/** Compacts (filters) an array in a blockwise manner according to offsets.
		@param elements This buffer is compacted / filled blockwise according to offsets.
			Must have k * elementsPerBlock with k \in N elements.
			All blocks of elementsPerBlock elements with offsets[blockIdx] != offsets[blockIdx + 1] are filtered / deleted.
		@param offsets Must have (elements.size() / elesPerBlock) + 1 entries whereas each offset[i] defines how many blocks (elementsPerBlock elements) should be filtered / skipped before block i.
			This means target buffer block (i - offset[i]) is set to the source buffer block i if offsets[i] == offsets[i + 1] (= block i is kept). 
		@param elementsPerBlock Defines how many elements are in a block.
			This allows compaction / filtering in a blockwise manner.
			Each block of elementsPerBlock elements is treated as a single unit which might be filtered or not.*/
		static void compaction(std::vector<T> &eles, const uint32 *offsets, const uint32 elementsPerBlock);

		/** Compacts (filters) an array in a blockwise manner according to offsets.
		@param targetBuffer This buffer is filled with the compacted (filtered) data blocks.
			Must have space for n elements with n = elementsPerBlock * (sourceBlockCount - offsets[sourceBlockCount]).
		@param sourceBuffer Elements from this buffer are copied blockwise into target buffer whereas some blocks of elements are skipped for compaction / filtering.
			Must contain m elements with m = sourceBlockCount * elementsPerBlock.
		@param offsets Must have sourceBlockCount + 1 entries whereas each offset[i] defines how many blocks (elementsPerBlock elements) should be filtered / skipped before block i.
			This means target buffer block (i - offset[i]) is set to the source buffer block i if offsets[i] == offsets[i + 1]. 
		@param sourceBlockCount Set this to the number of blocks in sourceBuffer. There must also be sourceBlockCount + 1 offsets in offsets and
			space for n = elementsPerBlock * (sourceBlockCount - offsets[sourceBlockCount]) elements in targetBuffer.
		@param elementsPerBlock Defines how many elements are in a block.
			This allows to do compaction / filtering blockwise.
			Each block of elementsPerBlock elements is treated as a single unit which might be filtered or not.*/
		static void compaction(T *targetbuffer, const T *sourceBuffer,
			const uint32 *offsets, const uint32 sourceBlockCount, const uint32 elementsPerBlock);

		/** Finds the minimum and maximum value in an unordered array of values.
		@param minimum Is set to the smallest element in elements or max<T> if elementCount is zero.
		@param maximum Is set to the largest element in elements or min<T> if elementCount is zero.
		@param elements Set this to the unordered array of elements you want to search through.
		@param elementCount Set this to the number of elements in elements you want to search through. */
		static void findExtrema(T &minimum, T &maximum, const T *elements, const size_t elementCount);

		/** Finds the maximum value in an unordered array of values.
		@param maximum Is set to the largest element in elements or min<T> if elementCount is zero.
		@param elements Set this to the unordered array of elements you want to search through.
		@param elementCount Set this to the number of elements in elements you want to search through. */
		static void findMaximum(T &maximum, const T *elements, const size_t elementCount);

		/** Finds the minimum value in an unordered array of values.
		@param minimum Is set to the smallest element in elements or max<T> if elementCount is zero.
		@param elements Set this to the unordered array of elements you want to search through.
		@param elementCount Set this to the number of elements in elements you want to search through. */
		static void findMinimum(T &minimum, const T *elements, const size_t elementCount);

		/** Sorts the elements and removes dupblicates.
		@elements Upon return elements are sorted and each element \in elments is unique.*/
		static void removeDuplicates(std::vector<T> &elements);
		
		/** Reorders the elements in elements according to targetIndices.
		@param elements These values are reordered like elements[targetIndices[i]] = elements[i]. Must have space for elementCount elements.
		@param targetIndices Must have elementCount entries which define where to place each element, that is targetIndices must be a permutation of { 0, 1, ..., elementCount - 1 }.
			Each ordering index targetIndices[i] must also be smaller than elementCount.
			Elements are reordered like elements[targetIndices[i]] = elements[i].
		@param elementCount Defines the number of element in elements and ordering indices in targetIndices.*/
		static void reorder(std::vector<T> &elements, const uint32 *targetIndices);

		/** Copies the elements from sourceElements to targetElements and changes the order according to targetIndices.
		@param targetElements These values are reordered like targetElements[targetIndices[i]] = sourceElements[i]. Must have space for elementCount elements.
		@param sourceElements These elements are copied to targetElements according to targetIndices. Must have elementCount elements.
		@param targetIndices Must have elementCount entries which define where to place each element, that is targetIndices must be a permutation of { 0, 1, ..., elementCount - 1 }.
			Each ordering index targetIndices[i] must also be smaller than elementCount.
			Elements are reordered like targetElementslements[targetIndices[i]] = sourceElements[i].
		@param elementCount Defines the number of elements in targetElements, sourceElements and the number of ordering indices in targetIndices.*/
		static void reorder(T *targetElements, const T *sourceElements, const uint32 *targetIndices, const uint32 &elementCount);
		
		/** Copies the elements from sourceElements to targetElements in a blockwise manner and changes the order according to targetIndices.
			E.g., 4 elements from block 2 starting with element 8 are copied to target block 1 starting with element 4.
		@param elements The elements in elements are reordered blockwise like target block targetIndices[i] is set to source block i for all source blocks i \in i < blockCount.
			Must have k * elemnetsPerBlock elements with k \in N.
		@param targetIndices These are block indices that define where each source block is put in targetElements.
			Must have blockCount entries which define where to place each block.
			That is targetIndices must be a permutation of { 0, 1, ..., blockCount - 1 }.
			Each ordering block index targetIndices[i] must also be smaller than elementCount.
			Elements are reordered like target block targetIndices[i] = source block sourceElements[i].
		@param elementsPerBlock Defines the size of each block. */
		static void reorder(std::vector<T> &elements, const uint32 *targetIndices, const uint32 &elementsPerBlock);

		/** Copies the elements from sourceElements to targetElements in a blockwise manner and changes the order according to targetIndices.
			E.g., 4 elements from block 2 starting with element 8 are copied to target block 1 starting with element 4.
		@param targetElements These values are reordered blockwise like target block targetIndices[i] = source block i.
			Must have space for elementsPerBlock * blockCount elements.
		@param sourceElements These elements are copied blockwise to targetElements according to targetIndices.
			Must have elementsPerBlock * blockCount elements.
		@param targetIndices These are block indices that define where each source block is put in targetElements.
			Must have blockCount entries which define where to place each block.
			That is targetIndices must be a permutation of { 0, 1, ..., blockCount - 1 }.
			Each ordering block index targetIndices[i] must also be smaller than elementCount.
			Elements are reordered like target block targetIndices[i] = source block sourceElements[i].
		@param blockCount Defines the number of block ordering indices in targetIndices.
		@param elementsPerBlock Defines the size of each block. */
		static void reorder(T *targetElements, const T *sourceElements, const uint32 *targetIndices, const uint32 &blockCount, const uint32 &elementsPerBlock);
	};
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	size_t Array<T>::deleteFirstBySwapWithBack(std::vector<T> &container, const T &element, const size_t startIdx)
	{
		const size_t size = container.size();

		for (size_t i = startIdx; i < size; ++i)
		{
			if (element != container[i])
				continue;

			container[i] = container.back();
			container.pop_back();
			return i;
		}

		return (size_t) -1;
	}

	template <class T>
	void Array<T>::compaction(std::vector<T> &buffer, const uint32 *offsets)
	{
		const uint32 oldCount = (uint32) buffer.size();
		const uint32 newCount = oldCount - offsets[oldCount];
		
		std::vector<T> newBuffer(newCount);
		compaction(newBuffer.data(), buffer.data(), offsets, oldCount);
		buffer.swap(newBuffer);
	}

	template <class T>
	void Array<T>::compaction(T *targetBuffer, const T *sourceBuffer, const uint32 *offsets, const uint32 sourceCount)
	{	
		#pragma omp parallel for
		for (int64 i = 0; i < sourceCount; ++i)
		{
			// discard this one?
			const uint32 oldIdx = (uint32) i;
			if (offsets[oldIdx] != offsets[oldIdx + 1])
				continue;

			// keep & copy it
			const uint32 newIdx = oldIdx - offsets[oldIdx];
			targetBuffer[newIdx] = sourceBuffer[oldIdx];
		}
	}

	template <class T>
	void Array<T>::compaction(std::vector<T> &eles, const uint32 *offsets, const uint32 elesPerBlock)
	{
		const uint32 oldCount = (uint32) eles.size();
		const uint32 sourceBlockCount = oldCount / elesPerBlock;
		const uint32 newCount = oldCount - elesPerBlock * offsets[sourceBlockCount];
		std::vector<T> newEles(newCount);

		compaction(newEles.data(), eles.data(), offsets, sourceBlockCount, elesPerBlock);
		newEles.swap(eles);
	}

	template <class T>
	void Array<T>::compaction(T *targetBuffer, const T *sourceBuffer, const uint32 *offsets, const uint32 sourceBlockCount, const uint32 elementsPerBlock)
	{	
		#pragma omp parallel for
		for (int64 i = 0; i < sourceBlockCount; ++i)
		{
			// discard this one?
			const uint32 oldIdx = (uint32) i;
			if (offsets[oldIdx] != offsets[oldIdx + 1])
				continue;

			// copy  data for a complete memory block of elementsPerBlock elements
			const uint32 newIdx = oldIdx - offsets[oldIdx];
			const T *const sourceStart = sourceBuffer + oldIdx * elementsPerBlock;
			T *const targetStart = targetBuffer + newIdx * elementsPerBlock;

			for (uint32 relativeIdx = 0; relativeIdx < elementsPerBlock; ++relativeIdx)
				targetStart[relativeIdx] = sourceStart[relativeIdx];
		}
	}

	template <class T>
	void Array<T>::findExtrema(T &minimum, T &maximum, const T *elements, const size_t elementCount)
	{
		minimum = (std::numeric_limits<T>::max)();
		maximum = std::numeric_limits<T>::lowest();

		for (size_t i = 0; i < elementCount; ++i)
		{
			if (elements[i] < minimum)
				minimum = elements[i];
			if (elements[i] > maximum)
				maximum = elements[i];
		}
	}
	
	template <class T>
	void Array<T>::findMaximum(T &maximum, const T *elements, const size_t elementCount)
	{
		maximum = std::numeric_limits<T>::lowest();

		for (size_t i = 0; i < elementCount; ++i)
			if (elements[i] > maximum)
				maximum = elements[i];
	}

	template <class T>
	void Array<T>::findMinimum(T &minimum, const T *elements, const size_t elementCount)
	{
		minimum = (std::numeric_limits<T>::max)();

		for (size_t i = 0; i < elementCount; ++i)
			if (elements[i] < minimum)
				minimum = elements[i];
	}

	template <class T>
	void Array<T>::removeDuplicates(std::vector<T> &elements)
	{
		// no work?
		if (elements.empty())
			return;

		// sort elements & remove adjacent dupblicates
		std::sort(elements.begin(), elements.end());
		std::vector<uint32>::iterator uniqueEnd = std::unique(elements.begin(), elements.end());
		elements.resize(std::distance(elements.begin(), uniqueEnd));
	}

	template <class T>
	void Array<T>::reorder(std::vector<T> &elements, const uint32 *targetIndices)
	{
		const uint32 eleCount = (uint32) elements.size();
		std::vector<T> newElements(eleCount);

		reorder(newElements.data(), elements.data(), targetIndices, eleCount);
		newElements.swap(elements);
	}

	template <class T>
	void Array<T>::reorder(T *targetEles, const T *sourceEles, const uint32 *targetIndices, const uint32 &eleCount)
	{
		#pragma omp parallel for
		for (int64 sourceEleIdx = 0; sourceEleIdx < eleCount; ++sourceEleIdx)
			targetEles[targetIndices[sourceEleIdx]] = sourceEles[sourceEleIdx];
	}

	template <class T>
	void Array<T>::reorder(std::vector<T> &elements, const uint32 *targetIndices, const uint32 &elesPerBlock)
	{
		const uint32 eleCount = (uint32) elements.size();
		const uint32 blockCount = eleCount / elesPerBlock;
		std::vector<T> newElements(eleCount);

		reorder(newElements.data(), elements.data(), targetIndices, blockCount, elesPerBlock);
		newElements.swap(elements);
	}

	template <class T>
	void Array<T>::reorder(T *targetElements, const T *sourceElements, const uint32 *targetIndices, const uint32 &blockCount, const uint32 &elesPerBlock)
	{
		#pragma omp parallel for
		for (int64 blockIdx = 0; blockIdx < blockCount; ++blockIdx)
		{
			// start pointers of source & target blocks
			const T *startSource = sourceElements + elesPerBlock * blockIdx;
			T *startTarget = targetElements + elesPerBlock * targetIndices[blockIdx];

			// copy block
			for (uint32 relativeIdx = 0; relativeIdx < elesPerBlock; ++relativeIdx)
				startTarget[relativeIdx] = startSource[relativeIdx];
		}
	}
}

#endif // _UTILITIES_ARRAY_H_