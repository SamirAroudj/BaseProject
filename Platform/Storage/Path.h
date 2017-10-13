/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _STORAGE_PATH_H_
#define _STORAGE_PATH_H_

#include <ostream>
#include <string>
#include "Platform/DataTypes.h"

namespace Storage
{
	class Path
	{
	public:
		static inline Path appendChild(const Path &parent, const Path &child);
		static inline Path extendLastName(const Path &path, const std::string &extension);
	
	public:
		Path();
		Path(const char *path);
		Path(const std::string &path);
		
		inline Path &appendChild(const std::string &subFolder);
		Path &appendChild(const Path &subFolder);

		bool getParent(Path &parent) const;

		Path &extendLastName(const std::string &extension);

		inline const std::string &getString() const;
		inline const char *getCString() const;

		Path &operator =(const std::string &path);
		Path &operator =(const Path &path);
		//inline Path operator +(const std::string &childPath) const;
		//Path operator +(const Path &childPath) const;

		//inline Path &operator +=(const std::string &childPath);
		//inline Path &operator +=(const Path &childPath);

	private:
		std::string mPath;
	};

	std::ostream &operator <<(std::ostream &os, const Path &path);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	inline Path Path::appendChild(const Path &parent, const Path &child)
	{
		Path totalPath(parent);
		totalPath.appendChild(child);
		return totalPath;
	}

	inline Path &Path::appendChild(const std::string &subFolder)
	{
		const Path subPath(subFolder);
		return appendChild(subPath);
	}

	inline Path Path::extendLastName(const Path &path, const std::string &extension)
	{
		Path totalPath(path);
		totalPath.extendLastName(extension);
		return totalPath;
	}
	
	inline const std::string &Path::getString() const
	{
		return mPath;
	}

	inline const char *Path::getCString() const
	{
		return mPath.c_str();
	}

	//inline Path Path::operator +( const std::string &childPath) const
	//{
	//	const Path temp(childPath);
	//	return *this + temp;
	//}

	//inline Path &Path::operator +=(const std::string &childPath)
	//{
	//	return this->append(childPath);
	//}
	//
	//inline Path &Path::operator +=(const Path &childPath)
	//{
	//	return this->append(childPath);
	//}
}

#endif // _STORAGE_PATH_H_
