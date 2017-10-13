/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _KERNELS_INTERPOLATIONS_WEIGHTINGS_H_
#define _KERNELS_INTERPOLATIONS_WEIGHTINGS_H_

#include <cassert>
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Platform/DataTypes.h"

namespace Math
{
	/** Computes the value of a normalized 1D Gaussian function g(x) with mean at 0.
	@param x Set this to the x value where you want to evaluate g(x).
	@param stdDev Defines the standard deviation of the 1D Gaussian function g(x).
	@return Returns the value of the normalized 1D Gaussian function g(x).*/
	inline Real getGaussian1D(const Real x, const Real stdDev);

	/** Computes the value of a normalized 1D Gaussian function g(x) with mean at 0.
	@param dSq Set this to the value dSq = x * x which corresponds to the evaluation position x.
	@param stdDev Defines the standard deviation of the 1D Gaussian function g(x).
	@return Returns the value of the normalized 1D Gaussian function g(x) with dSq = x * x and Gaussian mean = 0.*/
	Real getGaussian1DForSquaredDistance(const Real dSq, const Real stdDev);

	/** Computes the value of a normalized 2D Gaussian function g(x, y) with mean at (0, 0).
	@param x Set this to the x value where you want to evaluate g(x, y).
	@param y Set this to the y value where you want to evaluate g(x, y).
	@param stdDev Defines the standard deviation of the 2D Gaussian function g(x, y).
	@return Returns the value of the normalized 2D Gaussian function g(x, y).*/
	inline Real getGaussian2D(const Real x, const Real y, const Real stdDev);

	/** Computes and returns the derivative g'(x, y) = dg(x,y)/dx of the normalized 2D Gaussian function g(x, y) w.r.t. x.
	@param derivativeDistance This value corresponds to the function evaluation position x of the axis for which the derivative is computed.
	@param orthogonalDistance This value corresponds to the function evaluation position y of the axis orthogonal to the axis for which the derivative is computed.
	@param stdDev Defines the standard deviation of the 2D Gaussian function g(x, y).
	@return Returns the value of the derivative g'(x, y) = dg(x,y)/dx of the normalized 2D Gaussian g(x, y), mean = (0, 0) with respect to the x-axis. */
	Real getGaussian2DDerivative(const Real derivativeDistance, const Real orthogonalDistance, const Real stdDev = 2.0f);

	/** Computes the value of a normalized 2D Gaussian function g(x, y) with mean at (0, 0).
	@param dSq Set this to the value dSq = x * x + y * y which corresponds to the evaluation position (x, y).
	@param stdDev Defines the standard deviation of the 2D Gaussian function g(x, y).
	@return Returns the value of the normalized 2D Gaussian function g(x, y) with dSq = x * x + y * y and Gaussian mean = (0, 0).*/
	Real getGaussian2DForSquaredDistance(const Real dSq, const Real stdDev);

	/** Computes the value of a 3D normalized Gaussian function g(p) with p \in Real^3 and mean at [0, 0, 0].
	@param p Set this to the position p = [x, y, z] where you want to evaluate the normalized zero-mean Gaussian function g(p).
	@param stdDev Defines the standard deviation of the 3D Gaussian function g(p).
	@return Returns the value of the normalized zero-mean 3D Gaussian function g(p).*/
	Real getGaussian3D(const Vector3 &p, const Real stdDev);

	/** Computes and returns the derivative g'(p) = g'([x, y, z]) = d(g([x,y,z]))/d(derivativeDir) of the zero-mean normalized 3D Gaussian function g(p) w.r.t. the axis derivativeDir (x-, y- or z-axis).
	@param evaluationPosition Set this to the point p = [x, y, z] where you want to evaluate the function g'(p).
	@param derivativeDir Set this to AXIS_X, AXIS_Y or AXIS_Z to get the value of the derivative of g(p) w.r.t. the x-, y- or z-axis. (0 -> x, 1 -> y, 2 -> z)
	@param stdDev Defines the standard deviation of the 3D Gaussian function g(p).
	@return Returns the value of the derivative g'(p) of the normalized 3D Gaussian g([x, y, z]), mean = [0, 0, 0] with respect to the specified axis derivativeDir. */
	Real getGaussian3DDerivative(const Vector3 &evaluationPosition, const Axis derivativeDir, const Real stdDev = 2.0f);

