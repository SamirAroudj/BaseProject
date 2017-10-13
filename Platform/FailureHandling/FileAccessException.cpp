/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <iostream>
#include "Platform/FailureHandling/FileAccessException.h"

using namespace FailureHandling;
using namespace std;
using namespace Storage;

FileAccessException::FileAccessException(const string &message, const Path &fileName, int32 errorCode) : 
	FileException(message, fileName), mErrorCode(errorCode)
{

}

FileAccessException::~FileAccessException()
{

}

ostream &FailureHandling::operator <<(ostream &os, const FileAccessException &exception)
{
	cout << "An exception occured.\nA file could not be opened.\n";
	cout << exception.getMessage() << endl;
	cout << "File name: " << exception.getFileName() << endl;
	cout << "Error code: " << exception.getErrorCode() << endl;
	cout << "Exception source: " << exception.getSource() << endl;

	return os;
}
