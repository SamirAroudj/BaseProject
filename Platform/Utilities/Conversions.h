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
		/** Converts an integer or floating point number to a string (with number base base) representing that input number.
			Not all combinations of base and value type T are supported.
			\nSupported combinations:
			\nUnsigned integers support NUMBER_BASE_DECIMAL, NUMBER_BASE_HEXADECIMAL and NUMBER_BASE_OCTAL
			\nSigned integers support only NUMBER_BASE_DECIMAL.
			\nFloating point values support NUMBER_BASE_DECIMAL and NUMBER_BASE_HEXADECIMAL.
		@param value This value is converted to a string. E.g. from<uint32>(42) -> "42".
		@param length Use this to get strings of a minimum length.
			\nFor integers, length is the minimum number of characters the string must have which is enforced via left-side zero padding.
			\nExamples: from<uint32>(42, 3) -> "042" | from<uint32>(42, 1) -> "42"
			\nFor floating point numbers, length is the number of digits after the decimal point character (also called precision).
			\nExamples from<double>(3.24, 3) -> "3.240" | from<float>(42.042f, 2) -> "42.04"
		@return Returns a string representing the type T number whereas the string contains a number with base base.*/
		template <typename T, NumberBase base = NUMBER_BASE_DECIMAL>
		static inline std::string from(const T &value, const uint32 &length = 0);

		/** Converts a value in text format to another representation of type T.
			Not all value types T are supported.
			\nSupported types: int8, ..., int64, Real, uint8, ..., uint64.
		@param textValue Set this to the text-based (ASCII encoded) value you want to convert to type T.
		@return Returns the type T representation of textValue. */
		template <typename T>
		static inline T to(const std::string &textValue);

		/** Converts a set of text-based (ASCII encoded) values in the format { value, value, value, ... } to an array of type T values.
			Values must be separated by ", " and each value in turn is converted from its ASCII-based representation to type T via T Utlities::convert(const std::string &textValue).
		@param values Is filled with the conversion results and must have space for maxValueCount elements.
		@param maxValueCount Set this to the maximal number of type T elements which fit into bslurd.
		@param textValues Set this to the ASCII-encoded values to be converted which must have the format { value, value, value, value, ... }.
		@return Returns the number of converted values which have been written to values. */
		template <typename T>
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
		NUMBER_BASE_DECIMAL -> u \n
		NUMBER_BASE_HEXADECIMAL -> X \n
		NUMBER_BASE_OCTAL -> o \n
		@param base Defines the number base, e.g., decimal.
		@return Returns the printf specifier which corresponds to base. See printf specifier documentation (header: <cstdio>).
		@see See printf specifier documentation (header: <cstdio>).*/
		static inline const char toPrintfSpecifier(const NumberBase &base);

	private:
		/** Returns the printf format string "%.*<something>" for T and Base, e.g., "%.*hhu".
			The format strings are used to convert integer numbers to strings via snprintf.
		@return Returns the printf format string which fits to converting a number of type T and base base to a string via snprintf. */
		template <typename T, NumberBase base>
		static inline const char *getFormat();

		/** Converts a single number to a string using printf.
		@param value This type T number is converted to a string with base base.
		@param length For unsigned integers it enforces at least length characters in the resulting string via left side zero padding.
			For floating point numbers it defines the precisionn, in particular, the number of digits after the decimal point character.
		@return Returns a string (with base base) corresponding to the type T value.
			\nExamples: viaPrintf<uint8, NUMBER_BASE_DECIMAL>(42u, 4) -> "0042", viaPrintf<float, NUMBER_BASE_DECIMAL>(42.042, 2) -> "42.04".*/
		template<typename T, NumberBase base>
		static std::string viaPrintf(const T &value, const uint32 &length);
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename T, NumberBase base>
	inline std::string Converter::from(const T &value, const uint32 &length)
	{
		not implemented;
	}

	// specialize / allow Converter::from for double (decimal or hexadecimal)
	template <>
	inline std::string Converter::from<double, NUMBER_BASE_DECIMAL>(const double &value, const uint32 &precision)
	{
		return viaPrintf<double, NUMBER_BASE_DECIMAL>(value, precision);
	}

	template <>
	inline std::string Converter::from<double, NUMBER_BASE_HEXADECIMAL>(const double &value, const uint32 &precision)
	{
		return viaPrintf<double, NUMBER_BASE_HEXADECIMAL>(value, precision);
	}

	// specialize / allow Converter::from for float (decimal or hexadecimal)
	template <>
	inline std::string Converter::from<float, NUMBER_BASE_DECIMAL>(const float &value, const uint32 &precision)
	{
		return viaPrintf<float, NUMBER_BASE_DECIMAL>(value, precision);
	}

	template <>
	inline std::string Converter::from<float, NUMBER_BASE_HEXADECIMAL>(const float &value, const uint32 &precision)
	{
		return viaPrintf<float, NUMBER_BASE_HEXADECIMAL>(value, precision);
	}

	// specialize / allow Converter::from for signed integer with decimal base (only decimal for signed integers due to printf restriction)
	template <>
	inline std::string Converter::from<int8, NUMBER_BASE_DECIMAL>(const int8 &value, const uint32 &minLength)
	{
		return viaPrintf<int8, NUMBER_BASE_DECIMAL>(value, minLength);
	}

	template <>
	inline std::string Converter::from<int16, NUMBER_BASE_DECIMAL>(const int16 &value, const uint32 &minLength)
	{
		return viaPrintf<int16, NUMBER_BASE_DECIMAL>(value, minLength);
	}

	template <>
	inline std::string Converter::from<int32, NUMBER_BASE_DECIMAL>(const int32 &value, const uint32 &minLength)
	{
		return viaPrintf<int32, NUMBER_BASE_DECIMAL>(value, minLength);
	}

	template <>
	inline std::string Converter::from<int64, NUMBER_BASE_DECIMAL>(const int64 &value, const uint32 &minLength)
	{
		return viaPrintf<int64, NUMBER_BASE_DECIMAL>(value, minLength);
	}
	
	// specialize / allow Converter::from for long double (decimal or hexadecimal)
	template <>
	inline std::string Converter::from<long double, NUMBER_BASE_DECIMAL>(const long double &value, const uint32 &precision)
	{
		return viaPrintf<long double, NUMBER_BASE_DECIMAL>(value, precision);
	}

	template <>
	inline std::string Converter::from<long double, NUMBER_BASE_HEXADECIMAL>(const long double &value, const uint32 &precision)
	{
		return viaPrintf<long double, NUMBER_BASE_HEXADECIMAL>(value, precision);
	}

	// specialize / allow Converter::from for unsigned integer with different bases
	template <>
	inline std::string Converter::from<uint8, NUMBER_BASE_DECIMAL>(const uint8 &value, const uint32 &minLength)
	{
		return viaPrintf<uint8, NUMBER_BASE_DECIMAL>(value, minLength);
	}
		
	template <>
	inline std::string Converter::from<uint8, NUMBER_BASE_HEXADECIMAL>(const uint8 &value, const uint32 &minLength)
	{
		return viaPrintf<uint8, NUMBER_BASE_HEXADECIMAL>(value, minLength);
	}
		
	template <>
	inline std::string Converter::from<uint8, NUMBER_BASE_OCTAL>(const uint8 &value, const uint32 &minLength)
	{
		return viaPrintf<uint8, NUMBER_BASE_OCTAL>(value, minLength);
	}
		
	template <>
	inline std::string Converter::from<uint16, NUMBER_BASE_DECIMAL>(const uint16 &value, const uint32 &minLength)
	{
		return viaPrintf<uint16, NUMBER_BASE_DECIMAL>(value, minLength);
	}
		
	template <>
	inline std::string Converter::from<uint16, NUMBER_BASE_HEXADECIMAL>(const uint16 &value, const uint32 &minLength)
	{
		return viaPrintf<uint16, NUMBER_BASE_HEXADECIMAL>(value, minLength);
	}
		
	template <>
	inline std::string Converter::from<uint16, NUMBER_BASE_OCTAL>(const uint16 &value, const uint32 &minLength)
	{
		return viaPrintf<uint16, NUMBER_BASE_OCTAL>(value, minLength);
	}
		
	template <>
	inline std::string Converter::from<uint32, NUMBER_BASE_DECIMAL>(const uint32 &value, const uint32 &minLength)
	{
		return viaPrintf<uint32, NUMBER_BASE_DECIMAL>(value, minLength);
	}
		
	template <>
	inline std::string Converter::from<uint32, NUMBER_BASE_HEXADECIMAL>(const uint32 &value, const uint32 &minLength)
	{
		return viaPrintf<uint32, NUMBER_BASE_HEXADECIMAL>(value, minLength);
	}
		
	template <>
	inline std::string Converter::from<uint32, NUMBER_BASE_OCTAL>(const uint32 &value, const uint32 &minLength)
	{
		return viaPrintf<uint32, NUMBER_BASE_OCTAL>(value, minLength);
	}
		
	template <>
	inline std::string Converter::from<uint64, NUMBER_BASE_DECIMAL>(const uint64 &value, const uint32 &minLength)
	{
		return viaPrintf<uint64, NUMBER_BASE_DECIMAL>(value, minLength);
	}
		
	template <>
	inline std::string Converter::from<uint64, NUMBER_BASE_HEXADECIMAL>(const uint64 &value, const uint32 &minLength)
	{
		return viaPrintf<uint64, NUMBER_BASE_HEXADECIMAL>(value, minLength);
	}
		
	template <>
	inline std::string Converter::from<uint64, NUMBER_BASE_OCTAL>(const uint64 &value, const uint32 &minLength)
	{
		return viaPrintf<uint64, NUMBER_BASE_OCTAL>(value, minLength);
	}

	// functions necessary for from
	template <typename T, NumberBase base>
	std::string Converter::viaPrintf(const T &value, const uint32 &length)
	{
		// create the value
		const uint32 MAX_LENGTH = 1000;
		char buffer[MAX_LENGTH];
		std::snprintf(buffer, MAX_LENGTH, getFormat<T, base>(), length, value);

		// convert it to a string
		return std::string(buffer);
	}
	
	template <typename T, NumberBase base>
	inline const char *Converter::getFormat()
	{
		not implemented;
	}

	// double format strings
	template <>
	inline const char *Converter::getFormat<double, NUMBER_BASE_DECIMAL>()
	{
		return "%.*lf";
	}

	template <>
	inline const char *Converter::getFormat<double, NUMBER_BASE_HEXADECIMAL>()
	{
		return "%.*lA";
	}

	// float format strings
	template <>
	inline const char *Converter::getFormat<float, NUMBER_BASE_DECIMAL>()
	{
		return "%.*f";
	}
	
	// hexadecimal floating point format strings
	template <>
	inline const char *Converter::getFormat<float, NUMBER_BASE_HEXADECIMAL>()
	{
		return "%.*A";
	}

	// decimal signed integer format strings
	template <>
	inline const char *Converter::getFormat<int8, NUMBER_BASE_DECIMAL>()
	{
		return "%.*hhi";
	}

	template <>
	inline const char *Converter::getFormat<int16, NUMBER_BASE_DECIMAL>()
	{
		return "%.*hi";
	}

	template <>
	inline const char *Converter::getFormat<int32, NUMBER_BASE_DECIMAL>()
	{
		return "%.*i";
	}

	template <>
	inline const char *Converter::getFormat<int64, NUMBER_BASE_DECIMAL>()
	{
		return "%.*li";
	}
	
	// long double format strings
	template <>
	inline const char *Converter::getFormat<long double, NUMBER_BASE_DECIMAL>()
	{
		return "%.*Lf";
	}

	template <>
	inline const char *Converter::getFormat<long double, NUMBER_BASE_HEXADECIMAL>()
	{
		return "%.*LA";
	}

	// decimal unsigned integer format strings
	template <>
	inline const char *Converter::getFormat<uint8, NUMBER_BASE_DECIMAL>()
	{
		return "%.*hhu";
	}

	template <>
	inline const char *Converter::getFormat<uint16, NUMBER_BASE_DECIMAL>()
	{
		return "%.*hu";
	}

	template <>
	inline const char *Converter::getFormat<uint32, NUMBER_BASE_DECIMAL>()
	{
		return "%.*u";
	}

	template <>
	inline const char *Converter::getFormat<uint64, NUMBER_BASE_DECIMAL>()
	{
		return "%.*lu";
	}

	// hexadecimal unsigned integer format strings
	template <>
	inline const char *Converter::getFormat<uint8, NUMBER_BASE_HEXADECIMAL>()
	{
		return "%.*hhX";
	}

	template <>
	inline const char *Converter::getFormat<uint16, NUMBER_BASE_HEXADECIMAL>()
	{
		return "%.*hX";
	}

	template <>
	inline const char *Converter::getFormat<uint32, NUMBER_BASE_HEXADECIMAL>()
	{
		return "%.*X";
	}

	template <>
	inline const char *Converter::getFormat<uint64, NUMBER_BASE_HEXADECIMAL>()
	{
		return "%.*lX";
	}

	// octal unsigned integer format strings
	template <>
	inline const char *Converter::getFormat<uint8, NUMBER_BASE_OCTAL>()
	{
		return "%.*hho";
	}

	template <>
	inline const char *Converter::getFormat<uint16, NUMBER_BASE_OCTAL>()
	{
		return "%.*ho";
	}

	template <>
	inline const char *Converter::getFormat<uint32, NUMBER_BASE_OCTAL>()
	{
		return "%.*o";
	}

	template <>
	inline const char *Converter::getFormat<uint64, NUMBER_BASE_OCTAL>()
	{
		return "%.*lo";
	}

	// Converter::to -> convert from string to some type T
	template <typename T>
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

	template <typename T>
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

	inline const char Converter::toPrintfSpecifier(const NumberBase &base)
	{
		switch (base)
		{
			case NUMBER_BASE_DECIMAL: return 'u'; break;
			case NUMBER_BASE_HEXADECIMAL: return 'X'; break;
			case NUMBER_BASE_OCTAL: return 'o'; break;
			default: throw FailureHandling::Exception("Unimplemented base in convert to integerBaseToPrintfSpecifier!");
		}

		return 'u';
	}
}

#endif // _UTILITIES_CONVERSIONS_H_
