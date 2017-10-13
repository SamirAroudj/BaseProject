/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _RECTANGLE_PACKER_H_
#define _RECTANGLE_PACKER_H_

#include <cassert>
#include <list>
#include <vector>
#include "Platform/DataTypes.h"

namespace Utilities
{
	/** Represents an axis aligned rectangle with a position and a size. */
	struct BoundingBox
	{
		/** Sets every member to -1. */
		BoundingBox()
		{
			mX		= -1;
			mY		= -1;
			mWidth	= -1;
			mHeight	= -1;
		}

		/** Initializes the bounding box.
		@param x This is the x-coordinate of the bottom left corner. The edge at x belongs to the bounding box.
		@param y This is the y-coordinate of the bottom left corner. The edge at y belongs to the bounding box.
		@param width This is the width of the bounding box. x + width does not belong to the bounding box anymore
					 and x + width - 1 still belongs to it. Must be greater than zero.
		@param height This is the height of the bounding box. y + height does not belong to the bounding box anymore
					  and y + height - 1 still belongs to it. Must be greater than zero.*/
		BoundingBox(int32 x, int32 y, int32 width, int32 height)
		{
			assert(width > 0);
			assert(height > 0);

			mX		= x;
			mY		= y;
			mWidth	= width;
			mHeight	= height;
		}

		/** Returns true if this bounding box contains the other bounding box.
		@param other Tests whether other is contained by this bounding box.
		@return Returns true if this bounding box completely contains the entered bounding box.*/
		bool contains(BoundingBox &other)
		{
			return (other.mX >= mX) && (other.mY >= mY) &&
				   (other.mX + other.mWidth <= mX + mWidth) &&
				   (other.mY + other.mHeight <= mY + mHeight);
		}

		int32 mX;		/// x-coordinate of the bottom left corner, the edge at x belongs to the bounding box
		int32 mY;		/// y-coordinate of the bottom left corner, the edge at y belongs to the bounding box
		int32 mWidth;	/// width of the bounding box, x is inside, x + mWidth is outside the bounding box
		int32 mHeight;	/// height of the bounding box, y is inside, y + mHeight is outside the bounding box
	};

	/** Represents the position or x- and y-coordinates of a rectangle. */
	struct RectangleCoords
	{
		/** Initializes the coordinates.
		@param x This is the x-coordinate of the position.
		@param y This is the y-coordinate of the position. */
		RectangleCoords(int32 x, int32 y)
		{
			mX = x;
			mY = y;
		}

		int32 mX;	/// x-coordinate
		int32 mY;	/// y-coordinate
	};

	/** Represents an axis aligned rectangle of a specific size without position. */
	struct RectangleSize
	{
		/** Initializes the object.
		@param width This is the width of the rectangle. Must be greater than zero.
		@param height This is the height of the rectangle. Must be greater than zero.*/
		RectangleSize(int32 width, int32 height)
		{
			assert(width > 0);
			assert(height > 0);

			mWidth	= width;
			mHeight	= height;
		}

        int32 mWidth;		/// width of the rectangle, must be > 0
        int32 mHeight;		/// height of the rectangle, must be > 0
	};

    /// Realizes a bin packing algorithm to fit rectangles into a space which is as close as possible.
	class RectanglePacker
	{
	public:
		bool packRectangles(int32 binWidth, int32 binHeight, int32 numRectangles,
							RectangleSize *sizes, RectangleCoords *coords);

	protected:
		/**	Find the best choice: one of the rectangles placed in a free maximum rect -> heuristic: global best shortest side fit */
		void findBestChoice(int32 numRectangles, RectangleSize *sizes, RectangleCoords *coords,
			std::size_t &bestMaxRectIndex, std::size_t &bestRectIndex);

		BoundingBox placeRectIntoBin(RectangleSize *sizes, RectangleCoords *coords,
			std::size_t bestMaxRectIndex, std::size_t bestRectIndex);

		void processPendingRects();

		void subdivideMaxRects(const BoundingBox &occupiedSpace);


    protected:
		std::vector<BoundingBox> mMaxRects;
		std::list<BoundingBox>	 mPendingRects;
	};
}

#endif // _RECTANGLE_PACKER_H_

