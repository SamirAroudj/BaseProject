/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <cassert>
#include <string>
#include <vector>
#include "Platform/DataTypes.h"

namespace ResourceManagement
{
	/// This is the base class of AgeResource and UserResource
	/** This class is used to organize data which is loaded from file.
		For example, a Texture class can inherit from UserResource. */
	template <class T>
	class Resource
	{
	public:
		/** Free memory space that contained resource pointers.
			There must not be any resource left when calling this function. */
		static void freeMemory()
		{
			assert(msResources.empty());
			msResources.shrink_to_fit();
		}

		/** Obtain the name of the Resource object.
		@return The returned string identifies the Resource object.*/
		const std::string &getName() const
		{
			return mName;
		}

		/** Query the number of Resource objects of the type Resource<T>.
		@return The number of objects of the type Resource<T> is returned. */
		static uint32 getNumOfResources()
		{
			return msResources.size();
		}

		/** Obtain the path to the location where Resource<T> object data is stored.
		@return The returned character array contains the path to the resource location. */
		static const char *const getPathToResources()
		{
			return msResourcePath;
		}

	protected:
		/** Create a resource that is identified by its name.
		@param name Identifies the Resource object. The name must be unique. */
		Resource(const std::string &name) : mName(name)
		{
			#ifdef _DEBUG
				size_t numOfResources = msResources.size();
				for (size_t i = 0; i < numOfResources; ++i)
					assert(msResources[i]->getName() != mName);
			#endif //_DEBUG

			msResources.push_back((T *) this);
		}

		/** Does nothing. Just for proper destructor call of derived objects.*/
		virtual ~Resource()
		{

		}

	private:
        /** Copy constructor is forbidden.
        @param copy Copy constructor is forbidden. */
		Resource(const Resource &copy) { assert(false); }

        /** Assignment operator is forbidden.
        @param rhs Operator is forbidden. */
		Resource &operator =(const Resource &rhs) { assert(false); return *this; }

    protected:
        static const char *msResourcePath;		/// Contains the path to the location where Resource object data is stored.
        static std::vector<T *> msResources;	/// Contains all Resource objects of the type Resource<T>

	protected:
        const std::string mName; /// This is the unique identifier of this Resource object.
	};
}

#endif // _RESOURCE_H_
