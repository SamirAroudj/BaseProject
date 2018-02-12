/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cassert>
#include <cstdarg>
#include "Platform/FailureHandling/FileCorruptionException.h"
#include "Platform/FailureHandling/FileAccessException.h"
#include "Platform/FailureHandling/FileVersionException.h"
#include "Platform/Storage/File.h"
#include "Platform/Utilities/HelperFunctions.h"

using namespace FailureHandling;
using namespace Storage;
using namespace std;
using namespace Utilities;

const uint32 File::INVALID_VERSION = (uint32) - 1;

#ifdef _DEBUG
	uint32 File::sOpenFilesCount = 0;
#endif // _DEBUG

void File::loadTextFile(string &fileContent, const Path &fileName)
{
	char buffer[File::READING_BUFFER_SIZE];
	File file(fileName, File::OPEN_READING, false);

	while (file.hasLeftData())
	{
		const uint32 charCount = file.read(buffer, File::READING_BUFFER_SIZE, sizeof(char), File::READING_BUFFER_SIZE);
		fileContent.append(buffer, charCount);
	}	
}

File::File(const Path &fileName, const File::FileMode mode, const bool binaryFile, const uint32 fileVersion) :
	mName(fileName)
{
	assert(!fileName.getString().empty());
	FILE *file = NULL;

	// create file according to the chosen mode
	switch (mode)
	{
		case OPEN_READING:
		{
			file = openFile(fileName, binaryFile ? "rb" : "r");
			break;
		}

		case OPEN_READING_AND_WRITING:
		{
			file = openFile(fileName, binaryFile ? "rb+" : "r+");
			break;
		}

		case CREATE_WRITING:
		{
			file = openFile(fileName, binaryFile ? "wb" : "w");
			break;
		}

		case CREATE_READING_AND_WRITING:
		{
			file = openFile(fileName, binaryFile ? "wb+" : "w+");
			break;
		}

		case APPEND_WRITING:
		{
			file = openFile(fileName, binaryFile ? "ab" : "a");
			break;
		}

		case APPEND_READING_AND_WRITING:
		{
			file = openFile(fileName, binaryFile ? "ab+" : "a+");
			break;
		}

		default:
			assert(false);
	}

	// error checking
	const char *text = "Could not open, create or modify a file.";
	if (NULL == file)
		throw FileAccessException(text, mName, -1);

	int32 errorNumber = ferror(file);
	clearerr(file);
	if (errorNumber)
	{
		fclose(file);
		throw FileAccessException(text, mName, errorNumber);
	}

	// set handle & update counter
	mHandle = file;
	#ifdef _DEBUG
		++sOpenFilesCount;
	#endif // _DEBUG

	// Does file versioning matter?
	if (INVALID_VERSION == fileVersion)
		return;
	
	// file is opened: check expected file version
	if (mode == OPEN_READING || mode == OPEN_READING_AND_WRITING)
	{
		uint32 actualFileVersion;
		read(&actualFileVersion, sizeof(uint32), sizeof(uint32), 1);
		if (fileVersion != actualFileVersion)
			throw FileVersionException("File version mismatch during loading.", mName, fileVersion, actualFileVersion);
		return;
	}

	if (mode == APPEND_READING_AND_WRITING || mode == APPEND_WRITING)
	{
		// open the file at the beginning for only file version checking - see above
		File file(fileName, OPEN_READING, binaryFile, fileVersion);
		return;
	}

	// save file version for new file
	assert(mode == CREATE_WRITING || mode == CREATE_READING_AND_WRITING);
	write(&fileVersion, sizeof(uint32), 1);
}

File::~File()
{
	int returnValue = fclose(mHandle);
	mHandle			= NULL;

	#ifdef _DEBUG
		--sOpenFilesCount;

		assert(sOpenFilesCount >= 0);
		assert(0 == returnValue);
	#endif // _DEBUG
}

bool File::endOfFileReached() const
{
	return (0 != feof(mHandle));
}

bool File::errorOccured() const
{
	return (0 != ferror(mHandle));
}

bool File::hasLeftData() const
{
	return !errorOccured() && !endOfFileReached();
}

FILE *File::openFile(const Path &fileName, const char *mode)
{
	#ifdef _WINDOWS
		FILE *file = NULL;
		fopen_s(&file, fileName.getCString(), mode);
		return file;
	#else
		return fopen(fileName.getCString(), mode);
	#endif // _WINDOWS
}

