/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "ClosedConnectionException.h"

using namespace FailureHandling;

ClosedConnectionException::ClosedConnectionException(const std::string &message, int32 socketErrorCode) : NetworkException(message, socketErrorCode)
{

}