	/** Computes a compactly supported 1D polynomial weighting / kernel function with support range R and the following properties:
		1) f(R) = 0
		2) f'(R) = 0
		3) f'(0) = 0
		4) integral(0, supportRange){f(x) * dx} = 1
		5)	f(x) = 0 if x <= 0 || x >= supportRange \n
			f(x) = (4/R^4) * x^3 - (6/R^3) * x^2 + (2/R)
	This function is for evaluation of 1D kernels.
	@param x This defines where the weighting function is evaluated.
	@param R Defines the support range or length of the weighting / kernel function which starts at x = 0 and ends at x = R. The function returns 0 outside.
	@return Returns the value of the following function:
		f(x) = 0 if x <= 0 || x >= 4 \n
		f(x) = (4/R^4) * x^3 - (6/R^3) * x^2 + (2/R) */
	Real getKernel1DPoly3(const Real r, const Real supportRange);

	/** todo 
		polynomial multi-scale weighting function for circular disks
		compute polynomial kernel function: todo equation as comment
		compact support within circular disk from 0 to supportRange R
		f(R) = 0
		f'(R) = 0
		f'(0) = 0
		area integral = 1
		This function is for evaluation of circular 2D kernels. */
	Real getKernel2DPoly3(const Real r, const Real supportRange);

	/** todo
		spiky, polynomial, multi-scale weighting function for circular disks
		compute polynomial kernel function: todo
		compact support within sphere from 0 to supportRange R
		f(R) = 0
		f'(R) = 0
		volume integral = 1
		This function is for evaluation of circular 2D kernels.
		It is very similar to getKernel2DPoly3 but with higher weights more towards the kernel center and a spike in the kernel center.
		The kernel is from this publication:
		{Particle-based Fluid Simulation for Interactive Applications}, Muller, SCA 2003, http://dl.acm.org/citation.cfm?id=846276.846298
	@param r Set this to the distance to the kernel center.
	@param supportRange todo*/
	Real getKernel2DPoly3Spiky(const Real r, const Real supportRange);
		
	/** todo
		polynomial, multi-scale weighting function for circular disks
		compute polynomial kernel function: todo
		compact support within circular disk from 0 to supportRange R
		f(R) = 0
		f'(R) = 0
		f'(0) = 0
		volume integral = 1
		This function is for evaluation of circular 2D kernels.
		It is very similar to getKernel2DPoly3 but with higher weights more towards the kernel center.
		The kernel is from this publication:
		{Particle-based Fluid Simulation for Interactive Applications}, Muller, SCA 2003, http://dl.acm.org/citation.cfm?id=846276.846298
	@param rSq Set this to the squared distance to the kernel center.
	@param supportRange todo*/
	Real getKernel2DPoly6FromSquared(const Real rSq, const Real supportRange);

	/** todo
		polynomial, multi-scale weighting function for spheres
		compute polynomial kernel function: f_s(x) = (1 / (4 pi)) * (30 * R^-6 * r^3 - 45 * R^-5 * r^2 + 15 * R^-3)
		compact support within sphere from 0 to supportRange R
		f(R) = 0
		f'(R) = 0
		f'(0) = 0
		volume integral = 1
		This function is for evaluation of spherical 3D kernels. */
	Real getKernel3DPoly3(const Real r, const Real supportRange);

	/** todo
		spiky, polynomial, multi-scale weighting function for spheres
		compute polynomial kernel function: todo
		compact support within sphere from 0 to supportRange R
		f(R) = 0
		f'(R) = 0
		volume integral = 1
		This function is for evaluation of spherical 3D kernels.
		It is very similar to getKernel3DPoly3 but with higher weights more towards the kernel center and a spike in the kernel center.
		The kernel is from this publication:
		{Particle-based Fluid Simulation for Interactive Applications}, Muller, SCA 2003, http://dl.acm.org/citation.cfm?id=846276.846298
	@param r Set this to the distance to the kernel center.
	@param supportRange todo*/
	Real getKernel3DPoly3Spiky(const Real r, const Real supportRange);

