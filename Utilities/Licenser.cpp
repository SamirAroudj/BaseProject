/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#include <iostream>
#include "Platform/Storage/Directory.h"
#include "Platform/Storage/File.h"
#include "Utilities/Licenser.h"

using namespace std;
using namespace Storage;
using namespace Utilities;

Licenser::Licenser(const Path &licenseFileName)
{
	// read the license file
	File file(licenseFileName, File::OPEN_READING, false);
	const uint64 length = file.read(mContentBuffer);
	mLicenseText.resize(length);
	mLicenseText = (char *) mContentBuffer.data();
	
	if (mLicenseText.back() != '\n')
		mLicenseText.push_back('\n');
}

void Licenser::setLicense(const Path &root, const vector<string> &fileTypes)
{
	const uint32 typeCount = (uint32) fileTypes.size();
	vector<Path> fileNames;
	
	// set license for all files within root recursively provided they have one of the given file endings
	for (uint32 typeIdx = 0; typeIdx < typeCount; ++typeIdx)
	{
		const string &type = fileTypes[typeIdx];
		Directory::findDescendants(fileNames, root, type);
	}
	
	const uint32 fileCount = (uint32) fileNames.size();
	for (uint32 fileIdx = 0; fileIdx < fileCount; ++fileIdx)
		setLicense(fileNames[fileIdx]);
}

void Licenser::setLicense(const Path &fileName)
{
	// read input file content after license text
	{	
		// open input file
		const string beginning("/*");
		const string ending("*/");
		File input(fileName, File::OPEN_READING, false);
		
		// find old license text beginning
		string line;
		input.readTextLine(line);
		if (string::npos == line.find(beginning))
		{
			cout << "File \"" << fileName << "\" is skipped as it does not begin with a license text!\n";
			return;
		}
			
		// go through old license text
		while (string::npos == line.find(ending))
		{
			if (!input.hasLeftData())
			{
				cout << "File \"" << fileName << "\" is skipped as it starts with a broken license text!\n";
				return;
			}
		
			input.readTextLine(line);
		}
			
		// read file content after license text
		mContentBuffer.clear();
		input.read(mContentBuffer);
	}
	
	// overwrite file with new license text and copy content
	{
		File output(fileName, File::OPEN_READING_AND_WRITING, false);
		output.write(mLicenseText.data(), sizeof(uint8), mLicenseText.size());
		output.write(mContentBuffer.data(), sizeof(uint8), mContentBuffer.size());
	}
}
