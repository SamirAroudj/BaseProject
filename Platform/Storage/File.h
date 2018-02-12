/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _STORAGE_FILE_H_
#define _STORAGE_FILE_H_

#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include "Platform/DataTypes.h"
#include "Platform/Storage/Path.h"

namespace Storage
{
	/// This class should be used for opening and closing of every file
	/** The class is used to open or close file needed by the class user. It provides an operating system and device independent access to files.*/
	class File
	{
	public:
		/// Possible ways to open or create a file. The values correspond to the fopen modes.
		enum FileMode
		{
            OPEN_READING,               /// Open an existing file for reading only.
            OPEN_READING_AND_WRITING,   /// Open an existing file for reading and writing.
            CREATE_WRITING,             /// Create a file and only write to it.
            CREATE_READING_AND_WRITING, /// Create a file and read from it and write to it.
            APPEND_WRITING,             /// Open a file to only write data at its end.
            APPEND_READING_AND_WRITING  /// Open a file for reading and writing at its end.
		};

	public:
		/** Loads the complete content of a text (not binary) file and stores it in fileContent.
		@param fileContent This string will be filled with the complete text of the specified file.
		@param fileName Specifies the file from which the text is loaded. */
		static void loadTextFile(std::string &fileContent, const Storage::Path &fileName);

    public:
		/** Opens or creates a specific file.
		@param fileName The name and path of the file the access of which is requested.
		@param mode Defines the way the file is used, corresponds to the file mode options of fopen. See FileMode.
		@param binaryFile If binaryFile is true than the file is opened as binary file
						  otherwise it is opened as text file (some characters are treated in a special way.)
		@param fileVersion Use this to manage file formats.
			If a file is opened then this is the expected file version and an exception is thrown if the expected file version does not match the version in the opened file.
			If a file is created then this version is written into the very beginning of the file directly after creation.
			Set it to INVALID_VERSION to ignore it and open or create the file without regarding any version. */
		File(const Path &fileName, const FileMode mode, const bool binaryFile, const uint32 fileVersion = File::INVALID_VERSION);

		/** Releases the file handle. */
		~File();

		#ifdef _DEBUG
			/** Queries how many files are currently open / exist.
			@return Returns the number of currently existing / opened files. */
			static uint32 getOpenFilesCount() { return sOpenFilesCount; }
		#endif // _DEBUG

		/** Returns true if the end of the file was reached.
		@return Returns true if the end of the file was reached. */
		bool endOfFileReached() const;

		/** Returns true if a file error occured.
		@return Returns true if a file error occured. */
		bool errorOccured() const;

		/** Returns true if the end of the file hasn't been reached so far and if no error was encountered.
		@return Returns true if the end of the file hasn't been reached so far and if no error was encountered.*/
		bool hasLeftData() const;

		// TODO this should be removed
		/** Provides access to the file handle.
		@return A reference to the file handle is returned. */
		FILE &getHandle() { return *mHandle; }

		/** Returns the path and local name used to create the file.
		@return Returns the Path object identifying where the file is stored.*/
		inline const Path &getName() const;

		/** Reads data (elementCount blocks of size elementSize) from the file represented by this object.
		@param buffer Data is written to this buffer which should have a size of at least bufferSize >= elementSize * elementCount.
		@param bufferSize Specifies the size of the array buffer in bytes.
		@param elementSize Defines how large a single element to be read is. Unit: bytes
		@param elementCount Defines how many elements are supposed to be read.
		@return The number of completely read elementSize byte blocks is returned.
				The return value is smaller than elementCount if the file end is reached or
				if an error occurs before elementSize * elementCount bytes are read.*/
		uint32 read(void *buffer, const uint64 bufferSize, const uint32 elementSize, uint64 elementCount);

		/** Reads and appends data to target until there is no data left to be read.
		Appends all the data that was read from the file reading position before this call until all following data was read.
		@param target Remaining file data is apended to target.
		@param chunkSize Defines the size of chunks (in bytes) to be reserved and read in one call.
		@return Returns the number of bytes that have been read. */
		uint64 read(std::vector<uint8> &target, const uint32 chunkSize = File::READING_BUFFER_SIZE);
		
		/** Reads an 8 Bit integer from the file whereas it is considered to have the entered encoding.
		@param encoding Set this to the encoding of the data to be read to ensure right interpretation of the read file bits.
		@return Returns the read bits interpreted as uint8 whereas the file bits were considered to be encoded with the specified encoding. */
		uint8 readByte(const Encoding encoding);

		/** Reads a 64 Bit double from the file whereas it is considered to have the entered encoding.
		@param encoding Set this to the encoding of the data to be read to ensure right interpretation of the read file bits.
		@return Returns the read bits interpreted as double whereas the file bits were considered to be encoded with the specified encoding. */
		double readDouble(const Encoding encoding);

