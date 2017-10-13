/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cassert>
#ifdef _WINDOWS
	#include <Windows.h>
#endif // _WINDOWS
#include <GL/gl.h>
#include "FailureHandling.h"

void FailureHandling::checkForErrors()
{
	#ifdef CUDA
		FailureHandling::checkForCUDAErrors();
	#endif // CUDA
	FailureHandling::checkForOpenGLErrors();
}
	
void FailureHandling::checkForOpenGLErrors()
{
	GLenum glError = glGetError();
    if (glError != GL_NO_ERROR)
	{
		int dummy = 42;
		++dummy;
		assert(false);
	}
}
