/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _GIT_H_
#define _GIT_H_

#include <string>

namespace Git
{
	extern const char gBranch[];
	extern const char gCommitHash[];

	std::string getStatus();
}

#endif // _GIT_H_