/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include "Platform/DataTypes.h"

namespace Math
{
	class Statistics
	{
	public:
		/** Computes the parameters of the equation: parameters^t = y^t * X * (X^t * X)^-1
			Caution! DirectX convention!
			(This is the same as parameters = (X^t * X)^-1 * X^t * y but it is based on the DirectX convention.)
			todo
			todo layout within X row major
		@param parameters Caution: intialize these with some reasonable values / intial guess
		@param X
		@param y
		@param observationCount
		@param parameterCount */
		static void getMLSEFit(Real *parameters, const Real *XTranspose, const Real *y, const uint32 observationCount, const uint32 parameterCount);
	};
}

#endif // _STATISTICS_H_
