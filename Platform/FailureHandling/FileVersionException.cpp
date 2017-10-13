/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Platform/FailureHandling/FileVersionException.h"

using namespace FailureHandling;
using namespace std;
using namespace Storage;

FileVersionException::FileVersionException(const string &message, const Path &fileName, const uint32 codeVersion, const uint32 fileVersion) :
	FileException(message, fileName), mCodeVersion(codeVersion), mFileVersion(fileVersion)
{
}

std::ostream &FailureHandling::operator <<(std::ostream &os, const FileVersionException &exception)
{
	os << "A file version exception concerning a file occured." << endl;
	os << "Message: " << exception.getMessage() << endl;
	os << "File name: " << exception.getFileName() << endl;
	os << "Code version: " << exception.getCodeVersion() << endl;
	os << "File version: " << exception.getFileVersion() << endl;
	os << "Source: " << exception.getSource() << endl;

	return os;
}