uint32 File::read(void *buffer, const uint64 bufferSize, const uint32 elementSize, uint64 elementCount)
{
	#ifdef _WINDOWS
		return (uint32) fread_s(buffer, bufferSize, elementSize, elementCount, mHandle);
	#else
		uint32 byteCount = elementSize * elementCount;
		if (bufferSize < byteCount)
		{
			// todo log this: invalid buffer size
			elementCount = bufferSize / elementSize;
		}

		return fread(buffer, elementSize, elementCount, mHandle);
	#endif // _WINDOWS
}


uint64 File::read(vector<uint8> &target, const uint32 chunkSize)
{
	uint64 readBytes = 0;
	while (hasLeftData())
	{
		target.resize(target.size() + chunkSize);
		readBytes += read(target.data() + readBytes, chunkSize, sizeof(uint8), chunkSize);
		target.resize(readBytes);
	}
	
	return readBytes;
}


uint8 File::readByte(const Encoding encoding)
{
	uint8 data;

	switch (encoding)
	{
		case ENCODING_ASCII:
		{
			uint32 temp;
			scanf("%d", &temp);

			if (temp > 255)
				throw FileCorruptionException("ASCII encoded byte to be read is larger than 255", mName);
			else
				data = temp;
			break;
		}

		case ENCODING_BINARY_BIG_ENDIAN:
		case ENCODING_BINARY_LITTLE_ENDIAN:
			read(&data, sizeof(uint8), sizeof(uint8), 1);
			break;

		default:
			assert(false);
	}

	return data;
}

double File::readDouble(const Encoding encoding)
{
	double data;

	switch (encoding)
	{
		case ENCODING_ASCII:
		{
			scanf("%lf", &data);
			break;
		}
			
		case ENCODING_BINARY_LITTLE_ENDIAN:
		case ENCODING_BINARY_BIG_ENDIAN:
		{
			// read & convert the double value
			int64 temp;
			read(&temp, sizeof(double), sizeof(double), 1);
			if (ENCODING_BINARY_LITTLE_ENDIAN == encoding)
				temp = littleEndian64ToHostOrder(temp);
			else
				temp = bigEndian64ToHostOrder(temp);
			
			// interpret read data as double
			double *tempAsDouble = reinterpret_cast<double *>(&temp);
			data = *tempAsDouble;
			break;
		}

		default:
			assert(false);
	}

	return data;
}

float File::readFloat(const Encoding encoding)
{
	float data;

	switch (encoding)
	{
		case ENCODING_ASCII:
			scanf("%f", &data);
			break;
			
		case ENCODING_BINARY_LITTLE_ENDIAN:
		case ENCODING_BINARY_BIG_ENDIAN:
		{
			// read & convert the float value
			int32 temp;
			read(&temp, sizeof(float), sizeof(float), 1);
			if (ENCODING_BINARY_LITTLE_ENDIAN == encoding)
				temp = littleEndian32ToHostOrder(temp);
			else
				temp = bigEndian32ToHostOrder(temp);
			
			// interpret read data as float
			float *tempAsFloat = reinterpret_cast<float *>(&temp);
			data = *tempAsFloat;
			break;
		}

		default:
			assert(false);
	}

	return data;
}

