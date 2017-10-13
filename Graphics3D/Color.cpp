/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Color.h"

using namespace Graphics;

Color::Color(Real red, Real green, Real blue, Real alpha)
{
	assert(checkComponents(red, green, blue, alpha));
	mComponents[0] = red;
	mComponents[1] = green;
	mComponents[2] = blue;
	mComponents[3] = alpha;
}

void Color::set(Real red, Real green, Real blue, Real alpha)
{
	assert(checkComponents(red, green, blue, alpha));
	mComponents[0] = red;
	mComponents[1] = green;
	mComponents[2] = blue;
	mComponents[3] = alpha;
}		

#ifdef _DEBUG
bool Color::checkComponents(Real red, Real green, Real blue, Real alpha)
{
	return checkComponent(red) && checkComponent(green) && checkComponent(blue) && checkComponent(alpha);
}

bool Color::checkComponent(Real component)
{
	return (0.0f <= component) && (component <= 1.0f);
}
#endif // _DEBUG