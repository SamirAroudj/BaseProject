/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _LINEAR_SOLVER_H_
#define _LINEAR_SOLVER_H_

#include "Platform/DataTypes.h"
#include "Math/MathCore.h"

namespace Math
{
	/// This class implements different numerical solver techniques for a system of linear equations.
	class LinearSolver
	{
	public:
		//static void difference(Real *result, const uint32 n, const Real *a, const Real *b);
		//static void multiply(Real *result, uint32 n, const Real scalar, const Real *x);

		/** Implements vector matrix multiplication: r^t = x^t * A^t. Caution! DirectX convention!
			Memory of vector x and vector result must not be the same.
		@param result Contains r^t = x^t * A^t after call.  Caution! DirectX convention!
		@param size Vectors result and x are considered to have size elements and A is considered to be a square matrix of size size x size. 
		@param x This vector is assumed to have size elements and is used to compute r^t = x^t * A^t. Caution! DirectX convention!
		@param A This matrix is assumed to have a size of size(x) * size(x) and it is used to compute r^t = x^t * A^t.  Caution! DirectX convention!*/
		static void multiply(Real *result, uint32 size, const Real *x, const Real *A);

		/** Computes the commutative dot (or inner) product of a and b.
		@param size Vectors a and b are assumed to have size elements.
		@param a Is assumed to have size elements and is used for dot product computation of vector a and b. 
		@param b Is assumed to have size elements and is used for dot product computation of vector a and b. 
		@return The sum of the products a[i] * b[i] with 0 <= i < size is returned.*/
		static Real dotProduct(const uint32 size, const Real *a, const Real *b);

		/** Solves the system of linear equations: x^t * A^t = b^t with the conjugate gradients method where x^t is searched. Caution! DirectX convention!
			Requires a symmetric, positive semi-definite matrix A. 
		@param x Is assumed to have size size. Set this to a close initial or guessed solution for a warm start of the solution of x^t * A^t = b^t.
		@param diA diA must be a vector with size elements. The memory space is required for internal computations. The initial vector values don't matter.
		@param ri ri must be a vector with size elements. The memory space is required for internal computations. The initial vector values don't matter.
		@param di di must be a vector with size elements. The memory space is required for internal computations. The initial vector values don't matter.
		@param size Defines the size of the system of linear equations. A is assumed to be of size size x size and b, x, diA and ri are assumed to have size elements.
		@param A Defines the system of linear equations together with b. A is assumed to have size (size x size). Caution! DirectX convention is used: x^t * A^t = b^t is solved.
			   A must be a symmetric, positive semi-definite matrix. 
		@param b Defines the system of linear equations together with A. b is assumed to have size elemetns.  Caution! DirectX convention is used: x^t * A^t = b^t is solved.
		@param errorTolerance The system of linear equations is considered to be solved when the following is achieved:
			   latest residuum magnitude <= errorTolerance * initial residuum magnitude
		@param maxNumIterations CG method finds the solution for a symmetric, positive definite matrix of size size x size in size iterations if there are no numerical errors.
			   So set this value to size plus some constant like maxIncrementalResiduumIters to find the exact solution with certainty.
		@param maxIncrementalResiduumIters Defines after how many iterations the residuum (an error measure) is completely calculated instead of incrementally updated.
			   Set it to a value of around 50 to speed up computations while still avoiding numerical problems. */
		static void solveWithConjugateGradients(Real *x, Real *diA, Real *ri, Real *di,
												const uint32 size, const Real *A, const Real *b, const Real errorTolerance,
												const uint32 maxNumIterations, const uint32 maxIncrementalResiduumIters = 50);

		/** Solves the system of linear equations: x^t * A^t = b^t with the Jacobi method where x^t is searched. Caution! DirectX convention!
			Requires a strongly diagonally dominant matrix A. 
		@param x Is assumed to have a size which is equal to the parameter size. Set this to a close initial or guessed solution for a warm start of the solution of x^t * A^t = b^t.
		@param buffer Is assumed to have size size. The memory space is required for internal computations. The initial vector values don't matter.
		@param size Defines the size of the system of linear equations. A is assumed to be of size size x size and x, buffer and b are assumed to have size elements.
		@param A Defines the system of linear equations together with b. A is assumed to have size size x size. Caution! DirectX convention is used: x^t * A^t = b^t is solved.
			   A must be a strongly diagonally dominant matrix.
		@param b Defines the system of linear equations together with A. b is assumed to have size elemetns.  Caution! DirectX convention is used: x^t * A^t = b^t is solved.
		@param errorTolerance The system of linear equations is considered to be solved when the following is achieved:
			   latest residuum magnitude <= errorTolerance
		@param maxNumIterations Limits the number of Jacobi iterations in each of which every x component is changed.
		@param Returns true if a solution within the tolerance was found or false if maxNumIterations were reached without finding the solution. */
		static bool solveWithJacobi(Real *x, Real *buffer, const uint32 size, const Real *A, const Real *b,
									const Real errorTolerance, const uint32 maxNumIterations);
	};
}

#endif // _LINEAR_SOLVER_H_
