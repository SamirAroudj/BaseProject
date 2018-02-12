/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _UTILITIES_HELPER_FUNCTIONS_H_
#define _UTILITIES_HELPER_FUNCTIONS_H_

#include <algorithm>
#include <limits>
#include <vector>
#include "Platform/DataTypes.h"
#include "Platform/Utilities/Conversions.h"

namespace Utilities
{
	/** Checks whether a provided string called container contains any element of a list of substrings called elements.
	@param container The function searches an element of elements in container.
	@param elements Each string in elements is considered and tried as substring of container.
	@param elementCount Set this to the number strings in the array elements.
	@return Returns true if any element of elements is in container. If no element of elements is found in container then false is returned.*/
	bool containsAny(const std::string &container, const char * const *elements, const uint32 elementCount);

	/** Searches for an element in a container, deletes its first occurance and swaps it with the last element in the container. (O(n) for linear search and O(1) for deletion)
		Does not preserve the order of the elements.
	@param container The container is searched for the first occurance of element which is replaced by the last container element if the search is successful.
	@param element The first occurance of element is searched and replaced by the last container element.
	@param startIdx Defines where to start the search. (container[startIdx] is the first element to be checked.)
	@return Returns the index of the deleted / replaced container element or ((size_t) -1) if element was not found in container. */
	template <class T>
	size_t deleteFirstBySwapWithBack(std::vector<T> &container, const T &element, const size_t startIdx = 0);

	/** Goes through a provided list of strings called elements and searches for a string called element and returns its index if found.
	@param list The function searches an the string called element in this ordered set of strings.
	@param elementCount Set this to the number strings in the array list.
	@return Returns the index (w.r.t. list) of the element if found. If element is not in list then -1 is returned. */
	uint32 find(const char * const *list, const uint32 elementCount, const std::string &element);

	/** Finds the minimum and maximum value in an unordered array of values.
	@param minimum Is set to the smallest element in elements or max<T> if elementCount is zero.
	@param maximum Is set to the largest element in elements or min<T> if elementCount is zero.
	@param elements Set this to the unordered array of elements you want to search through.
	@param elementCount Set this to the number of elements in elements you want to search through. */
	template <class T>
	void findExtrema(T &minimum, T &maximum, const T *elements, const size_t elementCount);

	/** Checks whether a provided string called container contains any element of a list of substrings called elements and returns the index of the first found element.
	@param endCharIdx Set this to the address of an uint32 variable to get the index of the first character after the first found element. Is set to -1 if no element is found.
		Set this to NULL to ignore it.
	@param container The function searches an element of elements in container.
	@param elements Each string in elements is considered and tried as substring of container.
	@param elementCount Set this to the number strings in the array elements.
	@return Returns the index (w.r.t. elements) of the first found elements string in container or -1 if no element is contained  */
	uint32 findFirstOf(uint32 *endCharIdx, const std::string &container, const char * const *elements, const uint32 elementCount);

	/** Finds the maximum value in an unordered array of values.
	@param maximum Is set to the largest element in elements or min<T> if elementCount is zero.
	@param elements Set this to the unordered array of elements you want to search through.
	@param elementCount Set this to the number of elements in elements you want to search through. */
	template <class T>
	void findMaximum(T &maximum, const T *elements, const size_t elementCount);

	/** Finds the minimum value in an unordered array of values.
	@param minimum Is set to the smallest element in elements or max<T> if elementCount is zero.
	@param elements Set this to the unordered array of elements you want to search through.
	@param elementCount Set this to the number of elements in elements you want to search through. */
	template <class T>
	void findMinimum(T &minimum, const T *elements, const size_t elementCount);
	
	#ifdef _WINDOWS
		/** Converts the command line arguments and stores them separately in arguments.
		@param arguments Is filled with the command line arguments (more reasonable representation), possibly contains the program name.
		@param unformattedArguments Set this to the unformatted arguments from the c++ main function.*/
		void getCommandLineArguments(std::vector<std::string> &arguments, const char *unformattedArguments);
	#endif // _WINDOWS

	/** Converts the command line arguments and stores them separately in arguments.
	@param arguments Is filled with the command line arguments (more reasonable representation), possibly contains the program name.
	@param unformattedArguments Set this to the unformatted arguments from the c++ main function.
	@param argumentCount Set this to the number of arguments/strings in unformattedArguments. */
	void getCommandLineArguments(std::vector<std::string> &arguments, const char **unformattedArguments, const int32 argumentCount);

	/** Returns true if testString ends exactly with ending.
	@param testString Is searched for containment of ending at its very end.
	@param ending Specifies how testString should end.
	@return Returns only true if testString ends exactly with ending, e.g. testString = "Object.ply", ending = ".ply".*/
	bool hasEnding(const std::string &testString, const std::string &ending);
	
	/** Sorts the elements and removes dupblicates.
	@elements Upon return elements are sorted and each element \in elments is unique.*/
	template <class T>
	void removeDuplicates(std::vector<T> &elements);

	///** Prefix sum with elements[0] = 0. 
	//@param elements The prefix sum of the container elements is computed and stored within elements[0] = 0 and elements[count + 1] = total sum.
	//	!!!!!!!!!!!!!!! Make sure that there is one additional space for the total sum. !!!!!!!!!!!!!!!
	//@param count Set this to the number of elements in container which should have memory for count + 1 values.*/
	//template <class T>
	//void prefixSum(T *elements, const size_t count);

	/** Divides a text into subparts based on entered dividing characters delimeters.
		No subpart will contain any of the characters in delimeters.
		E.g. " test bla|blubb || foo ", " |" -> "test", "bla", "blubb", "foo"
	@param parts Is filled with found subparts which are divided by delimeters.
	@param text This string is divided into subparts.
	@param delimeters Each of the characters in delimeters is used to divide text into subparts.*/
	void split(std::vector<std::string> &parts, const std::string &text, const char *delimeters);

	/** Swaps the content of two variables.
	@param v1 This variable becomes v2.
	@param v2 This variable becomes v1. */
	template <class T>
	inline void swap(T &v1, T &v2);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	size_t deleteFirstBySwapWithBack(std::vector<T> &container, const T &element, const size_t startIdx)
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
	void findExtrema(T &minimum, T &maximum, const T *elements, const size_t elementCount)
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
	void findMaximum(T &maximum, const T *elements, const size_t elementCount)
	{
		maximum = std::numeric_limits<T>::lowest();

		for (size_t i = 0; i < elementCount; ++i)
			if (elements[i] > maximum)
				maximum = elements[i];
	}

	template <class T>
	void findMinimum(T &minimum, const T *elements, const size_t elementCount)
	{
		minimum = (std::numeric_limits<T>::max)();

		for (size_t i = 0; i < elementCount; ++i)
			if (elements[i] < minimum)
				minimum = elements[i];
	}

	template <class T>
	void removeDuplicates(std::vector<T> &elements)
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
	inline void swap(T &v1, T &v2)
	{
		T temp = v1;
		v1 = v2;
		v2 = temp;
	}
}

#endif // _UTILITIES_HELPER_FUNCTIONS_H_
