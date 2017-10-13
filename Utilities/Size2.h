/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _UTILITIES_SIZE_2_H_
#define _UTILITIES_SIZE_2_H_

#include "Platform/DataTypes.h"

namespace Utilities
{
	template <class T>
	struct Size2
	{
	public:
		Size2()
		{
			mDimensions[0] = 0;
			mDimensions[1] = 0;
		}

		Size2(const T size[2])
		{
			mDimensions[0] = size[0];
			mDimensions[1] = size[1];
		}

		Size2(const T &width, const T &height)
		{
			mDimensions[0] = width;
			mDimensions[1] = height;
		}

		inline T &operator[](size_t index)
		{
			return mDimensions[index];
		}

		inline const T &operator[](size_t index) const
		{
			return mDimensions[index];
		}

		inline T getElementCount() const
		{
			return getSize();
		}

		inline T getSize() const
		{
			return mDimensions[0] * mDimensions[1];
		}

		inline void set(const T size[2])
		{
			mDimensions[0] = size[0];
			mDimensions[1] = size[1];
		}

		inline void set(const T &width, const T &height)
		{
			mDimensions[0] = width;
			mDimensions[1] = height;
		}

	public:
		T mDimensions[2];
	};

	typedef Size2<uint32> ImgSize;
}

#endif // _UTILITIES_SIZE_2_H_
