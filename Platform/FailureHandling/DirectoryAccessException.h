/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _DIRECTORY_ACCESS_EXCEPTION_H_
#define _DIRECTORY_ACCESS_EXCEPTION_H_

#include <string>
#include "Platform/DataTypes.h"
#include "Platform/FailureHandling/FileException.h"

namespace FailureHandling
{
	/// This exception is thrown if a directory could not be opened for some reason or another.
	/** The exception contains a description of the situation and the name of the directory which could not be opened. */
	class DirectoryAccessException : public FileException
	{
	public:
		/** Constructs an exception that should be thrown / catched if a directory could not be opened for some reason.
		@param message Description of the situation, might contain the reason for the failed opening operation.
		@param directoryName Contains the name and path of the directory the data of which could not be accessed.
		@param errorCode Should contain an error code created due to the used directory access function, such as opendir. */
		DirectoryAccessException(const std::string &message, const Storage::Path &directoryName, int32 errorCode);

		/** Normal destructor */
		~DirectoryAccessException();

		/** Returns an error code which is related to the problem while accessing the directory.
		@returns Should return an error code which in turn was returned by the used directory access function, such as opendir. */
		inline int32 getErrorCode() const;

	private:
		const int32 mErrorCode; /// contains the error code returned by the used error function
	};
	
	/** Exception's message and source can be printed to a console window.
	@param os This is the desired output stream, eg cout.
	@param exception This is the DirectoryAccessException the message, directory name and source of which is printed.*/
	std::ostream &operator <<(std::ostream &os, const DirectoryAccessException &exception);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int32 DirectoryAccessException::getErrorCode() const
	{
		return mErrorCode;
	}
}

#endif // _DIRECTORY_ACCESS_EXCEPTION_H_

 
