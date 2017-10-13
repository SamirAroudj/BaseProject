/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <iostream>
#include "Platform/FailureHandling/DirectoryAccessException.h"

using namespace FailureHandling;
using namespace std;
using namespace Storage;

DirectoryAccessException::DirectoryAccessException(const string &message, const Path &directoryName, int32 errorCode) : 
	FileException(message, directoryName), mErrorCode(errorCode)
{

}

DirectoryAccessException::~DirectoryAccessException()
{

}

ostream &FailureHandling::operator <<(ostream &os, const DirectoryAccessException &exception)
{
	cout << "An exception occured.\nA directory could not be opened.\n";
	cout << exception.getMessage() << endl;
	cout << "Directory name: " << exception.getFileName() << endl;
	cout << "Error code: " << exception.getErrorCode() << endl;
	cout << "Exception source: " << exception.getSource() << endl;

	return os;
}
 