		/** Reads a 32 Bit float from the file whereas it is considered to have the entered encoding.
		@param encoding Set this to the encoding of the data to be read to ensure right interpretation of the read file bits.
		@return Returns the read bits interpreted as float whereas the file bits were considered to be encoded with the specified encoding. */
		float readFloat(const Encoding encoding);

		/** Reads an 8 Bit integer from the file whereas it is considered to have the entered encoding.
		@param encoding Set this to the encoding of the data to be read to ensure right interpretation of the read file bits.
		@return Returns the read bits interpreted as int8 whereas the file bits were considered to be encoded with the specified encoding. */
		int8 readInt8(const Encoding encoding);

		/** Reads a 16 Bit integer from the file whereas it is considered to have the entered encoding.
		@param encoding Set this to the encoding of the data to be read to ensure right interpretation of the read file bits.
		@return Returns the read bits interpreted as int16 whereas the file bits were considered to be encoded with the specified encoding. */
		int16 readInt16(const Encoding encoding);

		/** Reads a 32 Bit integer from the file whereas it is considered to have the entered encoding.
		@param encoding Set this to the encoding of the data to be read to ensure right interpretation of the read file bits.
		@return Returns the read bits interpreted as int32 whereas the file bits were considered to be encoded with the specified encoding. */
		int32 readInt32(const Encoding encoding);

		/** Reads a 64 Bit integer from the file whereas it is considered to have the entered encoding.
		@param encoding Set this to the encoding of the data to be read to ensure right interpretation of the read file bits.
		@return Returns the read bits interpreted as int64 whereas the file bits were considered to be encoded with the specified encoding. */
		int64 readInt64(const Encoding encoding);

		/** Reads a string whith the following format:
			data = [uint32 numberOfByteCharacters] [uint8 char_0] [uint8 char_1] ... [uint8 char_numberOfByteCharacters-1]. 
		@param target The result is stored in target.
		@param encoding Set this to the encoding of the data to be read to ensure right interpretation of the read file bits.
		@return True is returned if a string was successfully read otherwise false is returned. */
		bool readString(std::string &target, const Encoding encoding);

		/** Reads characters and writes them into target until a new line character was read or FILE_READING_BUFFER_SIZE characters were written to target.
		@param target Reads characters until a new line character was found or until FILE_READING_BUFFER_SIZE characters were read and
			stores them in target.
		@return Returns true if any character was successefully read and written to target. */
		bool readTextLine(std::string &target);

		/** Reads characters and writes them into target until a new line character was read or FILE_READING_BUFFER_SIZE characters were written to target.
			This read text line is then split accordint to delimeters and the resulting parts are stored in lineParts.
		@param lineParts Contains substrings of textLine whereas delimeters define where to split the text line. Characters in delimeters won't be written to lineParts.
		@param target Reads characters until a new line character was found or until FILE_READING_BUFFER_SIZE characters were read and
			stores them in target.
		@param delimeters The text line is split for any of the delimeter characters.
		@return Returns true if any character was successefully read and written to target. */
		bool readTextLine(std::vector<std::string> &lineParts, std::string &textLine, const char *delimeters);
	
		/** Reads an unsigned 8 Bit integer from the file whereas it is considered to have the entered encoding.
		@param encoding Set this to the encoding of the data to be read to ensure right interpretation of the read file bits.
		@return Returns the read bits interpreted as int8 whereas the file bits were considered to be encoded with the specified encoding. */
		uint8 readUInt8(const Encoding encoding);

		/** Reads an unsigned 16 Bit integer from the file whereas it is considered to have the entered encoding.
		@param encoding Set this to the encoding of the data to be read to ensure right interpretation of the read file bits.
		@return Returns the read bits interpreted as int16 whereas the file bits were considered to be encoded with the specified encoding. */
		uint16 readUInt16(const Encoding encoding);

		/** Reads an unsigned 32 Bit integer from the file whereas it is considered to have the entered encoding.
		@param encoding Set this to the encoding of the data to be read to ensure right interpretation of the read file bits.
		@return Returns the read bits interpreted as int32 whereas the file bits were considered to be encoded with the specified encoding. */
		uint32 readUInt32(const Encoding encoding);

		/** Reads an unsigned 64 Bit integer from the file whereas it is considered to have the entered encoding.
		@param encoding Set this to the encoding of the data to be read to ensure right interpretation of the read file bits.
		@return Returns the read bits interpreted as int64 whereas the file bits were considered to be encoded with the specified encoding. */
		uint64 readUInt64(const Encoding encoding);

		/** Executes scanf to read data in a specific format.
		@param format This is the usual scanf format string. See standard scanf implementation for more info.
		@param ... This is the usual scanf variable arguments list. See standard scanf implementation for more info.
		@return Returns the number of successfully read arguments or a negative value if
			read failure occurs before the first receiving argument was assigned.*/
		int32 scanf(const char *format, ...);

