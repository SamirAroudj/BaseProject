/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#include "Platform/Storage/Path.h"

using namespace std;
using namespace Storage;

Path::Path()
{

}

Path::Path(const char *path) :
 Path(string(path))
{
	
}

Path::Path(const string &path) :
	mPath(path)
{
	if (mPath.empty())
		return;

	for (uint32 i = 0; i < path.size(); ++i)
		if ('\\' == mPath[i])
			mPath[i] = '/';

	if ('/' == mPath.back())
		mPath.pop_back();
}

Path &Path::appendChild(const Path &subFolder)
{
	if (!mPath.empty() && '/' != subFolder.mPath[0])
		mPath += '/';
	mPath += subFolder.mPath;

	return *this;
}

Path &Path::extendLastName(const string &extension)
{
	Path newPath(mPath + extension);
	mPath = newPath.mPath;
	return *this;
}

bool Path::getParent(Path &parent) const
{
	size_t found = mPath.find_last_of('/');
	if (string::npos == found)
		return false;

	parent.mPath = mPath.substr(0, found);
	return true;
}

Path &Path::operator =(const std::string &path)
{
	const Path temp(path);
	return this->operator=(temp);
}

Path &Path::operator =(const Path &path)
{
	if (this == &path)
		return *this;

	mPath = path.mPath;
	return *this;
}

//Path Path::operator +(const Path &childPath) const
//{
//	Path result(*this);
//	result += childPath;
//	return result;
//}

ostream &Storage::operator <<(ostream &os, const Path &path)
{
	os << path.getString();
	return os;
}
