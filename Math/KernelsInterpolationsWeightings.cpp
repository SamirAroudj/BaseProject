/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cassert>
#include <cmath>
#include "Math/KernelsInterpolationsWeightings.h"

using namespace Math;

Real Math::getGaussian1DForSquaredDistance(const Real dSq, const Real stdDev)
{
	// check entered values
	assert(stdDev > 0.0f);
	if (stdDev <= 0.0f)
		return 0.0f;

	const Real variance	= stdDev * stdDev;
	const Real normTerm = stdDev * Math::SQRT_TWO_PI;

	return expr(- 0.5f * dSq / variance) / normTerm;
}

Real Math::getGaussian2DDerivative(const Real derivativeDistance, const Real orthogonalDistance, const Real stdDev)
{
	Real x = derivativeDistance;
	Real y = orthogonalDistance;

	Real rSq		= x * x + y * y;
	Real variance	= stdDev * stdDev;
	Real normTerm	= Math::TWO_PI * variance * variance;

	return -x * expr(- 0.5f * rSq / variance) / normTerm;
}

Real Math::getGaussian2DForSquaredDistance(const Real dSq, const Real stdDev)
{
	// check entered values
	assert(stdDev > 0.0f);
	if (stdDev <= 0.0f)
		return 0.0f;

	const Real variance	= stdDev * stdDev;
	const Real normTerm = variance * Math::TWO_PI;

	return expr(- 0.5f * dSq / variance) / normTerm;
}

Real Math::getGaussian3D(const Vector3 &p, const Real stdDev)
{
	// check entered values
	assert(stdDev > 0.0f);
	if (stdDev <= 0.0f)
		return 0.0f;

	// derivative of normalized 3D Gaussian g(p) with standard deviation stdDev w.r.t. axis derivativeDir
	const Real variance		= stdDev * stdDev;
	const Real denominator	= sqrtr(Math::TWO_PI * Math::TWO_PI * Math::TWO_PI) * variance * stdDev;
	const Real numerator	= expr(-0.5f * p.getLengthSquared() / variance);

	return numerator / denominator;
}

Real Math::getGaussian3DDerivative(const Math::Vector3 &p, const Axis derivativeDir, const Real stdDev)
{
	// check entered values
	assert(stdDev > 0.0f);
	if (stdDev <= 0.0f)
		return 0.0f;

	assert(derivativeDir <= 2);
	if (derivativeDir > 2)
		return 0.0f;

	// derivative of normalized 3D Gaussian g(p) with standard deviation stdDev w.r.t. axis derivativeDir
	const Real variance		= stdDev * stdDev;
	const Real denominator	= sqrtr(Math::TWO_PI * Math::TWO_PI * Math::TWO_PI) * variance * variance * stdDev;
	const Real numerator	= -p[derivativeDir] * expr(-0.5f * p.getLengthSquared() / variance);

	return numerator / denominator;
}

Real Math::getKernel1DPoly3(const Real r, const Real supportRange)
{
	// within support range?
	if (r <= 0.0f || r >= supportRange)
		return 0.0f;
	
	// required products
	const Real R = supportRange;	
	const Real R2 = R * R;
	const Real R3 = R2 * R;
	const Real R4 = R2 * R2;
	const Real r2 = r * r;
	const Real r3 = r2 * r;

	// polynomial multi-scale kernel function for line segments (1D): f_R(r) = (4/R^4) * r^3 - (6/R^3) * r^2 + (2/R)
	const Real value = (4 / R4) * r3 - (6 / R3) * r2 + (2 / R);
	return value;
}

Real Math::getKernel2DPoly3(const Real r, const Real supportRange)
{
	// within support range?
	if (r <= 0.0f || r >= supportRange)
		return 0.0f;

	// required products
	const Real R = supportRange;
	const Real R2 = R * R;
	const Real R4 = R2 * R2;
	const Real R5 = R4 * R;
	const Real r2 = r * r;
	const Real r3 = r2 * r;
	const Real t = 10.0f / (3.0f * Math::PI);
	
	// polynomial multi-scale kernel function for circular disks (2D): f_R(r) =  (20 / (3 * PI * R^5)) * r^3 - (10 / (PI * R^4)) * r^2 + (10 / (3 * PI * R^2));
	const Real value = t * ((2.0f / R5) * r3 - (3.0f / R4) * r2 + (1.0f / R2));
	return value;
}

