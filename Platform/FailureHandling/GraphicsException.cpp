/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "GraphicsException.h"

using namespace FailureHandling;
using namespace std;

ostream &FailureHandling::operator<<(ostream &os, const GraphicsException &exception)
{
	os << "Graphic exception: ";
	os << "\nMessage: " << exception.getMessage();
	os << "\nError code: " << exception.getErrorCode();
	return os;
}