	/** todo
		polynomial, multi-scale weighting function for spheres
		compute polynomial kernel function: todo
		compact support within sphere from 0 to supportRange R
		f(R) = 0
		f'(R) = 0
		f'(0) = 0
		volume integral = 1
		This function is for evaluation of spherical 3D kernels.
		It is very similar to getKernel3DPoly3 but with higher weights more towards the kernel center.
		The kernel is from this publication:
		{Particle-based Fluid Simulation for Interactive Applications}, Muller, SCA 2003, http://dl.acm.org/citation.cfm?id=846276.846298
	@param rSq Set this to the squared distance to the kernel center.
	@param supportRange todo*/
	Real getKernel3DPoly6FromSquared(const Real rSq, const Real supportRange);

	/** This function starts at zero with zero, reaches c at x = maximum / 2 and asymptotically reaches infinity at maximum.
		Can be used as cost function with maximum costs at x = maximum and fixed costs c at x = maximum / 2.
		Computes
		\n a) f(x) = 0 if x <= 0
		\n b) f(x) = c * (x / (maximum - x)) x > 0 && x < maximum
		\n c) f(x) = inifinity = REAL_MAX if x >= maximum
	@param x Set this to the x argument you want to get the function value for.
	@param maximum The function reaches infinity (REAL_MAX) at maximum.
	@param c This parameter controls the steepness of the function with f(x = maximum / 2) = c. */
	inline Real infinityMaximumCurve(const Real x, const Real maximum, const Real c = 1.0f);

	/** Does bilinear interpolation between the 4 provided values.
	@param values The entered 4 values are assumed to positioned in a rectangle as follows: [0] -> lower left, [1] -> lower right, [2] -> upper left, [3] -> upper right.
	@param factors These interpolation weights determine how much of each value is merged into the returned result.
		(factors = {0, 0} -> values[0], = {1, 0} -> values[1], = { 0, 1 } -> values[2], = { 1, 1 } -> values[3], = { 0.5, 0.5 } -> sum of values / 4)
	@return The returned value is the result of the bilinear interpolation of the entered 4 values with the coefficients factors. */
	template <class T>
	T interpolateBilinearly(const T values[4], const Real factors[2])
	{
		assert(factors[0] >= 0.0f);
		assert(factors[0] <= 1.0f);
		assert(factors[1] >= 0.0f);
		assert(factors[1] <= 1.0f);

		// 2x interpolation along x-axis
		Real f0 = 1.0f - factors[0];
		T temp0 = f0 * values[0] + factors[0] * values[1];
		T temp1 = f0 * values[2] + factors[0] * values[3];

		// return interpolation along y-axis of interpolation results
		f0 = 1.0f - factors[1];
		return f0 * temp0 + factors[1] * temp1;
	}

	/** Computes a truncated polynomial weighting function with the following properties:
		1) f(r) = 0
		2) f'(r) = g
		3) f'(0) = 0
		4) integral(0, r){f(x) * dx} = 1
		5)	f(x) = 0 if x <= 0 || x >= r \n
			f(x) = (4/r^4 + 4g/(3r^2))x^3 - (6/r^3 + 3g/(2r))x^2 + (2/r) + (1/6)rg
	@param x This defines where the weighting function is evaluated.
	@param r Defines the range or length of the weighting function which starts at x=0 and ends at x=r. The function returns 0 outside.
	@param g Set this to the gradient of f you want to have at x == r. (f(r) = g, see function properties)
	@return Returns the value of the following function:
		f(x) = 0 if x <= 0 || x >= R \n
		f(x) = (4/r^4 + 4g/(3r^2))x^3 - (6/r^3 + 3g/(2r))x^2 + (2/r) + (1/6)rg */
	Real normalizedPolynomialWeighting(const Real x, const Real r, const Real g);

