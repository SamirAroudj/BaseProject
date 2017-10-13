/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "NetworkException.h"

using namespace FailureHandling;
using namespace std;

NetworkException::NetworkException(const std::string &message, int32 socketErrorCode) : Exception(message), mSocketErrorCode(socketErrorCode)
{

}

/**Creates a small output message and writes it to the ostream.
@returns The returned ostream can be used for further concatenation.
@param os This is the stream the message describing the exception will be written to.
@param exception The information contained by this exception will be written to the ostream. */	
std::ostream &FailureHandling::operator<<(ostream &os, const NetworkException &exception)
{
	os << "A network exception occured:\n" << exception.getMessage() << endl;
	os << "socket error code: " << exception.getSocketErrorCode() << endl;
	os << "Source: " << exception.getSource() << endl;
	return os;
}
