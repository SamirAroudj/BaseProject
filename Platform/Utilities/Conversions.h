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
	class Converter
	{
	public:
		static inline std::string from(const int8 &value, const uint32 &minLength = 0)
		{
			return from<uint8>(value, minLength);
		}

		template <IntegerBase base>
		static inline std::string from(const uint8 &value, const uint32 &minLength = 0)
		{
			return from<uint8, base>(value, minLength);
		}

		/** Converts a value in text format to another representation of type T.
		@param textValue Set this to the text-based (ASCII encoded) value you want to convert to type T.
		@return Returns the type T representation of textValue. */
		template <class T>
		static inline T to(const std::string &textValue);

		/** Converts a set of text-based (ASCII encoded) values in the format { value, value, value, ... } to an array of type T values.
			Values must be separated by ", " and each value in turn is converted from its ASCII-based representation to type T via T Utlities::convert(const std::string &textValue).
		@param values Is filled with the conversion results and must have space for maxValueCount elements.
		@param maxValueCount Set this to the maximal number of type T elements which fit into bslurd.
		@param textValues Set this to the ASCII-encoded values to be converted which must have the format { value, value, value, value, ... }.
		@return Returns the number of converted values which have been written to values. */
		template <class T>
		static uint32 to(T *values, const uint32 maxValueCount, const std::string &textValues);

		/** Returns true if booleanText is element of { "t", "true", "yes", "y", "1" } otherwise false is returned.
		@return Returns true if booleanText is element of { "t", "true", "yes", "y", "1" } otherwise false is returned. */
		static bool toBoolean(const std::string &booleanText);

		/** Converts an array of elementCount InputType elements in inputArray to elementCount OutputType elements.
			The input array is freed and a new array for the converted type is returned.
		@param inputArray Set this to the array of elements you want to convert whereas each element must have the second template parameter as type.
			inputArray is freed and the pointer is set to NULL.
		@param elementCount Set this to the number of elements / InputType objects in the array inputArray.
		@return Returns an array of elementCount elements / objects of type OutputType (first template parameter) which was created from inputArray.*/
		template <class OutputType, class InputType>
		static OutputType *toFirstFromSecond(void *&inputArray, const uint32 elementCount);
	
		/** Converts an integer base to the corresponding printf specifier, e.g.,\n
		INTEGER_BASE_DECIMAL -> u \n
		INTEGER_BASE_HEXADECIMAL -> X \n
		INTEGER_BASE_OCTAL -> o \n
		@param base Defines the number base, e.g., decimal.
		@return Returns the printf specifier which corresponds to base. See printf specifier documentation (header: <cstdio>).
		@see See printf specifier documentation (header: <cstdio>).*/
		static inline const char toPrintfSpecifier(const IntegerBase &base);

	private:
		/** Converts an unsigned integer to a string (with base base) corresponding to that integer.
		@param value This unsigned integer is converted to a string. E.g. from<uint32>(42) -> "42".
		@param minLength Use this to get strings of a minimum length of minLength characters via left-side zero padding.
			\nExamples: from<uint32>(42, 3) -> "042" | from<uint32>(42, 1) -> "42"
		@return Returns a string corresponding to the unsigned type T integer whereas the string contains a base base number consisting of at least minLength characters.*/
		template <class T, IntegerBase base = INTEGER_BASE_DECIMAL>
		static inline std::string from(const T &value, const uint32 &minLength = 0);

		/** Returns the printf format string "%.*<something>" for T and Base, e.g., "%.*hhu".
			The format strings are used to convert integer numbers to strings via snprintf.
		@return Returns the printf format string which fits to converting a number of type T and base base to a string via snprintf. */
		template <class T, IntegerBase base>
		static inline const char *getFormat();

		/** Converts a single integer to a string with minLength as optional 0 padding.
		@param value This type T integer is converted to a string with base base.
		@param minLength Set this to 0 to avoid padding or to minLength to have a string number with at least minLength characters.
		@return Returns a string (with base base) corresponding to the integer value and it has the minimum length minLength.
			\nExample: viaPrintf<uint8, INTEGER_BASE_DECIMAL>(42u, 4) -> "0042".*/
		template<class T, IntegerBase base>
		static inline std::string viaPrintf(const T &value, const uint32 &minLength);

		// specialized version for template getFormat();
		// decimal integer format strings
		template <>
		static inline const char *getFormat<uint8, INTEGER_BASE_DECIMAL>();

		template <>
		static inline const char *getFormat<uint16, INTEGER_BASE_DECIMAL>();

		template <>
		static inline const char *getFormat<uint32, INTEGER_BASE_DECIMAL>();

		template <>
		static inline const char *getFormat<uint64, INTEGER_BASE_DECIMAL>();

		// hexadecimal integer format strings
		template <>
		static inline const char *getFormat<uint8, INTEGER_BASE_HEXADECIMAL>();

		template <>
		static inline const char *getFormat<uint16, INTEGER_BASE_HEXADECIMAL>();

		template <>
		static inline const char *getFormat<uint32, INTEGER_BASE_HEXADECIMAL>();

		template <>
		static inline const char *getFormat<uint64, INTEGER_BASE_HEXADECIMAL>();

		// octal integer format strings
		template <>
		static inline const char *getFormat<uint8, INTEGER_BASE_OCTAL>();

		template <>
		static inline const char *getFormat<uint16, INTEGER_BASE_OCTAL>();

		template <>
		static inline const char *getFormat<uint32, INTEGER_BASE_OCTAL>();

		template <>
		static inline const char *getFormat<uint64, INTEGER_BASE_OCTAL>();
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T, IntegerBase base>
	inline std::string Converter::from(const T &value, const uint32 &minLength)
	{
		not implemented;
	}
		
	template <IntegerBase base>
	inline std::string Converter::from<uint8, base>(const uint8 &value, const uint32 &minLength)
	{
		return viaPrintf<uint8, base>(value, minLength);
	}
		
	template <>
	inline std::string Converter::from<uint16, INTEGER_BASE_DECIMAL>(const uint16 &value, const uint32 &minLength)
	{
		return viaPrintf<uint16, INTEGER_BASE_DECIMAL>(value, minLength);
	}
	
	template <class T, IntegerBase base>
	inline std::string Converter::viaPrintf(const T &value, const uint32 &minLength)
	{
		// create the value
		const uint32 MAX_LENGTH = 1000;
		char buffer[MAX_LENGTH];
		std::snprintf(buffer, MAX_LENGTH, getFormat<T, base>(), minLength, value);

		// convert it to a string
		return std::string(buffer);
	}
	
	template <class T, IntegerBase base>
	inline const char *Converter::getFormat()
	{
		not implemented;
	}

	// decimal integer format strings
	template <>
	inline const char *Converter::getFormat<uint8, INTEGER_BASE_DECIMAL>()
	{
		return "%.*hhu";
	}

	template <>
	inline const char *Converter::getFormat<uint16, INTEGER_BASE_DECIMAL>()
	{
		return "%.*hu";
	}

	template <>
	inline const char *Converter::getFormat<uint32, INTEGER_BASE_DECIMAL>()
	{
		return "%.*u";
	}

	template <>
	inline const char *Converter::getFormat<uint64, INTEGER_BASE_DECIMAL>()
	{
		return "%.*lu";
	}

	// hexadecimal integer format strings
	template <>
	inline const char *Converter::getFormat<uint8, INTEGER_BASE_HEXADECIMAL>()
	{
		return "%.*hhX";
	}

	template <>
	inline const char *Converter::getFormat<uint16, INTEGER_BASE_HEXADECIMAL>()
	{
		return "%.*hX";
	}

	template <>
	inline const char *Converter::getFormat<uint32, INTEGER_BASE_HEXADECIMAL>()
	{
		return "%.*X";
	}

	template <>
	inline const char *Converter::getFormat<uint64, INTEGER_BASE_HEXADECIMAL>()
	{
		return "%.*lX";
	}

	// octal integer format strings
	template <>
	inline const char *Converter::getFormat<uint8, INTEGER_BASE_OCTAL>()
	{
		return "%.*hho";
	}

	template <>
	inline const char *Converter::getFormat<uint16, INTEGER_BASE_OCTAL>()
	{
		return "%.*ho";
	}

	template <>
	inline const char *Converter::getFormat<uint32, INTEGER_BASE_OCTAL>()
	{
		return "%.*o";
	}

	template <>
	inline const char *Converter::getFormat<uint64, INTEGER_BASE_OCTAL>()
	{
		return "%.*lo";
	}

	//int8 "hhi";
	//int16 "hi";
	//int32 "i";
	//int64 "li";

	//float "f";
	//long double "L"

	template <class T>
	inline T Converter::to(const std::string &textValue)
	{
		not implemented;
	}

	template <>
	inline int8 Converter::to<int8>(const std::string &textValue)
	{
		return (int8) strtol(textValue.c_str(), NULL, 10);
	}

	template <>
	inline int16 Converter::to<int16>(const std::string &textValue)
	{
		return (int16) strtol(textValue.c_str(), NULL, 10);
	}

	template <>
	inline int32 Converter::to<int32>(const std::string &textValue)
	{
		return (int32) strtol(textValue.c_str(), NULL, 10);
	}

	template <>
	inline int64 Converter::to<int64>(const std::string &textValue)
	{
		return (int64) strtoll(textValue.c_str(), NULL, 10);
	}

	template <>
	inline Real Converter::to<Real>(const std::string &textValue)
	{
		#ifdef DOUBLE_PRECISION
			return strtod(textValue.c_str(), NULL);
		#else
			return strtof(textValue.c_str(), NULL);
		#endif	
	}

	template <>
	inline uint8 Converter::to<uint8>(const std::string &textValue)
	{
		return (uint8) strtoul(textValue.c_str(), NULL, 10);
	}

	template <>
	inline uint16 Converter::to<uint16>(const std::string &textValue)
	{
		return (uint16) strtoul(textValue.c_str(), NULL, 10);
	}

	template <>
	inline uint32 Converter::to<uint32>(const std::string &textValue)
	{
		return (uint32) strtoul(textValue.c_str(), NULL, 10);
	}

	template <>
	inline uint64 Converter::to<uint64>(const std::string &textValue)
	{
		return (uint64) strtoull(textValue.c_str(), NULL, 10);
	}

	template <class T>
	uint32 Converter::to(T *values, const uint32 maxValueCount, const std::string &textValues)
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
			if (std::string::npos == valueEnd)
				return count;

			// convert the number
			const std::string value = textValues.substr(valueStart, valueEnd - valueStart);
			values[count] = Converter::to<T>(value);
		}
		
		return count;
	}

	template <class OutputType, class InputType>
	OutputType *Converter::toFirstFromSecond(void *&inputArray, const uint32 elementCount)
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

	inline const char Converter::toPrintfSpecifier(const IntegerBase &base)
	{
		switch (base)
		{
			case INTEGER_BASE_DECIMAL: return 'u'; break;
			case INTEGER_BASE_HEXADECIMAL: return 'X'; break;
			case INTEGER_BASE_OCTAL: return 'o'; break;
			default: throw FailureHandling::Exception("Unimplemented base in convert to integerBaseToPrintfSpecifier!");
		}

		return 'u';
	}
}

#endif // _UTILITIES_CONVERSIONS_H_
