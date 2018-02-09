/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _UTILITIES_CONVERSIONS_H_
#define _UTILITIES_CONVERSIONS_H_

#include <string>
#include "Platform/DataTypes.h"
#include "Platform/FailureHandling/Exception.h"

namespace Utilities
{
	/** Converts a value in text format to another representation of type T.
	@param textValue Set this to the text-based (ASCII encoded) value you want to convert to type T.
	@return Returns the type T representation of textValue. */
	template <class T>
	inline T convert(const std::string &textValue);

	/** Converts a set of text-based (ASCII encoded) values in the format { value, value, value, ... } to an array of type T values.
		Values must be separated by ", " and each value in turn is converted from its ASCII-based representation to type T via T Utlities::convert(const std::string &textValue).
	@param values Is filled with the conversion results and must have space for maxValueCount elements.
	@param maxValueCount Set this to the maximal number of type T elements which fit into bslurd.
	@param textValues Set this to the ASCII-encoded values to be converted which must have the format { value, value, value, value, ... }.
	@return Returns the number of converted values which have been written to values. */
	template <class T>
	uint32 convert(T *values, const uint32 maxValueCount, const std::string &textValues);

	/** Converts an array of elementCount InputType elements in inputArray to elementCount OutputType elements.
		The input array is freed and a new array for the converted type is returned.
	@param inputArray Set this to the array of elements you want to convert whereas each element must have the second template parameter as type.
		inputArray is freed and the pointer is set to NULL.
	@param elementCount Set this to the number of elements / InputType objects in the array inputArray.
	@return Returns an array of elementCount elements / objects of type OutputType (first template parameter) which was created from inputArray.*/
	template <class OutputType, class InputType>
	OutputType *convert(void *&inputArray, const uint32 elementCount);

	/** Returns true if booleanText is element of { "t", "true", "yes", "y", "1" } otherwise false is returned.
	@return Returns true if booleanText is element of { "t", "true", "yes", "y", "1" } otherwise false is returned. */
	bool convertToBoolean(const std::string &booleanText);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	inline T convert(const std::string &textValue)
	{
		T temp = 0;
		throw FailureHandling::Exception("Unsupported type for T Utilities::convert(const std::string &textValue)!");
		return temp;
	}

	template <>
	inline int8 convert<int8>(const std::string &textValue)
	{
		return (int8) strtol(textValue.c_str(), NULL, 10);
	}

	template <>
	inline int16 convert<int16>(const std::string &textValue)
	{
		return (int16) strtol(textValue.c_str(), NULL, 10);
	}

	template <>
	inline int32 convert<int32>(const std::string &textValue)
	{
		return (int32) strtol(textValue.c_str(), NULL, 10);
	}

	template <>
	inline int64 convert<int64>(const std::string &textValue)
	{
		return (int64) strtoll(textValue.c_str(), NULL, 10);
	}

	template <>
	inline Real convert<Real>(const std::string &textValue)
	{
		#ifdef DOUBLE_PRECISION
			return strtod(textValue.c_str(), NULL);
		#else
			return strtof(textValue.c_str(), NULL);
		#endif	
	}

	template <>
	inline uint8 convert<uint8>(const std::string &textValue)
	{
		return (uint8) strtoul(textValue.c_str(), NULL, 10);
	}

	template <>
	inline uint16 convert<uint16>(const std::string &textValue)
	{
		return (uint16) strtoul(textValue.c_str(), NULL, 10);
	}

	template <>
	inline uint32 convert<uint32>(const std::string &textValue)
	{
		return (uint32) strtoul(textValue.c_str(), NULL, 10);
	}

	template <>
	inline uint64 convert<uint64>(const std::string &textValue)
	{
		return (uint64) strtoull(textValue.c_str(), NULL, 10);
	}

	template <class T>
	uint32 convert(T *values, const uint32 maxValueCount, const std::string &textValues)
	{
		// format: { value, value, value, ... }
		std::string value;
		
		// valid start & end?
		uint32 count = 0;
		if (textValues.front() != '{' || textValues.back() != '}')
			return count;

		// convert each number from text to its T representation
		size_t valueStart = 1;
		size_t valueEnd;
		for (; count < maxValueCount; ++count, valueStart = valueEnd + 1)
		{
			// find the next number
			while (isblank(textValues[valueStart]))
				++valueStart;
			valueEnd = textValues.find(',', valueStart + 1);

			// valid area?
			if (string::npos == valueEnd)
				return count;

			// convert the number
			const std::string value = textValues.substr(valueStart, valueEnd - valueStart);
			values[count] = convert<T>(value);
		}
		
		return count;
	}

	template <class OutputType, class InputType>
	OutputType *convert(void *&inputArray, const uint32 elementCount)
	{
		// allocate elementCount elements of the output type
		OutputType *outputArray = new OutputType[elementCount];

		// convert each element
		InputType *temp = (InputType *) inputArray;
		for (uint32 eleIdx = 0; eleIdx < elementCount; ++eleIdx)
			outputArray[eleIdx] = (OutputType) temp[eleIdx];
	
		// free input
		delete [] temp;
		temp = NULL;
		inputArray = NULL;

		// return converted input
		return outputArray;
	}
}

#endif // _UTILITIES_CONVERSIONS_H_