/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifdef _WINDOWS
	#include <Windows.h>
#endif // _WINDOWS

#include <cstring>
#include <iterator>
#include <sstream>
#include "Platform/Utilities/HelperFunctions.h"

using namespace std;
using namespace Utilities;

bool Utilities::containsAny(const string &container, const char * const *elements, const uint32 elementCount)
{
	for (uint32 i = 0; i < elementCount; ++i)
		if (string::npos != container.find(elements[i]))
			return true;

	return false;
}

uint32 Utilities::find(const char * const *list, const uint32 elementCount, const std::string &element)
{
	for (uint32 i = 0; i < elementCount; ++i)
		if (element == list[i])
			return i;
	return (uint32) -1;
}

uint32 Utilities::findFirstOf(uint32 *endCharIdx, const string &container, const char * const *elements, const uint32 elementCount)
{
	// test each element
	for (uint32 i = 0; i < elementCount; ++i)
	{
		const char	*element	= elements[i];
		size_t		idx			= container.find(element);
		if (string::npos == idx)
			continue;

		if (endCharIdx)
			*endCharIdx = (uint32) (idx + strlen(element));
		return i;
	}

	// no element was found
	if (endCharIdx)
		*endCharIdx = (uint32) -1;
	return (uint32) -1;
}

#ifdef _WINDOWS
	void Utilities::getCommandLineArguments(std::vector<std::string> &arguments, const char *unformattedArguments)
	{
		arguments.clear();

		// convert command line arguments if necessary
		istringstream iss(unformattedArguments);
		arguments = { istream_iterator<string>(iss), istream_iterator<string>() };
	}
#endif // _WINDOWS

void Utilities::getCommandLineArguments(std::vector<std::string> &arguments,
		const char **unformattedArguments, const int32 argumentCount)
{
	arguments.clear();
	for (int32 i = 1; i < argumentCount; ++i)
		arguments.push_back((const char *) unformattedArguments[i]);
}

bool Utilities::hasEnding(const string &testString, const string &ending)
{
	const size_t testStringLength = testString.length();
	const size_t endingLength = ending.length();
	
	// not possible?
	if (testStringLength < endingLength)
		return false;
	
	// correct ending?
	const size_t endingStart = testStringLength - endingLength;
	return (0 == testString.compare(endingStart, endingLength, ending));
}

void Utilities::split(vector<string> &parts, const string &text, const char *delimeters)
{
	const size_t textLength	= text.length();
		  uint32 partCount	= 0;
	parts.clear();

	for (size_t processedCount = 0; processedCount < textLength; ++processedCount)
	{
		size_t pos = text.find_first_of(delimeters, processedCount);
		if (string::npos == pos)
			pos = textLength;

		size_t length = pos - processedCount;
		if (0 == length)
			continue;

		// extract substring
		parts.resize(partCount + 1);
		parts[partCount] = text.substr(processedCount, length);
		processedCount	+= length;
		++partCount;
	}
}
