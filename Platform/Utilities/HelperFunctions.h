/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _UTILITIES_HELPER_FUNCTIONS_H_
#define _UTILITIES_HELPER_FUNCTIONS_H_

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

	/** Goes through a provided list of strings called elements and searches for a string called element and returns its index if found.
	@param strings The function searches an the string called element in this ordered set of strings.
	@param elementCount Set this to the number strings in the array list.
	@return Returns the index (w.r.t. list) of the element if found. If element is not in list then -1 is returned. */
	uint32 find(const char * const *strings, const uint32 elementCount, const std::string &element);

	/** Checks whether a provided string called container contains any element of a list of substrings called elements and returns the index of the first found element.
	@param endCharIdx Set this to the address of an uint32 variable to get the index of the first character after the first found element. Is set to -1 if no element is found.
		Set this to NULL to ignore it.
	@param container The function searches an element of elements in container.
	@param elements Each string in elements is considered and tried as substring of container.
	@param elementCount Set this to the number strings in the array elements.
	@return Returns the index (w.r.t. elements) of the first found elements string in container or -1 if no element is contained  */
	uint32 findFirstOf(uint32 *endCharIdx, const std::string &container, const char * const *elements, const uint32 elementCount);
	
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
	inline void swap(T &v1, T &v2)
	{
		T temp = v1;
		v1 = v2;
		v2 = temp;
	}
}

#endif // _UTILITIES_HELPER_FUNCTIONS_H_
