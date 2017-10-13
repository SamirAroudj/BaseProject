/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifdef _WINDOWS
	#include<windows.h>
#elif _LINUX
	#include <cstring>
	#include <sys/stat.h>
	#include <dirent.h>
#endif // _WINDOWS

#include <cassert>
#include "Platform/FailureHandling/DirectoryAccessException.h"
#include "Platform/Storage/Directory.h"
#include "Utilities/HelperFunctions.h"

using namespace FailureHandling;
using namespace Storage;
using namespace std;

bool isFakeEntry(const char *entryName)
{
	bool isFake = false;
	isFake |= ((1 == strlen(entryName)) && (0 == strncmp(entryName, ".", 1)));
	isFake |= ((2 == strlen(entryName)) && (0 == strncmp(entryName, "..", 2)));
	return isFake;
}

bool Directory::createDirectory(const Path &directoryName, const bool failIfExisting)
{
	// do nothing if existing && no fail if already existing
	if (!failIfExisting)
		if (exists(directoryName))
			return true;

	// create the directory
	#ifdef _WINDOWS
		return (0 != CreateDirectoryA(directoryName.getCString(), NULL));
	#elif _LINUX
		const mode_t mode = (S_IRWXU | S_IRWXG | S_IRWXO);
		return (0 == mkdir(directoryName.getCString(), mode));
	#else
		assert(false);
		return false;
	#endif // _WINDOWS
		
	return false;
}

bool Directory::exists(const Path &directoryName)
{
	bool exists = false;
	bool isDirectory = false;

	#ifdef _WINDOWS
		const DWORD attributes = GetFileAttributesA(directoryName.getCString());
		exists = (INVALID_FILE_ATTRIBUTES != attributes);
		if (exists)
			isDirectory = (FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY & attributes));
	#elif _LINUX
		struct stat status;
		exists = (0 == stat(directoryName.getCString(), &status));
		if (exists)
			isDirectory = (0 != (S_IFDIR & status.st_mode));
	#else
		assert(false);
		return false;
	#endif // _WINDOWS

	if (exists)
		return isDirectory;
	return false;
}

void Directory::findChildren(vector<string> &children, const Path &path, const string &ending)
{
	const string errorMessage = "Directory::findChildren failed!";
	
	#ifdef _WINDOWS
		WIN32_FIND_DATAA searchData;
		memset(&searchData, 0, sizeof(WIN32_FIND_DATAA));

		// search mask
		string searchMask = path.getString() + "/*";
		searchMask += ending;

		// access directory
		HANDLE findHandle = FindFirstFileA(searchMask.c_str(), &searchData);
		if (INVALID_HANDLE_VALUE == findHandle)
			throw DirectoryAccessException("Directory::findChildren failed!", path.getString(), GetLastError());
	
		// go through all thingies in the folder		
		while(INVALID_HANDLE_VALUE != findHandle)
		{
			children.push_back(searchData.cFileName);
			if (false == FindNextFileA(findHandle, &searchData))
				break;
		}
			
		// close the directory
		FindClose(findHandle);
		findHandle = INVALID_HANDLE_VALUE;
		
	#elif _LINUX
		// access directory
		DIR *dirStream = opendir(path.getCString());
		if (NULL == dirStream)
		{
			// todo log this properly
			throw DirectoryAccessException(errorMessage, path.getString(), errno);
			return;	
		}
		
		// go through all thingies in the folder
		if (ending.empty())
		{
			for (struct dirent *entry = readdir(dirStream); entry; entry = readdir(dirStream))
				if (!isFakeEntry(entry->d_name))
					children.push_back(entry->d_name);
		}
		else
		{
			for (struct dirent *entry = readdir(dirStream); entry; entry = readdir(dirStream))
			{
				const string entryName = entry->d_name;
				if (!isFakeEntry(entry->d_name) && Utilities::hasEnding(entryName, ending))
					children.push_back(entryName);
			}
		}
		
		// close the directory
		closedir(dirStream);
		dirStream = NULL;
		
	#else
		assert(false);
		
	#endif // _WINDOWS
}

void Directory::findDescendants(vector<Path> &descendants, const Path &root, const string &ending)
{	
	// find direct children
	vector<string> children;
	Directory::findChildren(children, root);
	const uint32 childCount = (uint32) children.size();
	
	// for all direct children: add fitting files & go through folders
	for (uint32 childIdx = 0; childIdx < childCount; ++childIdx)
	{
		const string &childName = children[childIdx];
		
		// next directory to search through?
		Path childFolder(root);
		childFolder.appendChild(childName);
		if (Directory::exists(childFolder))
		{
			Directory::findDescendants(descendants, childFolder, ending);
			continue;
		}
		
		// file but wrong ending?
		if (!Utilities::hasEnding(childName, ending))
			continue;
		
		// add wanted file
		Path filePath(root);
		filePath.appendChild(childName);
		descendants.push_back(filePath);			
	}
}

