/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _INPUT_EXCPETION_H_
#define _INPUT_EXCEPTION_H_

#include "Platform/DataTypes.h"
#include "Exception.h"

namespace FailureHandling
{
	/** Exception for signalling problems with regard to input devices, such as failed initialization of Direct Input. */
	class InputException : public Exception
	{
	public:
		/** Creates an exceptions which describes a problem regarding input management.
		@param message Describes the reason and or circumstances why this exception is created.
		@param errorCode This is an error code identifying the problem, e.g. a return value of a Direct Input function call.*/
		InputException(const std::string &message, const int32 errorCode);

		/** Obtains the error code identifying the particular problem.
		@return For example, the error code could be a return value of a Direct Input function call. */
		int32 getErrorCode() const { return mErrorCode; }

	private:
		int32 mErrorCode; /// The error code identifying the problem, e.g. return value of a Direct Input function call.
	};
}

#endif // _INPUT_EXCEPTION_H_
