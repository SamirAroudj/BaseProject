/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _COLOR_MAP_H_
#define _COLOR_MAP_H_

#include "Graphics/Color.h"

namespace Graphics
{
	class ColorMap
	{
	public:
		/** Returns a color of the VIRIDIS color map whereas the return color is linearly interpolated between the two closest color buckets between which x falls.
		@param x Set this to a value \in [0, 255]. Values outside are clamped to [0, 255].
			The returned color is linearly interpolated, e.g., x = 1,75 -> color = VIRIDIS[1] * 0.25 + VIRIDS[2] * 0.75.
		@return color Contains the VIRIDIS color (linearly interpolated) to which maps x. */
		static Color getViridisColorLinearly(const Real x);

	public:
		static const Color VIRIDIS[256]; /// VIRIDIS color map, see https://bids.github.io/colormap/
	};
}


#endif // _COLOR_MAP_H_