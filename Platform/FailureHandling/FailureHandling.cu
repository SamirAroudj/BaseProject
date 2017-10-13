/**
	Author: Samir Aroudj
*/
#include <cassert>
#include "FailureHandling.h"

void FailureHandling::checkForCUDAErrors()
{	
	cudaError_t cudaError = cudaGetLastError();
	if (cudaError != cudaSuccess)
	{
		const char *errorString = cudaGetErrorString(cudaError);
		assert(false);
		if (errorString)
			errorString = NULL;
	}
}
