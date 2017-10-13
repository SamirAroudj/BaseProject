/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "InputException.h"

using namespace FailureHandling;

InputException::InputException(const std::string &message, const int32 errorCode) :
	Exception(message), mErrorCode(errorCode)
{
}