	/** The smooth cosine function curve can be used for interpolation of animations or similar tasks.
		Instead of only using a time factor t, the function can be applied to t to be used as interpolation factor since
		the function starts and ends with a slope of zero, it is continuous and monotonically increasing.\n 
		function: 0.5 * (1 - cosr(PI * x) \n
		domain: [0,1] \n
		codomain: [0, 1] \n
	@param xThe input value x should be element of the interval [0, 1]. </param>
	@return The returned value is element of the interval [0,1] if the input value x is element of the interval [0,1]*/
	inline Real smoothCosFCurve(Real x);

	/** This polynomial function f(x) = 2/r^3 * x^3 - 3/r^2 * x^2 + 1 with r as range can be used for one dimensional blending or weighting within a certain distance.
	It has the following properties:
		f(0) = 1
		f'(0) = 0
		f(r) = 0
		f'(r) = 0
		f(x) = 0 if x >= r.
	This means that f smoothly goes to 1 at x=0 and it smoothly reaches 0 at x=range.
	@param x Set this to the value where you want to evaluate the function. x should be positive if you want to use this for blending or weighting.
	@param range This defines at what point the function smoothly reaches 0.
	@return The returned value f(x) = 2/range^3 * x^3 - 3/range^2 * x^2 + 1. */
	inline Real smoothPolynomialFCurve(Real x, Real range);

	/** Computes the values of the polynomial weighting function f(x^2) = (15/16) * r^-5 * x^4 - (15/8) * r^-3 * x^2 + (15/16) * r^-1 for x \in [0, r].
	@param x2 Set this to the squared function position (x2 = x * x) where you want to evaluate the weighting function f(x).
	@param range This value defines the range of the positive function values of f(x). It smoothly starts and ends at f(x = -r) = 0 and f(x = r) = 0.
	@see See smoothNormalizedPolynomialWeighting for properties of this function. */
	Real smoothNormalizedPolynomialWeightingFromXSq(Real x2, Real range);

	/** This polynomial weighting function f(x) = (15/16) * r^-5 * x^4 - (15/8) * r^-3 * x^2 + (15/16) * r^-1 has the following properties:
		\n1. integral(-r, r){f(x) * dx} = 1
		\n2. f(-r) = 0
		\n3. f(r) = 0
		\n4. df/dx(r) = 0
		\n5. df/dx(-r) = 0
		\n Basically, it is a symmetric polynomial weighting function which sums up to 1 over its domain and it smoothly starts and ends at x = -r and x = r.
	@param x Set this to the function evaluation position x to get f(x). If x <= -r or x >= r then f(x) = 0 otherwise f(x) > 0.
	@param range This value defines the range of the positive function values of f(x). It smoothly starts and ends at f(x = -r) = 0 and f(x = r) = 0.
		If x \not \in [-r, r] then zero is returned. */
	inline Real smoothNormalizedPolynomialWeighting(Real x, Real range);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline Real getGaussian1D(const Real x, const Real stdDev)
	{
		return getGaussian1DForSquaredDistance(x * x, stdDev);
	}
	
	inline Real getGaussian2D(const Real x, const Real y, const Real stdDev)
	{
		return getGaussian2DForSquaredDistance(x * x + y * y, stdDev);
	}

	inline Real infinityMaximumCurve(const Real x, const Real maximum, const Real c)
	{
		if (x <= 0.0f)
			return 0.0f;
		if (x >= maximum)
			return REAL_MAX;

		const Real temp = x / (maximum - x);
		return c * temp;
	}

	inline Real smoothCosFCurve(Real x)
	{
		return 0.5f * (1.0f - cosr(PI * x));
	}

	inline Real smoothPolynomialFCurve(Real x, Real range)
	{
		assert(x >= 0.0f);
		if (x >= range || x < 0.0f)
			return 0.0f;

		Real rangeSq = range * range;
		Real xSq  = x * x;

		return 2.0f / (rangeSq * range) * (xSq * x)	- 3 / rangeSq * xSq	+ 1;
	}

	inline Real smoothNormalizedPolynomialWeighting(Real x, Real r)
	{
		return smoothNormalizedPolynomialWeightingFromXSq(x * x, r);
	}
}

#endif // _KERNELS_INTERPOLATIONS_WEIGHTINGS_H_
