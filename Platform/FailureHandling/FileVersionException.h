/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _FILE_VERSION_EXCEPTION_H_
#define _FILE_VERSION_EXCEPTION_H_

#include "Platform/DataTypes.h"
#include "Platform/FailureHandling/FileException.h"

namespace FailureHandling
{
	/// An object of this class should be thrown if there is a file for which the loading code version is not supposed to as either the code or file are outdated.
    /** This exception contains information about a file the data of which cannot be loaded due to a version missmatch between the file and the loading code.
        That means that the content of the file does not fit to the code implementation.
        Therefore this exception should be thrown to show that the file cannot be loaded.
		Either the file should be converted to match the new version or the code must be updated to be able to handle the file format. */
	class FileVersionException : public FileException
	{
	public:
		/** Create an exception which describes which file cannot be handled and also contains the code version.
		@param message The message string contains the reason or circumstances while this exception was thrown.
		@param fileName This parameter should contain the path and name of the file which cannot be handled.
		@param fileVersion Set this to the version of the file which identifies its format and the version of the code with which it was created.
		@param codeVersion Set this to the version of the code which could not process the file since the code is too new or too old. */
		FileVersionException(const std::string &message, const Storage::Path &fileName, const uint32 codeVersion, const uint32 fileVersion);

		/** Returns the version of the file which identifies its format and the version of the code with which it was created.
		@return Returns the version of the file which identifies its format and the version of the code with which it was created. */
		inline uint32 getCodeVersion() const;

		/** Returns the version of the code which could not process the problematic file since the code is too new or too old.
		@return Returns the version of the code which could not process the problematic file since the code is too new or too old.*/
		inline uint32 getFileVersion() const;

	protected:
		const uint32 mCodeVersion;	/// version of the code which cannot handle the file
		const uint32 mFileVersion;	/// version of the file = version of the code which was used to create the file
	};

	
	/** Exception's message, source etc. can be printed to a console window.
	@param os This is the desired output stream, eg cout.
    @param exception This is the exception the message and source of which is printed. */
	std::ostream &operator <<(std::ostream &os, const FileVersionException &exception);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline uint32 FileVersionException::getCodeVersion() const
	{
		return mCodeVersion;
	}
		
	inline uint32 FileVersionException::getFileVersion() const
	{
		return mFileVersion;
	}
}

#endif // _FILE_VERSION_EXCEPTION_H_

