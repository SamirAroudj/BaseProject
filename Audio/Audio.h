/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "fmod_errors.h"

namespace Audio
{
	// throws an AudioException containing a description on error
	void errorCheck(FMOD_RESULT result);
}

#endif // _AUDIO_H_
