/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Audio.h"
#include "Platform/ExceptionHandling/AudioException.h"

using namespace Audio;
using namespace ExceptionHandling;

void Audio::errorCheck(FMOD_RESULT result)
{
	if (FMOD_OK != result)
		throw AudioException(FMOD_ErrorString(result));
}
