/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _I_SERIALIZABLE_H_
#define _I_SERIALIZABLE_H_

#include "Platform/DataTypes.h"
#include "IDeserializer.h"
#include "ISerializer.h"

namespace Patterns
{
	/// A class which implements this interface can be serialized or deserialized using objects implementing the IDeserializer or ISerializer interfaces
	/** This interface is used to create an abstraction layer for object serialization and deserialization. Every ISerializable is able to serialize or
	desirialize itself by using objects that implement the IDeserializer or ISerializer interfaces. The ISerializable knows what data is necessary to
	be serialized or deserialized. This data can then be processed by the basic functionality the IDeserializer and ISerializer must provide.*/
	class ISerializable
	{
	public:
		/** Retrieve data from the deserializer to fill this object with this data which is important to represent the ISerializable. 
		@param deserializer The IDeserializer functions should be used to retrieve data from the deserializer in order to fill the ISerializable with the obtained data.*/
		virtual void deserialize(IDeserializer &deserializer) = 0;

		/** Get the amount of memory in bytes this ISerializable object needs to serialize itself. See the ISerializer function documentation to find out how many bytes
		each write function needs in order to determine the entire amount of memory necessary for serialization.
		This information might be required by a class that implements the ISerializer interface since the memory space used to store the serialized data can be limited.
		Due to this function the serializer can then determine weather the serialization of an object is possible or feasable as there is not enough memory left.
		@returns The number of bytes necessary to serialize this object is returned. */
		virtual uint32 getRequiredSerializationMemory() const = 0;

		/** Use the object which implements the ISerializer interface to serialize the data of this object by using the functionality provided by the ISerializer interface.
		@param serializer The ISerializer functions should be used to serialize data by means of the serializer functions to create a serialized representation of this object.*/
		virtual void serialize(ISerializer &serializer) const = 0;
	};
}

#endif // _I_SERIALIZABLE_H_