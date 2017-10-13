/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _WINDOW_EXCEPTION_H_
#define _WINDOW_EXCEPTION_H_

#include "Exception.h"

namespace FailureHandling
{
	/// This class should be used for exceptions regarding to the main window.
	/** Reasons to throw this window are for example failing window creation or impossible window description registrations. */
	class WindowException : public Exception
	{
	public:
		/**	Creates an window exception and stores some information about the reason why this exception is created.
        @param message This variable should contain the reason for the exception creation. */
		WindowException(const std::string &message);

	private:
	};
}

#endif // _WINDOW_EXCEPTION_H_
