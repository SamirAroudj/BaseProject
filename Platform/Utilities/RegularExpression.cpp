/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _REGEX_
	#include <cctype>
#endif // _REGEX_

#include <regex>
#include "Platform/Utilities/RegularExpression.h"

using namespace std;
using namespace Math;
using namespace Utilities;

// Implement RegularExpression stuff only if REGEX is truly supported by the build system and cause compiler errors otherwise to avoid suprises during runtime
#ifdef _REGEX_
	// help variables to define regular expressions
	#define D_SIGN "(?:\\+|-)"
	#define D_INT D_SIGN "?\\d+"

	#define D_FIXED_POINT D_INT "(?:\\.\\d*)?|" D_SIGN "?\\.\\d+"
	#define D_REAL "(?:" D_FIXED_POINT ")(?:(?:e|E)" D_SIGN "?\\d+)?"

	#define D_REAL_PAIR "(" D_REAL ")(?: |,|, |;|; |:|\\|)(" D_REAL ")"
	//#define D_VECTOR2 D_REAL_PAIR "|\\(" D_REAL_PAIR "\\)"

	// Initialize static member variables
	const char * const RegularExpression::INTEGER = D_INT;
	const char * const RegularExpression::FIXED_POINT = D_FIXED_POINT;
	const char * const RegularExpression::REAL = D_REAL;
	const char * const RegularExpression::REAL_PAIR = D_REAL_PAIR;
	//const char * const RegularExpression::VECTOR2 = D_VECTOR2;


	uint32 RegularExpression::getRealPair(Real &r1, Real &r2, const char *text, bool matchAtStart, int32 length)
	{
		// variables for matching
		const char	*regExpText = (matchAtStart ? "^" D_REAL_PAIR : D_REAL_PAIR);
		regex		realPair(regExpText);
		cmatch		matches;
		bool		success;

		// search / match
		if (length > 0)
			success = regex_search(text, text + length, matches, realPair);
		else
			success = regex_search(text, matches, realPair);

		if (!success)
			return 0;

		// convert 2 sub matches to Real data types
		string n0 = matches.str(1);
		string n1 = matches.str(2);
		r1 = Converter::to<float>(n0);
		r2 = Converter::to<float>(n1);

		return (uint32) (matches.prefix().length() + matches.length(0));
	}

#else	// # _REGEX_ is not defined - there is no std support
		// todo: this is a dirty & quick solution - wait for gcc 4.9 until only std::regex can be used

	bool isFloatStartChar(char c)
	{
		return ('+' == c || '-' == c || isdigit(c) || '.' == c);
	}

	uint32 getReal(Real &r, const char *text, bool matchAtStart, int32 length)
	{
		// todo: this is a dirty & quick solution - wait for gcc 4.9 until only std::regex can be used
		if (0 == length)
			length = strlen(text);

		// find float value start
		uint32 start = 0;
		if (!matchAtStart)
			for (char c = text[start]; !isFloatStartChar(c) && start < length; c = text[start])
				++start;

		if (!isFloatStartChar(text[start]))
			return 0;

		// get real value starting at text[start]
		float temp;
		if (1 != sscanf(text + start, "%f", &temp))
			return 0;
		r = temp;

		uint32 end = start + 1;

		for (char c = text[end];
			 isdigit(c) || '.' == c || 'e' == tolower(c) ||
			 (('+' == c || '-' == c) && tolower(text[end - 1]) == 'e');
			 c = text[end])
		{
			++end;
		}

		return (end < length ? end : 0);
	}

	uint32 isSeperator(char c)
	{
		return c == ',' || c == ';' ||  c == ':' ||  c == '|' ||  c == ' ';
	}

	uint32 RegularExpression::getRealPair(Real &r0, Real &r1, const char *text, bool matchAtStart, int32 maxLength)
	{
		// first real part
		uint32 length = getReal(r0, text, matchAtStart, maxLength);
		if (0 == length)
			return 0;

		// seperator part
		bool valid = isSeperator(text[length]);
		if (' ' == text[length] && isSeperator(text[length + 1]))
		{
			++length;
			valid = true;
		}
		++length;
		if (!valid)
			return 0;

		// second real part
		uint32 lengthR1 = getReal(r1, text + length, true, maxLength - length);
		if (0 == lengthR1)
			return 0;
		length += lengthR1;

		return length;
	}

#endif // _REGEX_

uint32 RegularExpression::getRealPairs(vector<Vector2> &pairs, const char *text, int32 length)
{
	bool	noLength		= (length < 1);
	uint32	readCharsTotal	= 0;
	Real	r1;
	Real	r2;

	// get pairs
	while (true)
	{
		// try to get a single pair
		const char	*current	= text + readCharsTotal;
		int32		readChars	= getRealPair(r1, r2, current, false, (noLength ? 0 : length));
		if (0 == readChars)
			break;

		// process found pair
		length			-= readChars;
		readCharsTotal	+= readChars;
		pairs.push_back(Vector2(r1, r2));
	}

	return readCharsTotal;
}
