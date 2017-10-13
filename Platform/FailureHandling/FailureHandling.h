/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _FAILURE_HANDLING_H_
#define _FAILURE_HANDLING_H_


/// Contains all classes to deal with exceptional or erroneous situations.
/** The exceptions which are provided by this namespace are designed to help overcoming situation specific problems.
	They are used to keep track of the source and reason of an exception that cannot simply be dealt by the invoking function.
*/
namespace FailureHandling
{
	/** Checks for different kinds of errors, such as ones from CUDA or OpenGL.
		An assertion fails if an error has occured.*/
	void checkForErrors();
	
	#ifdef CUDA
		/** Checks for errors from CUDA.
			An assertion fails if an error has occured.*/
		void checkForCUDAErrors();
	#endif // CUDA
	
	/** Checks for errors from OpenGL.
		An assertion fails if an error has occured.*/
	void checkForOpenGLErrors();
}

#endif // _FAILURE_HANDLING_H_