/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _USER_RESOURCE_H_
#define _USER_RESOURCE_H_

#include "Platform/ResourceManagement/Resource.h"
#include "Platform/Utilities/Array.h"

namespace ResourceManagement
{
	/// Objects of this class should be used to store data from disc.
	/** Requested UserResource<T> objects are cached.
		The number of users of a resource is increased by every request function call. 
		Likewise, each release function call decreases the number of users of a UserResource<T> object.
		If the number of users is set to zero then the resource is freed. */
	template <class T>
	class UserResource : public Resource<T>
	{
	public:
		/** Obtain the number of users.
		@return The returned value (>= 1) is the number of users of this object. */
		inline uint32 getNumOfUsers() const;

		/** A release call decreases the number of users of this UserResource<T> object.
			If the number of users is set to zero then the resource object is freed.
		@param resource Its number of users is decreased and it is freed if necessary.
			The pointer is also set to NULL. */
		static void release(T *&resource);

		/** Request a resource and increases its number of users.
		@param resourceName This is the unique identifier of the resource to be returned.
		@return Returns a pointer to the UserResource<T> object identified by resourceName or
			NULL if it a no resource with this name exists. */
		static T *request(const std::string &resourceName);

	protected:
		/** Create UserResource<T> object with one user.
		@param name This is the identifier of the resource. It must be unique. */
		UserResource(const std::string &name);
		
		/** Does nothing. Just for proper destructor call.*/
		virtual ~UserResource();

		/** Number of users is reduced by one. */
		inline void decreaseNumOfUsers();
		
		/** Number of users is increased by one. */
		inline void increaseNumOfUsers();

	private:
        /** Copy constructor is forbidden.
        @param copy Copy construtor is forbidden. */
		inline UserResource(const UserResource &copy);

        /** Assignment operator is forbidden.
        @param rhs Operator is forbidden. */
		inline UserResource &operator =(const UserResource &rhs);

    protected:
        uint32 mNumOfUsers;	/// Stores the number of users of this UserResource<T> object.
                            /// This value should not become less then zero. Object is freed if it becomes zero.
	};


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	inline uint32 UserResource<T>::getNumOfUsers() const
	{
		return mNumOfUsers;
	}
	
	template <class T>
	void UserResource<T>::release(T *&resource)
	{
		size_t numOfResources = Resource<T>::msResources.size();

		#ifdef _DEBUG
			bool found = false;	// does the resource exist?
			for (size_t i = 0; i < numOfResources; ++i)
			{
				if (Resource<T>::msResources[i] == resource)
				{
					found = true;
					break;
				}
			}
			assert(found);
		#endif // _DEBUG

		// decrease num of users
		assert(resource->mNumOfUsers > 0);
		resource->decreaseNumOfUsers();

		// release & remove it if it is not used anymore
		if (0 < resource->getNumOfUsers())
			return;

		Array<T *>::deleteFirstBySwapWithBack(Resource<T>::msResources, resource);
		delete resource;
		resource = NULL;
	}
	
	template <class T>
	T *UserResource<T>::request(const std::string &resourceName)
	{
		size_t numOfResources = Resource<T>::msResources.size();
		for (size_t i = 0; i < numOfResources; ++i)
		{
			if (resourceName == Resource<T>::msResources[i]->getName())
			{
				Resource<T>::msResources[i]->increaseNumOfUsers();
				return Resource<T>::msResources[i];
			}
		}

		return NULL;
	}
	
	template <class T>
	inline UserResource<T>::UserResource(const std::string &name) :
		Resource<T>(name), mNumOfUsers(1)
	{
		
	}
		
	template <class T>
	inline UserResource<T>::~UserResource()
	{
		assert(mNumOfUsers == 0);
	}
	
	template <class T>
	inline void UserResource<T>::decreaseNumOfUsers()
	{ 
		--mNumOfUsers;
		assert(mNumOfUsers >= 0);
	}
		
	template <class T>
	inline void UserResource<T>::increaseNumOfUsers()
	{
		mNumOfUsers++;
	}
	
	template <class T>
	inline UserResource<T>::UserResource(const UserResource &copy)
	{
		assert(false);
	}

	template <class T>
	inline UserResource<T> &UserResource<T>::operator =(const UserResource &rhs)
	{
		assert(false); return *this;
	}
}

#endif // _USER_RESOURCE_H_
