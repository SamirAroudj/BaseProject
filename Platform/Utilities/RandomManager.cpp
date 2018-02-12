/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Platform/Utilities/RandomManager.h"

using namespace std;
using namespace Utilities;

RandomManager::RandomManager(uint32 seedValue) :
	mGenerator(seedValue), mSeed(seedValue)
{
}

void RandomManager::setSeed(uint32 seed)
{
	mGenerator = mt19937_64(seed);
}
