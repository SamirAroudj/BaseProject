/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _I_DESERIALIZER_H_
#define _I_DESERIALIZER_H_

#include <string>
#include "Platform/DataTypes.h"

namespace Patterns
{
	/// This inteface provides basic functionality to retrieve serialized information from an object which implements this interface.
	/** Objects which implement the IDeserializer interface take care of the details of the deserialization of basic data types.
	Other objects which implement the ISerializable interface can then make use of this functionality in order to get their representation data. */
	class IDeserializer
	{
	public:
		/** The next expected data type in the source of the IDeserializer is char.
		@returns Interprete the next byte as a character value and return the corresponding value. */
		virtual char readChar() = 0;

		/** The next expected data type in the source of the IDeserializer is string.
		@param text Interprete the next bytes until '\0' was read as a characters of a string and return the corresponding value. */
		virtual void readString(std::string &text) = 0;

		/** The next expected data type in the source of the IDeserializer is int16.
		@returns Interprete the next two bytes as an int16 value and return the corresponding value. */
		virtual int16 readInt16() = 0;

		/** The next expected data type in the source of the IDeserializer is uint16.
		@returns Interprete the next two bytes as an uint16 value and return the corresponding value. */
		virtual uint16 readUInt16() = 0;
		
		/** The next expected data type in the source of the IDeserializer is int32.
		@returns Interprete the next four bytes as an int32 value and return the corresponding value. */
		virtual int32 readInt32() = 0;
		
		/** The next expected data type in the source of the IDeserializer is uint32.
		@returns Interprete the next four bytes as an uint32 value and return the corresponding value. */
		virtual uint32 readUInt32() = 0;
		
		/** The next expected data type in the source of the IDeserializer is int64.
		@returns Interprete the next eight bytes as an int64 value and return the corresponding value. */
		//virtual int64 readInt64() = 0;
		
		/** The next expected data type in the source of the IDeserializer isuint64.
		@returns Interprete the next eight bytes as an uint64 value and return the corresponding value. */
		//virtual uint64 readUInt64() = 0;

		/** The next expected data type in the source of the IDeserializer is Real.
		@returns Interprete the next four bytes as a single precision floating point value and return the corresponding value. */
		virtual Real readFloat() = 0;
		
		/** The next expected data type in the source of the IDeserializer is double.
		@returns Interprete the next eight bytes as a double value and return the corresponding value. */
		virtual double readDouble() = 0;
	};
}

#endif // _I_DESERIALIZER_H_