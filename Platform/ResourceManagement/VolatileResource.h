/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _VOLATILE_RESOURCE_H_
#define _VOLATILE_RESOURCE_H_

#include <algorithm>
#include "Resource.h"

namespace ResourceManagement
{
	template <class T>
	struct VolatileResourceComparer;

	/// Objects of this class should be used to store data from disc which only needs to be accessible until the next call of request.
	/** Requested Resource objects are cached. But they can be freed every time request is called.
		A User must set the maximum number of cached objects. If a Resource object is requested that is not cached and this object would exceed the
		maximum number of cached objects then the Resource object which wasn't requested for the longest time is released. 
		Therefore a user should not store references or pointers to requested VolatileResource objects if another request call can free these objects. */
	template <class T>
	class VolatileResource : public Resource<T>
	{
	public:
		/** Frees all resources and requested memory. */
		static void freeMemory()
		{
			uint32 numOfResources = (uint32) Resource<T>::msResources.size();
			for (uint32 i = 0; i < numOfResources; ++i)
				delete Resource<T>::msResources[i];
			Resource<T>::msResources.clear();

			Resource<T>::freeMemory();
			msMaximumNumber = 0;
		}

		/** Obtains the "age" of a Resource object.
			The more other resources are requested the older this resource becomes.
		@return Returns the age of this Resource object whereas zero means that
				this resource is young and it was lately requested.*/
		uint32 getAge() const
		{
			return mAge;
		}

		/** Returns the maximum number of Resource objects that are cached.
		@return The maximum number of Resource objects which are cached is returned. */
		static uint32 getMaximumNumber()
		{
			return msMaximumNumber;
		}

		/** Requests a VolatileResource object which is identified by its name.
		@param resourceName Identifies the VolatileResource object.
		@return The requested resource is returned if it is cached otherwise NULL is returned. */
		static T *request(const std::string &resourceName)
		{
			uint32 requestedResourceIndex = static_cast<uint32>(-1);
			uint32 oldestResourceIndex = 0;
			uint32 numOfResources = (uint32) Resource<T>::msResources.size();

			// go through all resources - each one gets older, find oldest and requested resource
			for (uint32 i = 0; i < numOfResources; ++i)		
			{
				VolatileResource<T> *resource = Resource<T>::msResources[i];
				resource->growOlder();

				// update oldest resource idx
				if (Resource<T>::msResources[oldestResourceIndex]->getAge() < resource->getAge())
					oldestResourceIndex = i;

				// resources that are requested often are freed less due to their low age
				if (resourceName == resource->getName())
				{
					resource->becomeYoung();				
					requestedResourceIndex = i;
				}
			}

			if (requestedResourceIndex != static_cast<uint32>(-1))	// found? then return it
				return Resource<T>::msResources[requestedResourceIndex];

			// it is more than likely that a new resource is created since the requested resource does not exist -> free oldest resources if necessary
			if (numOfResources == msMaximumNumber)
			{
				delete Resource<T>::msResources[oldestResourceIndex];
				Resource<T>::msResources[oldestResourceIndex] = Resource<T>::msResources.back();
				Resource<T>::msResources.pop_back();
			}

			return NULL;
		}

		/** Sets the maximum number of VolatileResource<T> objects which are cached.
		@param maxNumOfResources Limits the number of VolatileResource<T> objects which are cached.
								 Cached objects are freed if necessary. (if maxNumOfResources < number of currently cached resources) */
		static void setMaximumNumber(uint32 maxNumOfResources)
		{
			if (maxNumOfResources >= msMaximumNumber)
				return;

			assert(maxNumOfResources > 0);
			msMaximumNumber = maxNumOfResources;

			std::sort(Resource<T>::msResources.begin(), Resource<T>::msResources.end(), VolatileResourceComparer<T>());

			for (uint32 i = Resource<T>::msResources.size(); i > msMaximumNumber; --i)
			{
				delete Resource<T>::msResources.back();
				Resource<T>::msResources.pop_back();
			}
		}

	protected:
		/** Creates a new and "young" VolatileResource<T> object.
		@param name This must be a unique identifier. */
		VolatileResource(const std::string &name) : Resource<T>(name), mAge(0) { }

		/** Does nothing. It's there for safe programming reasons.*/
		virtual ~VolatileResource() { }

		/** Sets resource age to zero. So other VolatileResource<T> objects are freed erlier. */
		void becomeYoung() { mAge = 0; }

		/** Increases the age. Therefore it is more likely that this object is freed if necessary. */
		void growOlder() { ++mAge; }

    private:
        /** Copy constructor is forbidden.
        @param copy Copy constructor is forbidden.*/
        VolatileResource(const VolatileResource &copy) { assert(false); }

        /** Assignment operator is forbidden.
        @param rhs Operator is forbidden. */
		VolatileResource &operator =(const VolatileResource &rhs) { assert(false); return *this; }

    protected:
		static uint32	msMaximumNumber;	/// Contains the maximum number of VolatileResource<T> objects which are cached.
		uint32			mAge;				/// Stores the age of this resource wheras zero means young. Older resources are freed erlier than younger resources.
	};
	
	/// This struct is used for sorting of VolatileResource objects
	template <class T>
	struct VolatileResourceComparer
	{
		/** Compares two VolatileResource<T> objects according to their age. 
		@param lhs lhs is tested to be "smaller" than rhs according to their age.
		@param rhs ths is tested to be "greater than or equal to"  lhs according to their age.
		@return True is returned when lhs is younger than rhs. */
		bool operator ()(const VolatileResource<T> *const lhs, const VolatileResource<T> *const rhs)
		{
			return lhs->getAge() < rhs->getAge();
		}
	};
}

#endif // _VOLATILE_RESOURCE_H_