		/** Executes scanf to extract a string between two strings by means of the automatically created format string:\n
			"formatBegin%s(bufferSize - 1)formatEnd"\n
			"%s" is automatically inserted.
			This means that a string between formatBegin and formatEnd is searched which is allowed to have a length of bufferSize - 1.
		@param formatBegin Set this to the characters which shall occur before the string which is extracted.
		@param formatEnd Set this to the characters which shall occur after the string which is extracted.
		@param buffer Set this to the buffer which is filled with the extracted string.
		@param bufferSize Set this to the maximum number of characters which can be written into buffer.\n
			(Don't forget that '\0' is always appended as last character to terminate the string. )
		@return Returns one on success otherwise zero is returned. (The number of read strings is returned.)*/
		int32 scanfString(const char *formatBegin, const char *formatEnd, char *buffer, uint32 bufferSize);

		/** Writes bits and bytes read from the memory area called data to this file whereas the writing is done element per element like in fwrite.
		@param data Bytes starting at the memory adress this pointer refers to are written to the file.
		@param elementSize Set this to the size of a single element in the memory area in bytes. 
		@param elementCount Set this to the number of elements which are stored in the memory area to which data refers to.
		@return Returns the number of elements which were successfully written to the file which can be fewer than elementCount due to possible errors.*/
		uint32 write(const void *data, const uint32 elementSize, const uint64 elementCount);
		
		/** Writes a 64 bit floating point number in host order to the file with the specified encoding.
		@param value Set this to the host order floating point number you want to convert if necessary and write to this file.
		@param encoding Specifies how the entered floating point number is represented in the file. E.g. it might be converted to big endian format.
		@return Returns true if the floating point number was successfully written. */
		bool writeDouble(const double value, const Encoding encoding);
		
		/** Writes a 32 bit floating point number in host order to the file with the specified encoding.
		@param value Set this to the host order floating point number you want to convert if necessary and write to this file.
		@param encoding Specifies how the entered floating point number is represented in the file. E.g. it might be converted to big endian format.
		@return Returns true if the floating point number was successfully written. */
		bool writeFloat(const float value, const Encoding encoding);

		/** Writes an integer in host order to the file with the specified encoding.
		@param integer Set this to the host order integer you want to convert if necessary and write to this file.
		@param encoding Specifies how the entered integer is represented in the file. E.g. it might be converted to big endian format.
		@return Returns true if the integer was successfully written. */
		bool writeInt32(const int32 integer, const Encoding encoding);

		/** Writes a 32 or 64 floating point number (depends on the flag DOUBLE_PRECISION) in host order to the file with the specified encoding.
		@param value Set this to the host order floating point number you want to convert if necessary and write to this file.
		@param encoding Specifies how the entered floating point number is represented in the file. E.g. it might be converted to big endian format.
		@return Returns true if the floating point number was successfully written. */
		bool writeReal(const Real value, const Encoding encoding);

		/** Writes a string whith the following format:
			data = [uint32 numberOfByteCharacters] [uint8 char_0] [uint8 char_1] ... [uint8 char_numberOfByteCharacters-1]. 
		@param source This string is written to this file.
		@param encoding Set this to the encoding of the data to be read to ensure right interpretation of the read file bits. 
		@return Returns number of successfully written characters or a negative value if something went wrong. */
		int32 writeString(const std::string &source, const Encoding encoding);

		/** Writes an unsiged byte to the file with the specified encoding.
		@param integer Set this to the integer you want to convert if necessary and write to this file.
		@param encoding Specifies how the entered integer is represented in the file. E.g. it might be converted to ASCII format.
		@return Returns true if the integer was successfully written. */
		bool writeUInt8(const uint8 integer, const Encoding encoding);

	protected:
		/** Opens a file specified by the full fileName and a mode like "a", "a+" etc (see fopen or fopen_s)
		@param fileName The name and path of the file which is requested to be opened.
		@param mode see the documentation for fopen or fopen_s.
		@returns Returns the file handle which belongs to the opened file. */
		static FILE *openFile(const Path &fileName, const char *mode);

    private:
        /** Copy constructor is forbidden.
        @param copy Copy constructor is forbidden. */
        File(const File &copy) : mHandle(NULL) { assert(false); }

        /** Assignment operator is forbidden.
        @param rhs Operator is forbidden.*/
        File &operator =(const File &rhs) { assert(false); return *this; }

	public:
		/** This value should be the same as the maximum number of characters in a file line. */
		static const uint32 READING_BUFFER_SIZE = 1024u;
		static const uint32 INVALID_VERSION;

	private:
		#ifdef _DEBUG
			static uint32 sOpenFilesCount;	/// stores the number of currently opened file which is used to detect errors and forgotten files
		#endif // _DEBUG

		const Path mName;					/// file name, e.g., used for file exceptions for specific messages
        FILE *mHandle;						/// points to the file on mass storage
		char mBuffer[READING_BUFFER_SIZE];	/// temporary buffer for reading text
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	inline const Path &File::getName() const
	{
		return mName;
	}
}

#endif // _STORAGE_FILE_H_

