/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _DATA_TYPES_H_
#define _DATA_TYPES_H_

#include <inttypes.h>
#include "../Math/MathCore.h"

// Define fixed size integer types.
typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

/// Defines what kind of data is supposed to be in a particular address space, e.g. a buffer read from some file.
/** Helps to identify whether data is stored as ASCII characters, in little-endian binary format, ...
	@see	https://en.wikipedia.org/wiki/ASCII\n
			https://en.wikipedia.org/wiki/Endianness\n
			EncodingStrings */
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!if you change this also change the Encoding enumeration!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
enum Encoding
{
	ENCODING_ASCII,					/// Simple ASCII encoding which can be read manually :)
	ENCODING_BINARY_LITTLE_ENDIAN,	/// Data can mean anything, but if it is contains integers then these are stored in little endian format.
	ENCODING_BINARY_BIG_ENDIAN,		/// Data can mean anything, but if it is contains integers then these are stored in big endian format.
	ENCODING_COUNT					/// Number of different data types.
};

/** Contains the text versions for the enumeration Encoding. 
	@see Encoding */
extern const char *EncodingStrings[ENCODING_COUNT];

/** Enumeration for identification of integer bases, e.g., hexadecimal numbers like F1A3BD26 (32 bit). */
enum NumberBase
{
	NUMBER_BASE_DECIMAL,		/// decimal numbers, base 10 -> allowed characters 0 - 9
	NUMBER_BASE_HEXADECIMAL,	/// hexadecimal numbers, base 16, allowed characters 0 - 9, A - F
	NUMBER_BASE_OCTAL,			/// octal numbers, base 16, allowed characters 0 - 7
	INTERGER_BASE_COUNT
};

// conversion functions needed for integers due to different encoding (little and big endian)
/** Converts an integer with big endian encoding to an integer with this machine's encoding.
@param bigEndianInt Set this to the big endian integer you want to convert.
@return Returns an integer which can normally be used by this machine. */
uint16 bigEndian16ToHostOrder(uint16 bigEndianInt);

/** Converts an integer with big endian encoding to an integer with this machine's encoding.
@param bigEndianInt Set this to the big endian integer you want to convert.
@return Returns an integer which can normally be used by this machine. */
uint32 bigEndian32ToHostOrder(uint32 bigEndianInt);

/** Converts an integer with big endian encoding to an integer with this machine's encoding.
@param bigEndianInt Set this to the big endian integer you want to convert.
@return Returns an integer which can normally be used by this machine. */
uint64 bigEndian64ToHostOrder(uint64 bigEndianInt);

/** Converts an integer with little endian encoding to an integer with this machine's encoding.
@param littleEndianInt Set this to the little endian integer you want to convert.
@return Returns an integer which can normally be used by this machine. */
uint16 littleEndian16ToHostOrder(uint16 littleEndianInt);

/** Converts an integer with little endian encoding to an integer with this machine's encoding.
@param littleEndianInt Set this to the little endian integer you want to convert.
@return Returns an integer which can normally be used by this machine. */
uint32 littleEndian32ToHostOrder(uint32 littleEndianInt);

/** Converts an integer with little endian encoding to an integer with this machine's encoding.
@param littleEndianInt Set this to the little endian integer you want to convert.
@return Returns an integer which can normally be used by this machine. */
uint64 littleEndian64ToHostOrder(uint64 littleEndianInt);

/** Converts an integer with host order (this machine's) encoding to an integer with big endian encoding.
@param hostOrderInt Set this to the host order integer you want to convert.
@return Returns the corresponding integer with big endian encoding. */
uint16 hostOrderToBigEndian16(uint16 hostOrderInt);

/** Converts an integer with host order (this machine's) encoding to an integer with big endian encoding.
@param hostOrderInt Set this to the host order integer you want to convert.
@return Returns the corresponding integer with big endian encoding. */
uint32 hostOrderToBigEndian32(uint32 hostOrderInt);

/** Converts an integer with host order (this machine's) encoding to an integer with big endian encoding.
@param hostOrderInt Set this to the host order integer you want to convert.
@return Returns the corresponding integer with big endian encoding. */
uint64 hostOrderToBigEndian64(uint64 hostOrderInt);

/** Converts an integer with host order (this machine's) encoding to an integer with little endian encoding.
@param hostOrderInt Set this to the host order integer you want to convert.
@return Returns the corresponding integer with little endian encoding. */
uint16 hostOrderToLittleEndian16(uint16 hostOrderInt);

/** Converts an integer with host order (this machine's) encoding to an integer with little endian encoding.
@param hostOrderInt Set this to the host order integer you want to convert.
@return Returns the corresponding integer with little endian encoding. */
uint32 hostOrderToLittleEndian32(uint32 hostOrderInt);

/** Converts an integer with host order (this machine's) encoding to an integer with little endian encoding.
@param hostOrderInt Set this to the host order integer you want to convert.
@return Returns the corresponding integer with little endian encoding. */
uint64 hostOrderToLittleEndian64(uint64 hostOrderInt);

// windows
//		typedef char int8;
//		typedef unsigned char uint8;
//		typedef __int16 int16;
//		typedef __int32 int32;
//		typedef __int64 int64;
//		typedef unsigned __int16 uint16;
//		typedef unsigned __int32 uint32;
//		typedef unsigned __int64 uint64;

// Defines which are necessary due to incomplete C or C++ standard
#ifdef __GNUC__
	#define restrict __restrict__
#elif  __CUDACC__
	#define restrict __restrict__
#else
	
#endif // __GNUC__

#endif // _DATA_TYPES_H_
