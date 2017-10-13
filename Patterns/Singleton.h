/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <cassert>
#include <cstring>
#include <new>

///Contains classes which implement useful design patterns, such as Singleton.
/** This namespace is intended to store implementations of design patterns. The classes are solutions for common problems which can occur in various situations.
	The aim of the implementations is to provide efficient, well known functionality which is easy to understand since there are many documentations about these pattterns.
	Hence design patterns allow to decently structure a programme and enhance team communication. */
namespace Patterns
{
	/// The singleton pattern implementation prohibits existance of several objects of its class at one time and provides global access to its unique class object.
	/** This class is used for functionality which only need one object to be carried out. Often managers are singletons;
		imagine a manager class that is responsible for initialising and shutting down network resources.
		You probably won't need two objects to deal with this issue. If you try to create a second object while another one is active then
		an assert fails since two several active objects of this class are forbidden. There is also a convenient way to access these
		singleton objects by there getSingleton class function which is handier than storing pointer to these unique objects. */
	template <class T>
	class Singleton
	{
	public:
		/** Creates the unique object. Fails if there is already another active singleton object.*/
		Singleton();

		/** Destroys the unique object. Fails if there is no active singleton object.
			You can only call the destructor if there is an active object.
			After destruction you can create a new singleton which does not infringe on the single object rule. */
		virtual ~Singleton();

		/** Global access to the single object which fails if there is no active object.
			This static functions returns a reference to the unique object. It fails if there is no single object.
		@return The returned object is a reference to the singleton object. */
		inline static T &getSingleton();

		/** Global access to the singleton object which fails if there is no active object.
			The functions provides global access which only works if there is an active object.
		@return The returned variable contains a pointer to the single object. There is an assert which fails when there is no active object. */
		inline static T *getSingletonPointer();

		/** Check weather there is an singleton object which can be used.
			The function can be used in order to determine weather there is a singleton object or weather a singleton object should be created.
		@return The function returns true if there is a singleton which can be used.
				Returns false if the singleton object must be created before use of getSingleton.*/
		inline static bool exists();
		
	protected:
		static T *msSingleton;	/// This static variable actually contains the single object. 
								/// Therefore there can only be one object of this class and it can be accessed by static functions.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	Singleton<T>::Singleton()
	{
		assert(!msSingleton);
		msSingleton = static_cast<T *> (this);
	}
	
	template <class T>
	Singleton<T>::~Singleton()
	{
		assert(msSingleton);
		msSingleton = NULL;
	}
	
	template <class T>
	inline T &Singleton<T>::getSingleton()
	{
		assert(msSingleton);
		return *msSingleton;
	}
	
	template <class T>
	inline T *Singleton<T>::getSingletonPointer()
	{
		assert(msSingleton);
		return msSingleton;
	}
	
	template <class T>
	inline bool Singleton<T>::exists()
	{
		return NULL != msSingleton;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   static template member variable definitions   ////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <class T>
	T *Singleton<T>::msSingleton = NULL;
}

#endif // _SINGLETON_H_

