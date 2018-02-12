/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _LICENSER_H_
#define _LICENSER_H_

#include <string>
#include <vector>
#include "Platform/Storage/Path.h"

namespace Utilities
{
	class Licenser
	{
	public:
		Licenser(const Storage::Path &licenseFile);
		void setLicense(const Storage::Path &fileName);
		void setLicense(const Storage::Path &root, const std::vector<std::string> &fileTypes);
		
	private:
		std::vector<uint8> mContentBuffer;
		std::string mLicenseText;
	};
}

#endif // _LICENSER_H_
