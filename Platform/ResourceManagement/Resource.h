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
#include "Platform/Storage/Path.h"

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
		static inline void freeMemory();

		/** Query the number of Resource objects of the type Resource<T>.
		@return The number of objects of the type Resource<T> is returned. */
		static inline uint32 getNumOfResources();

		/** Obtain the path to the location where Resource<T> object data is stored.
		@return The returned character array contains the path to the resource location. */
		static inline const Storage::Path &getPathToResources();

	public:
		/** Obtain the name of the Resource object.
		@return The returned string identifies the Resource object.*/
		inline const std::string &getName() const;

	protected:
		/** Create a resource that is identified by its name.
		@param name Identifies the Resource object. The name must be unique. */
		Resource(const std::string &name);

		/** Does nothing. Just for proper destructor call of derived objects.*/
		inline virtual ~Resource();

	private:
        /** Copy constructor is forbidden.
        @param copy Copy constructor is forbidden. */
		Resource(const Resource &copy);

        /** Assignment operator is forbidden.
        @param rhs Operator is forbidden. */
		Resource &operator =(const Resource &rhs);

    protected:
        static Storage::Path msResourcePath;	/// Contains the path to the location where Resource object data is stored.
        static std::vector<T *> msResources;	/// Contains all Resource objects of the type Resource<T>

	protected:
        const std::string mName; /// This is the unique identifier of this Resource object.
	};
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	inline void Resource<T>::freeMemory()
	{
		// free resource pointers and path memory
		assert(msResources.empty());
		msResources.shrink_to_fit();
		msResourcePath.freeMemory();
	}
	
	template <class T>
	inline uint32 Resource<T>::getNumOfResources()
	{
		return msResources.size();
	}
	
	template <class T>
	inline const Storage::Path &Resource<T>::getPathToResources()
	{
		return msResourcePath;
	}
	
	template <class T>
	inline const std::string &Resource<T>::getName() const
	{
		return mName;
	}
	
	template <class T>
	Resource<T>::Resource(const std::string &name) : mName(name)
	{
		#ifdef _DEBUG
			size_t numOfResources = msResources.size();
			for (size_t i = 0; i < numOfResources; ++i)
				assert(msResources[i]->getName() != mName);
		#endif //_DEBUG

		msResources.push_back((T *) this);
	}
	
	template <class T>
	inline Resource<T>::~Resource()
	{

	}
		
	template <class T>
	Resource<T>::Resource(const Resource<T> &copy)
	{
		assert(false);
	}
	
	template <class T>
	Resource<T> &Resource<T>::operator =(const Resource<T> &rhs)
	{
		assert(false);
		return *this;
	}
}

#endif // _RESOURCE_H_
