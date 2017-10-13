/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _FILE_CORRUPTION_EXCEPTION_H_
#define _FILE_CORRUPTION_EXCEPTION_H_

#include "Platform/FailureHandling/FileException.h"

namespace FailureHandling
{
	/// An object of this class should be thrown if there is a file the data of which does not meet the structure format / requirements of the processing code.
    /** This exception contains information about a file the data of which is corrupted and cannot be handled.
        That means that it's structure / format does not meet the application's requirements / code.
        Therefore this exception should be thrown if there is a corrupted file. */
	class FileCorruptionException : public FileException
	{
	public:
		/** Create an exception which describes which file is corrupted and contains information about related circumstances.
		@param message The message string contains the reason or circumstances while this exception was thrown.
		@param fileName This parameter should contain the path and name of the file which is corrupted. */
		FileCorruptionException(const std::string &message, const Storage::Path &fileName);
	};
	
	/** Exception's message, source etc. can be printed to a console window.
	@param os This is the desired output stream, eg cout.
    @param exception This is the exception the message and source of which is printed. */
	std::ostream &operator <<(std::ostream &os, const FileCorruptionException &exception);
}

#endif // _FILE_CORRUPTION_EXCEPTION_H_

