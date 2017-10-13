/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "Platform/DataTypes.h"

/// Contains small features that can be useful for other code, e.g. insertion sort.
namespace Utilities
{
	/** Implements insertion sort.
	@param values The elements in values are unordered, they are not changed by the algorithm.
	@param orderedValues Ordered values is filled with the elements of values which are orderd according to the operator <.
	@param arraySize This is the count of the values to be sorted. */
	template <class T>
	void insertionSort(T *values, T *orderedValues, uint32 arraySize)
	{
		if (0 == arraySize)
			return;

		orderedValues[0] = values[0];
		for(uint32 i = 1; i < arraySize; ++i)	// insertion sort
		{
			for(uint32 j = 0; j < i; ++j)
			{
				if (values[i] < orderedValues[j])
				{
					for(uint32 k = i; k > j; --k)
						orderedValues[k] = orderedValues[k - 1];
					orderedValues[j] = values[i];
					break;
				}

				if (j == i - 1)
					orderedValues[i] = values[i];
			}
		}
	}
}

#endif // _UTILITIES_H_