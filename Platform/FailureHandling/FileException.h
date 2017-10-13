/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _FILE_EXCEPTION_H_
#define _FILE_EXCEPTION_H_

#include "Exception.h"
#include "Platform/Storage/Path.h"

namespace FailureHandling
{
	/// An object of this class should be thrown if there is a file which cannot be processed for some reason.
    /** This exception contains information about a file which cannot be processed for some reason or another. 
		Constains a message to describe the circumstances and the file name. */
	class FileException : public Exception
	{
	public:
		/** Create an exception which describes which file cannot be handled with a short text.
		@param message The message string contains the reason or circumstances to describe why this exception is thrown.
		@param fileName This parameter should contain the path and name of the file which cannot be handled. */
		FileException(const std::string &message, const Storage::Path &fileName);

		/** Obtain the path to and name of the problematic file.
		@returns The function returns the path to and name of the file which is the source of this exception. */
		const Storage::Path &getFileName() const;

	private:
        const Storage::Path &mFileName; /// path and name of the problematic file
	};
	
	/** Exception's message, source etc. can be printed to a console window.
	@param os This is the desired output stream, eg cout.
    @param exception This is the exception the message and source of which is printed. */
	std::ostream &operator <<(std::ostream &os, const FileException &exception);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline const Storage::Path &FileException::getFileName() const
	{
		return mFileName;
	}
}

#endif // _FILE_EXCEPTION_H_

