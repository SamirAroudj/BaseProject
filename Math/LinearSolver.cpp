/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cassert>
#include "LinearSolver.h"

using namespace Math;

void LinearSolver::solveWithConjugateGradients(Real *x, Real *diA, Real *ri, Real *di,
											   const uint32 n, const Real *A, const Real *b, const Real errorTolerance,
											   const uint32 maxNumIterations, const uint32 maxIncrementalResiduumIters)
{
	// d0^t = r0^t (= residuum^t) = b^t - x0^t * A^t
	multiply(ri, n, x, A);
	for (uint32 componentIdx = 0; componentIdx < n; ++componentIdx)
		ri[componentIdx] = b[componentIdx] - ri[componentIdx];
	for (uint32 componentIdx = 0; componentIdx < n; ++componentIdx)
		di[componentIdx] = ri[componentIdx];

	// <ri, ri>, target error measure to be reached & maximum number of iterations
		  Real riDotRi	= dotProduct(n, ri, ri);
	const Real target	= riDotRi * errorTolerance * errorTolerance;

	// compute solution, abort when nMax iterations were executed or the residuum (= error measure) is small enough
	for (uint32 i = 0, incrementalResiduumIters = 0;
		 (i < maxNumIterations) && (riDotRi > target);
		 ++i, ++incrementalResiduumIters)
	{
		// ai = <ri, ri> / <di, di^t * A^t>
		multiply(diA, n, di, A);
		Real ai = riDotRi / dotProduct(n, di, diA);	 
		
		// xi+1 = xi + ai * di;
		for (uint32 componentIdx = 0; componentIdx < n; ++componentIdx)
			x[componentIdx] += ai * di[componentIdx];
		
		// ri = ...
		if (maxIncrementalResiduumIters == incrementalResiduumIters)
		{
			// ri+1^t = b^t - x^t * A^t: recompute residuum completely to avoid numerical problems
			incrementalResiduumIters = 0;
			multiply(ri, n, x, A);
			for (uint32 componentIdx = 0; componentIdx < n; ++componentIdx)
				ri[componentIdx] = b[componentIdx] - ri[componentIdx];
		}
		else 
		{
			// ri+1^t = ri^t - ai * di^t * A^t: incremental residuum computation
			for (uint32 componentIdx = 0; componentIdx < n; ++componentIdx)
				ri[componentIdx] -= ai * diA[componentIdx];
		}

		// di+1 = ri+1 + (<ri+1, ri+1> / <ri, ri>) * di
		Real riDotRiNew = dotProduct(n, ri, ri);
		Real factor		= riDotRiNew / riDotRi;
		for (uint32 componentIdx = 0; componentIdx < n; ++componentIdx)
			di[componentIdx] = ri[componentIdx] + factor * di[componentIdx];
		riDotRi = riDotRiNew;
	}
}

bool LinearSolver::solveWithJacobi(Real *x, Real *buffer, const uint32 size, const Real *A, const Real *b,
								   const Real errorTolerance, const uint32 maxNumIterations)
{
	const Real eSq = errorTolerance * errorTolerance;

	// todo: optimization in parallel on GPU? or change memory layout?
	for (uint32 iterCount = 0; iterCount < maxNumIterations; ++iterCount)
	{
		Real rSq = 0.0f; // squared error measure magnitude = squared residuum magnitude

		// x[i] = (b[i] - sum over {0 < j != i < size} of (x[j] * mat[j][i])) / mat[i][i]
		for (uint32 newXIdx = 0, outerOffset = 0; newXIdx < size; ++newXIdx, outerOffset += size)
		{
			// find new x
			Real sum = 0.0f;
			for (uint32 matRowIdx = 0, innerOffset = 0; matRowIdx < size; ++matRowIdx, innerOffset += size)
				if (matRowIdx != newXIdx)
					sum += x[matRowIdx] * A[innerOffset + newXIdx];

			assert(A[outerOffset + newXIdx]);
			buffer[newXIdx] = (b[newXIdx] - sum) / A[outerOffset + newXIdx];

			Real componentError = b[newXIdx] - (sum + x[newXIdx] * A[outerOffset + newXIdx]);
			rSq += componentError * componentError;
		}

		// swap x and buffer
		Real *temp = x;
		x = buffer;
		buffer = temp;

		// solution achieved?
		if (eSq > rSq)
			return true;
	}

	return false;
}

//void LinearSolver::difference(Real *result, const uint32 n, const Real *a, const Real *b)
//{
//	for (uint32 i = 0; i < n; ++i)
//		result[i] = a[i] - b[i];
//}
		
Real LinearSolver::dotProduct(const uint32 n, const Real *a, const Real *b)
{
	Real result = 0.0f;
	for (uint32 i = 0; i < n; ++i)
		result += a[i] * b[i];
	return result;
}

void LinearSolver::multiply(Real *result, uint32 n, const Real *x, const Real *A)
{
	// set result to 0
	for (uint32 i = 0; i < n; ++i)
		result[i] = 0.0f;

	// compute result by accessing one matrix row after the other: split up dot product for each component for better memory access
	for (uint32 xIdx = 0, offset = 0; xIdx < n; ++xIdx, offset += n) 
	{
		// go over matrix row xIdx
		for (uint32 resultIdx = 0; resultIdx < n; ++resultIdx)
			result[resultIdx] += x[xIdx] * A[offset + resultIdx];
	}
}

//void LinearSolver::multiply(Real *result, uint32 n, const Real scalar, const Real *x)
//{
//	for (uint32 i = 0; i < n; ++i)
//		result[i] = scalar * x[i];
//}
