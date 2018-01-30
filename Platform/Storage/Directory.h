/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _STORAGE_DIRECTORY_H_
#define _STORAGE_DIRECTORY_H_

#include <string>
#include <vector>
#include "Platform/DataTypes.h"
#include "Platform/Storage/Path.h"

namespace Storage
{
	class Directory
	{
	public:
		static bool createDirectory(const Path &path, const bool failIfExisting = false);

		/** Checks whether a directory exists.
		@param path Set this to the complete path of the directory you want to check.
		@return Returns true if the directory identified by path exists. Only returns true if it is a directory. */
		static bool exists(const Path &path);

		static void findChildren(std::vector<std::string> &children,
			const Path &path, const std::string &ending = "");
		static void findDescendants(std::vector<Path> &descendants, const Path &root, const std::string &ending = "");

	private:
	};
}

#endif // _STORAGE_DIRECTORY_H_
