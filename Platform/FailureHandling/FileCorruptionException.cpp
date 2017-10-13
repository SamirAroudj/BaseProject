/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Platform/FailureHandling/FileCorruptionException.h"

using namespace FailureHandling;
using namespace std;
using namespace Storage;

FileCorruptionException::FileCorruptionException(const string &message, const Path &fileName) :
	FileException(message, fileName)
{

}

std::ostream &FailureHandling::operator <<(std::ostream &os, const FileCorruptionException &exception)
{
	os << "An exception concerning a file occured." << endl;
	os << "Message: " << exception.getMessage() << endl;
	os << "File name: " << exception.getFileName() << endl;
	os << "Source: " << exception.getSource() << endl;

	return os;
}
