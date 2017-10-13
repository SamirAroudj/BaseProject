/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _EXCEPTION_HANDLING_H_
#define _EXCEPTION_HANDLING_H_

#include <iostream>
#include "Platform/DataTypes.h"
#include "Exception.h"

namespace FailureHandling
{
	/** Exception class for signalling graphics errors. */
	class GraphicsException : public Exception
	{
	public:
		/** Creates an exception and stores some information, such as the reason and an error code for identification.
		@param message The message should describe why this exception is created.
		@param errorCode The error code should be retrieved by the underlying graphics API and identify the particular problem.*/
		GraphicsException(const std::string &message, int32 errorCode) : Exception(message), mErrorCode(errorCode) { }

		/** Gets the error code identifying the particualr problem.
		@return The returned error code identifies the exception and is usually retrieved by the underlying graphics API. */
		int32 getErrorCode() const { return mErrorCode; }

	private:
		int32 mErrorCode; /// The error code identifies the particular problem and should be retrieved by the underlying graphics API.
	};

	/** Exception's message, error code and source can be printed to a stream.
	@param os This is the desired output stream, eg cout.
	@param exception This is the exception the message and source of which is printed.
	@return Returns the outputstream with the exception description.*/
	std::ostream &operator<<(std::ostream &os, const GraphicsException &exception);
}

#endif // _EXCEPTION_HANDLING_H_
