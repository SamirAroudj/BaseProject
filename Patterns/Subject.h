/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _SUBJECT_H_
#define _SUBJECT_H_

#include <cassert>
#include "Platform/DataTypes.h"
#include "Utilities/HelperFunctions.h"

namespace Patterns
{
	/** Bahavioral Pattern::Subject Observer: Subject class - See Design Patterns book by Gamma, Helm, Johnson, Vlissides
		Defines a one-to-many dependency between objects.
		When the subject object changes state, all its dependents (observers) are notified and updated.
	*/
	template <class T>
    class Subject
	{
	public:
		/** Creates an empty subject */
		inline Subject();

		/** Only checks that all observers were deregistered again. */
		inline virtual ~Subject();

		/** Deregisters all observers. */
		void clear();

		/** Deregisters a particular observer.
			In debug mode: checks that observer has actually been registered before.
		@param observer Set this to the observer you want to deregister.
			Only deregister once and only deregister observers which have been registered before otherwise an assertion fails in debug mode.*/
		void deregisterObserver(T *observer);

		/** Returns the number of currently registered observers.
		@return Returns the number of currently registered observers. */
		inline uint32 getObserverCount() const;

		/** Registers an observer object which can then be notified by this Subject object about state changes.
		@param observer Set this to any unregistered observer object which shall be notified about state changes of this Subject object.
			Register each observer object once at a time.
			There must not be duplicates in the list of managed observers.*/
		inline void registerObserver(T *observer);

		/** Calls registerObserver for all entered elements.
			Do not register any object more than once at a time.
			There must not be duplicates in the internal set of managed observers.
		@param observers Fill this with all objects which should receive notifications from this Subject object.
			Do not register observers multiple times. Debug mode: an assertion fails if the same observer object is registered more than once. */
		inline void registerObservers(const std::vector<T *> &observers);

	protected:
		std::vector<T *> mObservers;	/// Contains links to all observer objects which should be notified by state changes of this Subject object.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	Subject<T>::Subject()
	{

	}
	
	template <class T>
	Subject<T>::~Subject()
	{
		assert(0 == getObserverCount());
	}

	template <class T>
	void Subject<T>::clear()
	{
		mObservers.clear();
	}
	
	template <class T>
	void Subject<T>::deregisterObserver(T *observer)
	{	
		const size_t size = mObservers.size();
		const size_t idx = Utilities::deleteFirstBySwapWithBack<T *>(mObservers, observer, 0);
		if (idx < size)
			return;
		
		// todo log this error
		#ifdef _DEBUG
			// this observer has never been registered
			assert(false);
		#endif // _DEBUG
	}

	template <class T>
	inline uint32 Subject<T>::getObserverCount() const
	{
		return (uint32)	mObservers.size();
	}
	
	template <class T>
	void Subject<T>::registerObserver(T *observer)
	{
		#ifdef _DEBUG
			// check that observer is only registered once
			const uint32 count = (uint32) mObservers.size();
			for (uint32 i = 0; i < count; ++i)
				assert(observer != mObservers[i]);
		#endif // _DEBUG

		mObservers.push_back(observer);
	}

	template <class T>
	inline void Subject<T>::registerObservers(const std::vector<T *> &listeners)
	{
		const size_t count = listeners.size();
		for (size_t i = 0; i < count; ++i)
			registerObserver(listeners[i]);
	}
}

#endif // _SUBJECT_H_
