/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <stdlib.h>
#include "Platform/DataTypes.h"

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!if you change this also change the Encoding enumeration!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const char *EncodingStrings[ENCODING_COUNT] =
{
	"ascii",
	"binary_little_endian",
	"binary_big_endian"
};

#ifdef _WINDOWS
	// "All versions of windows run little-endian, period."
	// http://social.msdn.microsoft.com/Forums/en-US/windowsmobiledev/thread/04c92ef9-e38e-415f-8958-ec9f7c196fd3

	// little or big endian to host order functions
	uint16 bigEndian16ToHostOrder(uint16 bigEndianInt)
	{
		return _byteswap_ushort(bigEndianInt);
	}

	uint32 bigEndian32ToHostOrder(uint32 bigEndianInt)
	{
		return _byteswap_ulong(bigEndianInt);
	}

	uint64 bigEndian64ToHostOrder(uint64 bigEndianInt)
	{
		return _byteswap_uint64(bigEndianInt);
	}

	uint16 littleEndian16ToHostOrder(uint16 littleEndianInt)
	{
		return littleEndianInt;
	}

	uint32 littleEndian32ToHostOrder(uint32 littleEndianInt)
	{
		return littleEndianInt;
	}

	uint64 littleEndian64ToHostOrder(uint64 littleEndianInt)
	{
		return littleEndianInt;
	}

	// host order to big or little endian functions
	uint16 hostOrderToBigEndian16(uint16 hostOrderInt)
	{
		return _byteswap_ushort(hostOrderInt);
	}

	uint32 hostOrderToBigEndian32(uint32 hostOrderInt)
	{
		return _byteswap_ulong(hostOrderInt);
	}

	uint64 hostOrderToBigEndian64(uint64 hostOrderInt)
	{
		return _byteswap_uint64(hostOrderInt);
	}

	uint16 hostOrderToLittleEndian16(uint16 hostOrderInt)
	{
		return hostOrderInt;
	}

	uint32 hostOrderToLittleEndian32(uint32 hostOrderInt)
	{
		return hostOrderInt;
	}

	uint64 hostOrderToLittleEndian64(uint64 hostOrderInt)
	{
		return hostOrderInt;
	}

#elif _LINUX
#include <endian.h>
// BSD:  #include <sys/endian.h>

	uint16 bigEndian16ToHostOrder(uint16 bigEndianInt)
	{
		return be16toh(bigEndianInt);
	}

	uint32 bigEndian32ToHostOrder(uint32 bigEndianInt)
	{
		return be32toh(bigEndianInt);
	}

	uint64 bigEndian64ToHostOrder(uint64 bigEndianInt)
	{
		return be64toh(bigEndianInt);
	}

	uint16 littleEndian16ToHostOrder(uint16 littleEndianInt)
	{
		return le16toh(littleEndianInt);
	}

	uint32 littleEndian32ToHostOrder(uint32 littleEndianInt)
	{
		return le32toh(littleEndianInt);
	}

	uint64 littleEndian64ToHostOrder(uint64 littleEndianInt)
	{
		return le64toh(littleEndianInt);
	}

	// host order to big or little endian functions
	uint16 hostOrderToBigEndian16(uint16 hostOrderInt)
	{
		return htobe16(hostOrderInt);
	}

	uint32 hostOrderToBigEndian32(uint32 hostOrderInt)
	{
		return htobe32(hostOrderInt);
	}

	uint64 hostOrderToBigEndian64(uint64 hostOrderInt)
	{
		return htobe64(hostOrderInt);
	}

	uint16 hostOrderToLittleEndian16(uint16 hostOrderInt)
	{
		return htole16(hostOrderInt);
	}

	uint32 hostOrderToLittleEndian32(uint32 hostOrderInt)
	{
		return htole32(hostOrderInt);
	}

	uint64 hostOrderToLittleEndian64(uint64 hostOrderInt)
	{
		return htole64(hostOrderInt);
	}

#else
	BAM!
	This IS NOT IMPLEMENTED! :)

#endif

