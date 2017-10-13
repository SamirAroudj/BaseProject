/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _I_SERIALIZER_H_
#define _I_SERIALIZER_H_

#include <string>
#include "Platform/DataTypes.h"

namespace Patterns
{	
	/// This inteface provides basic functionality to serialize information by means of an object which implements this interface.
	/** Objects which implement the ISerializer interface take care of the details of the serialization of basic data types.
	Other objects which implement the ISerializable interface can then make use of this functionality in order to serialize their data representation. */
	class ISerializer
	{
	public:
		/** Serialize one character and append it to the target memory which contains the serialization data.
		@param byte This character is serialized to the target memory the ISerializer provides.
		@note Implementations of this method should require one byte of free memory space. */
		virtual void write(char byte) = 0;

		/** Serialize a string and append it to the target memory which contains the serialization data.
		The string should be serialized by writing its characters and ending this sequence whith '\0'.
		@param text This string is serialized to the target memory the ISerializer provides.
		@note Implementations of this method should require length(text) + 1 bytes of free memory space. */
		virtual void write(const std::string &text) = 0;

		/** Serialize an int16 and append it to the target memory which contains the serialization data.
		@param value This int16 is serialized to the target memory the ISerializer provides.
		@note Implementations of this method should require 2 bytes of free memory space. */
		virtual void write(int16 value) = 0;

		/** Serialize an uint16 and append it to the target memory which contains the serialization data.
		@param value This uint16 is serialized to the target memory the ISerializer provides.
		@note Implementations of this method should require 2 bytes of free memory space. */
		virtual void write(uint16 value) = 0;

		/** Serialize an int32 and append it to the target memory which contains the serialization data.
		@param value This int32 is serialized to the target memory the ISerializer provides.
		@note Implementations of this method should require 4 bytes of free memory space. */
		virtual void write(int32 value) = 0;

		/** Serialize an uint32 and append it to the target memory which contains the serialization data.
		@param value This uint32 is serialized to the target memory the ISerializer provides.
		@note Implementations of this method should require 4 bytes of free memory space. */
		virtual void write(uint32 value) = 0;

		/** Serialize an int64 and append it to the target memory which contains the serialization data.
		@param value This int64 is serialized to the target memory the ISerializer provides.
		@note Implementations of this method should require 8 bytes of free memory space. */
		//virtual void write(int64 value) = 0;

		/** Serialize an uint64 and append it to the target memory which contains the serialization data.
		@param value This uint64 is serialized to the target memory the ISerializer provides.
		@note Implementations of this method should require 8 bytes of free memory space. */
		//virtual void write(uint64 value) = 0;

		/** Serialize a floating point value and append it to the target memory which contains the serialization data.
		@param value This floating point value is serialized to the target memory the ISerializer provides.
		@note Implementations of this method should require 4 bytes of free memory space. */
		virtual void write(float value) = 0;

		/** Serialize a double value and append it to the target memory which contains the serialization data.
		@param value This double value is serialized to the target memory the ISerializer provides.
		@note Implementations of this method should require 8 bytes of free memory space. */
		virtual void write(double value) = 0;
	};
}

#endif // _I_SERIALIZER_H_