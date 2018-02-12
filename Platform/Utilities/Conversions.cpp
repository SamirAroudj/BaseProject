/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#include "Platform/Utilities/Conversions.h"

using namespace Utilities;
using namespace std;

bool Utilities::convertToBoolean(const string &text)
{
	return text == "t" || text == "true" || text == "yes" || text == "y" || text == "1";
}