int8 File::readInt8(const Encoding encoding)
{
	int8 data;

	switch (encoding)
	{
		case ENCODING_ASCII:
		{
			int32 temp;
			scanf("%d", &temp);
			data = (int8) temp;
			break;
		}

		case ENCODING_BINARY_BIG_ENDIAN:
		case ENCODING_BINARY_LITTLE_ENDIAN:
		{
			read(&data, sizeof(int8), sizeof(int8), 1);
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return data;
}

int16 File::readInt16(const Encoding encoding)
{
	int16 data;

	switch (encoding)
	{
		case ENCODING_ASCII:
			scanf("%d", &data);
			break;

		case ENCODING_BINARY_BIG_ENDIAN:
			read(&data, sizeof(int16), sizeof(int16), 1);
			data = bigEndian16ToHostOrder(data);
			break;

		case ENCODING_BINARY_LITTLE_ENDIAN:
			read(&data, sizeof(int16), sizeof(int16), 1);
			data = littleEndian16ToHostOrder(data);
			break;

		default:
			assert(false);
	}

	return data;
}

int32 File::readInt32(const Encoding encoding)
{
	int32 data;

	switch (encoding)
	{
		case ENCODING_ASCII:
			scanf("%d", &data);
			break;

		case ENCODING_BINARY_BIG_ENDIAN:
			read(&data, sizeof(int32), sizeof(int32), 1);
			data = bigEndian32ToHostOrder(data);
			break;

		case ENCODING_BINARY_LITTLE_ENDIAN:
			read(&data, sizeof(int32), sizeof(int32), 1);
			data = littleEndian32ToHostOrder(data);
			break;

		default:
			assert(false);
	}

	return data;
}

int64 File::readInt64(const Encoding encoding)
{
	int64 data;

	switch (encoding)
	{
		case ENCODING_ASCII:
			scanf("%d", &data);
			break;

		case ENCODING_BINARY_BIG_ENDIAN:
			read(&data, sizeof(int64), sizeof(int64), 1);
			data = bigEndian64ToHostOrder(data);
			break;

		case ENCODING_BINARY_LITTLE_ENDIAN:
			read(&data, sizeof(int64), sizeof(int64), 1);
			data = littleEndian64ToHostOrder(data);
			break;

		default:
			assert(false);
	}

	return data;
}

bool File::readString(std::string &target, const Encoding encoding)
{
	const uint32 numberOfCharacters = readInt32(encoding);
	target.resize(numberOfCharacters);

	for (uint32 i = 0; i < numberOfCharacters; ++i)
		target[i] = readByte(encoding);

	return !errorOccured();
}

bool File::readTextLine(string &target)
{
	char *temp = fgets(mBuffer, READING_BUFFER_SIZE, mHandle);
	target = mBuffer;

	return (NULL != temp);
}

bool File::readTextLine(vector<string> &lineParts, string &textLine, const char *delimeters)
{
	// try to read a line
	bool success = readTextLine(textLine);

	// return empty thingies on failure
	if (!success)
	{
		lineParts.clear();
		textLine.clear();
		return false;
	}

	// split the line
	Utilities::split(lineParts, textLine, delimeters);
	return success;
}

uint8 File::readUInt8(const Encoding encoding)
{
	uint8 data;

	switch (encoding)
	{
		case ENCODING_ASCII:
		{
			int32 temp;
			scanf("%d", &temp);
			data = (uint8) temp;
			break;
		}

		case ENCODING_BINARY_BIG_ENDIAN:
		case ENCODING_BINARY_LITTLE_ENDIAN:
		{
			read(&data, sizeof(uint8), sizeof(uint8), 1);
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return data;
}

uint16 File::readUInt16(const Encoding encoding)
{
	uint16 data;

	switch (encoding)
	{
		case ENCODING_ASCII:
			scanf("%d", &data);
			break;

		case ENCODING_BINARY_BIG_ENDIAN:
			read(&data, sizeof(uint16), sizeof(uint16), 1);
			data = bigEndian16ToHostOrder(data);
			break;

		case ENCODING_BINARY_LITTLE_ENDIAN:
			read(&data, sizeof(uint16), sizeof(uint16), 1);
			data = littleEndian16ToHostOrder(data);
			break;

		default:
			assert(false);
	}

	return data;
}

uint32 File::readUInt32(const Encoding encoding)
{
	uint32 data;

	switch (encoding)
	{
		case ENCODING_ASCII:
			scanf("%d", &data);
			break;

		case ENCODING_BINARY_BIG_ENDIAN:
			read(&data, sizeof(uint32), sizeof(uint32), 1);
			data = bigEndian32ToHostOrder(data);
			break;

		case ENCODING_BINARY_LITTLE_ENDIAN:
			read(&data, sizeof(uint32), sizeof(uint32), 1);
			data = littleEndian32ToHostOrder(data);
			break;

		default:
			assert(false);
	}

	return data;
}

uint64 File::readUInt64(const Encoding encoding)
{
	uint64 data;

	switch (encoding)
	{
		case ENCODING_ASCII:
			scanf("%d", &data);
			break;

		case ENCODING_BINARY_BIG_ENDIAN:
			read(&data, sizeof(uint64), sizeof(uint64), 1);
			data = bigEndian64ToHostOrder(data);
			break;

		case ENCODING_BINARY_LITTLE_ENDIAN:
			read(&data, sizeof(uint64), sizeof(uint64), 1);
			data = littleEndian64ToHostOrder(data);
			break;

		default:
			assert(false);
	}

	return data;
}

int32 File::scanf(const char *format, ...)
{
	int32	readArgumentsCount = 0;
	va_list	arguments;

	va_start(arguments, format);
		#ifdef _WINDOWS
			readArgumentsCount = vfscanf_s(mHandle, format, arguments);
		#else
			readArgumentsCount = vfscanf(mHandle, format, arguments);
		#endif // _WINDOWS
	va_end(arguments);

	return readArgumentsCount;
}

int32 File::scanfString(const char *formatBegin, const char *formatEnd, char *buffer, uint32 bufferSize)
{
	if (0 == bufferSize)
		return 0;

	// build format string
	string format = formatBegin;
	#ifdef _WINDOWS
		format += "%s";
	#else
		char temp[1000];
		snprintf(temp, 1000, "%%%ds", bufferSize);
		format += temp;
	#endif // _WINDOWS
	format += formatEnd;

	#ifdef _WINDOWS
		return fscanf_s(mHandle, format.c_str(), buffer, bufferSize - 1);
	#else
		return fscanf(mHandle, format.c_str(), buffer);
	#endif // _WINDOWS
}

uint32 File::write(const void *data, const uint32 elementSize, const uint64 elementCount)
{
	return (uint32) fwrite(data, elementSize, elementCount, mHandle);
}

bool File::writeReal(const Real value, const Encoding encoding)
{
	#ifdef DOUBLE_PRECISION
		return writeDouble(value, encoding);
	#else
		return writeFloat(value, encoding);
	#endif
}

bool File::writeDouble(const double value, const Encoding encoding)
{
	switch (encoding)
	{
		case ENCODING_ASCII:
		{
			return (0 < fprintf(mHandle, "%f", value));
		}

		case ENCODING_BINARY_LITTLE_ENDIAN:
		case ENCODING_BINARY_BIG_ENDIAN:
		{
			double temp = value;
			int64 *tempAsInt = reinterpret_cast<int64 *>(&temp);
			
			if (ENCODING_BINARY_LITTLE_ENDIAN == encoding)
				*tempAsInt = hostOrderToLittleEndian64(*tempAsInt);
			else
				*tempAsInt = hostOrderToBigEndian64(*tempAsInt);
				
			return (1 == write(&temp, sizeof(double), 1));
		}

		default:
		{
			assert(false);
			throw Exception("Unsupported encoding for double writing.");
		}
	}	
}

bool File::writeFloat(const float value, const Encoding encoding)
{
	switch (encoding)
	{
		case ENCODING_ASCII:
		{
			return (0 < fprintf(mHandle, "%f", value));
		}

		case ENCODING_BINARY_LITTLE_ENDIAN:
		case ENCODING_BINARY_BIG_ENDIAN:
		{
			float temp = value;
			int32 *tempAsInt = reinterpret_cast<int32 *>(&temp);
			
			if (ENCODING_BINARY_LITTLE_ENDIAN == encoding)
				*tempAsInt = hostOrderToLittleEndian32(*tempAsInt);
			else
				*tempAsInt = hostOrderToBigEndian32(*tempAsInt);
				
			return (1 == write(&temp, sizeof(float), 1));
		}

		default:
		{
			assert(false);
			throw Exception("Unsupported encoding for float writing.");
		}
	}	
}

bool File::writeInt32(const int32 integer, const Encoding encoding)
{
	switch (encoding)
	{
		case ENCODING_ASCII:
		{
			return (0 < fprintf(mHandle, "%d", integer));
		}

		case ENCODING_BINARY_LITTLE_ENDIAN:
		{
			const int32 temp = hostOrderToLittleEndian32(integer);
			return (1 == write(&temp, sizeof(uint32), 1));
		}

		case ENCODING_BINARY_BIG_ENDIAN:
		{
			const int32 temp = hostOrderToBigEndian32(integer);
			return (1 == write(&temp, sizeof(uint32), 1));
		}

		default:
		{
			assert(false);
			throw Exception("Unsupported encoding for int32 writing.");
		}
	}
}

int32 File::writeString(const std::string &source, const Encoding encoding)
{
	const uint32 numberOfCharacters = (uint32) source.size();
	if (0 == writeInt32(numberOfCharacters, encoding))
		return -1;
	
	const char *chars = source.data();
	switch (encoding)
	{
		case ENCODING_ASCII:
			return fprintf(mHandle, "%s", chars);

		case ENCODING_BINARY_LITTLE_ENDIAN:
		case ENCODING_BINARY_BIG_ENDIAN:
			return write(chars, sizeof(uint8), numberOfCharacters);

		default:
		{
			assert(false);
			throw Exception("Unsupported encoding for string writing.");
		}
	}

	return -1;
}

bool File::writeUInt8(const uint8 integer, const Encoding encoding)
{
	
	switch (encoding)
	{
		case ENCODING_ASCII:
		{
			return (0 < fprintf(mHandle, "%u", integer));
		}

		case ENCODING_BINARY_LITTLE_ENDIAN:
		case ENCODING_BINARY_BIG_ENDIAN:
		{
			return (1 == write(&integer, sizeof(uint8), 1));
		}

		default:
		{
			assert(false);
			throw Exception("Unsupported encoding for uint8 writing.");
		}
	}
}
