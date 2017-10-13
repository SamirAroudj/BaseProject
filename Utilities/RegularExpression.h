/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _REGULAR_EXPRESSION_H_
#define _REGULAR_EXPRESSION_H_

#include <vector>
#include "Math/Vector2.h"
#include "Platform/DataTypes.h"

// Is stuff of <regex> properly implemented and supported?
#ifdef __GNUC__
	#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9)
		#define _REGEX_
	#endif // GCC is at least 4.9.x and supports <regex>
#elif _MSC_VER
	#define _REGEX_
#endif // __GNUC__

namespace Utilities
{
	/// Provides handy features for easier usage of std::regex etc. from <regex>
	struct RegularExpression
	{
	public:
		/** Searches for a pair of real numbers in text which might be divided by a space or a space and an element of {,;:|}.
		@param r0 Is set to the value of the first real number in parameter text or REAL_MAX if the matching failed.
		@param r1 Is set to the value of the second real number in parameter text or REAL_MAX if the matching failed.
		@param text The search is done in text.
		@param matchAtStart Set this to true to require that possible matches begin with the first input character of text.
		@param length Set this to a positive value to restrict the search to the first length characters.
		@return Returns the number of matched characters or 0 if the matching failed. */
		static uint32 getRealPair(Real &r0, Real &r1, const char *text, bool matchAtStart, int32 length = 0);

		/** Searches for pairs of real numbers in text where there is an arbitrary seperator between pairs and a space or a space and an element of {,;:|} between both elements of a single pair.
		@param pairs Found coordinate pairs in text are added to pairs vector according to their occurance order.
		@param text The search is done in text.
		@param length Set this to a positive value to restrict the search to the first length characters.
		@return Returns the number of matched characters or 0 if the matching failed. */
		static uint32 getRealPairs(std::vector<Math::Vector2> &pairs, const char *text, int32 length = 0);

	public:
		#ifdef _REGEX_
			static const char * const INTEGER;		/// Contains an ECMAScript regular expression matching only whole numbers. (z \in Z)
			static const char * const FIXED_POINT;	/// Contains an ECMAScript regular expression matching fixed point numbers of formate integer, integer., .integer or integer.integer
			static const char * const REAL;			/// Contains an ECMAScript regular expression matching floating or fixed point numbers, e.g. -1.23e+06.
			static const char * const REAL_PAIR;	/// Contains an ECMAScript regular expression matching pairs of real numbers like real, real or real real etc.
		#endif // _REGEX
	};
}

#endif // _REGULAR_EXPRESSION_H_
