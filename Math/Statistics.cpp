/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#include <cstring>
#include "Math/LinearSolver.h"
#include "Math/Statistics.h"

using namespace Math;

void Statistics::getMLSEFit(Real *parameters, const Real *XTranspose, const Real *y, const uint32 observationCount, const uint32 parameterCount)
{
	// solve y^t * X = theta^t * X^t * X
	// -> reformulate: X^t * X = A^t = A, theta = x, y^t * X = b^t -> x^t * A^t = b^t
	// -> employ linear solver

	// request required memory
	uint32 parameterCountSq = parameterCount * parameterCount;
	uint32 requiredElements	= parameterCountSq + 4 * parameterCount;

	Real *A		= new Real[requiredElements];
	Real *b		= A		+ parameterCountSq;
	Real *diA	= b		+ parameterCount;
	Real *ri	= diA	+ parameterCount;
	Real *di	= ri	+ parameterCount;

	// X^t * X = A^t
	for (uint32 i = 0, offset_i = 0; i < parameterCount; ++i, offset_i += observationCount)
	{
		for (uint32 j = i, offset_j = offset_i; j < parameterCount; ++j, offset_j += observationCount)
		{
			Real result = LinearSolver::dotProduct(observationCount, XTranspose + offset_i, XTranspose + offset_j);

			A[i * parameterCount + j] = result;
			if (i != j)
				A[j * parameterCount + i] = result;
		}
	}

	// b^t = y^t * X
	for (uint32 targetColumn = 0, offset = 0;  targetColumn < parameterCount; ++targetColumn, offset += observationCount)
		b[targetColumn] = LinearSolver::dotProduct(observationCount, y, XTranspose + offset);

	LinearSolver::solveWithConjugateGradients(parameters, diA, ri, di, parameterCount, A, b, Math::EPSILON, -1);

	// free resources
	delete [] A;
	A	= NULL;
	b	= NULL;
	diA = NULL;
	ri	= NULL;
	di	= NULL;
}
