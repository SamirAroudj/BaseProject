/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _AUDIO_EXCEPTION_H_
#define _AUDIO_EXCEPTION_H_

#include "Exception.h"

namespace FailureHandling
{
    /** This exception should be thrown when a problem concerning Audio code from the Audio namespace occurs. */
	class AudioException : public Exception
	{
	public:
        /** Creates an AudioException object with a specific message for debugging and logging.
        @param Set this to message which describes the occured problem to enable reasonable logging and debugging. */
		AudioException(const std::string &message);
	};

    /** AudioException object's message and source can be printed to a console window.
    @param os This is the desired output stream, eg cout.
    @param exception This is the exception the message and source of which is printed. */
	std::ostream &operator <<(std::ostream &os, const AudioException &exception);
}

#endif // AUDIO_EXCEPTION_H_