Real Math::getKernel2DPoly3Spiky(const Real r, const Real supportRange)
{
	// within support range?
	if (r <= 0.0f || r >= supportRange)
		return 0.0f;

	// required products
	const Real R = supportRange;
	
	// 10 / (pi * R^2)
	const Real normTerm = 10.0f / (Math::PI * R * R);
	
	// (1 - r / R)^3
	Real temp = 1 - r / R;
	temp = temp * temp * temp;

	// 10 / (pi * R^2) * (1 - r / R)^3
	const Real value = normTerm * temp;
	return value;
}

Real Math::getKernel3DPoly3Spiky(const Real r, const Real supportRange)
{
	// within support range?
	if (r <= 0.0f || r >= supportRange)
		return 0.0f;

	// required products
	const Real R = supportRange;
	const Real R2 = R * R;
	const Real R3 = R2 * R;

	// 15 / (pi * R^3)
	const Real normTerm = 15.0f / (Math::PI * R3);
	
	// (1 - r / R)^3
	Real temp = 1 - r / R;
	temp = temp * temp * temp;

	// 15 / (pi * R^3) * (1 - r / R)^3
	const Real value = normTerm * temp;
	return value;
}

Real Math::getKernel2DPoly6FromSquared(const Real rSq, const Real supportRange)
{
	const Real R = supportRange;
	const Real RSq = R * R;

	// within support range?
	if (rSq >= RSq)
		return 0.0f;

	// 4 / (pi * R^2)
	const Real normTerm = 4.0f / (Math::PI * RSq);

	// (1 - r^2 / R^2)^3
	Real temp = (1.0f - rSq / RSq);
	temp = temp * temp * temp;

	// 4 / (pi * R^2) * (1 - r^2 / R^2)^3
	const Real value = normTerm * temp;
	return value;
}

Real Math::getKernel3DPoly3(const Real r, const Real supportRange)
{
	// within support range?
	if (r <= 0.0f || r >= supportRange)
		return 0.0f;

	// required products
	const Real R = supportRange;
	const Real R2 = R * R;
	const Real R3 = R2 * R;
	const Real R5 = R3 * R2;
	const Real R6 = R3 * R3;

	const Real r2 = r * r;
	const Real r3 = r2 * r;
		
	// compute polynomial kernel function for 3D: f_R(r) = (1 / (4 pi)) * (30 * R^-6 * r^3 - 45 * R^-5 * r^2 + 15 * R^-3)
	const Real temp = (30 / R6) * r3 -(45 / R5) * r2 + (15 / R3);
	const Real value = temp / (4.0f * Math::PI);

	return value;
}

Real Math::getKernel3DPoly6FromSquared(const Real rSq, const Real supportRange)
{
	const Real R = supportRange;
	const Real RSq = R * R;

	// within support range?
	if (rSq >= RSq)
		return 0.0f;
	
	// required products
	const Real R3 = RSq * R;

	// 315 / (64 * pi * R^3)
	const Real normTerm = 315.0f / ((64.0f * Math::PI) * R3);

	// (1 - (r^2) / (R^2))^3
	Real temp = 1.0f - rSq / RSq;
	temp = temp * temp * temp;

	// 315 / (64 * pi * R^3) * (1 - (r^2) / (R^2))^3
	const Real value = normTerm * temp;
	return value;
}

Real Math::normalizedPolynomialWeighting(const Real x, const Real r, const Real g)
{
	// only compact support for x \in [0, r]
	if (x <= 0.0f || x >= r)
		return 0.0f;

	const Real r2 = r * r;
	const Real r3 = r2 * r;
	const Real r4 = r2 * r2;
	const Real x2 = x * x;
	const Real x3 = x2 * x;

	return
	  (4 / r4 + 4 * g / (3 * r2)) * x3
	- (6 / r3 + 3 * g / (2 * r)) * x2
	+ (2 / r) + (1 / 6) * r * g;
}

Real Math::smoothNormalizedPolynomialWeightingFromXSq(Real x2, Real r)
{
	Real r2 = r * r;
	if (x2 >= r2)
		return 0.0f;

	Real r4 = r2 * r2;
	Real x4 = x2 * x2;

	return	  x4 * ((15.0f / 16.0f) / (r4 * r))
			- x2 * ((15.0f / 8.0f)  / (r2 * r))
			+      ((15.0f / 16.0f) / (r));
}