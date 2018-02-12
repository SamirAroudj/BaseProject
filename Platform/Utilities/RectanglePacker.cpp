/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cstring>
#include <limits>
#include "RectanglePacker.h"

using namespace Utilities;
using namespace std;

bool RectanglePacker::packRectangles(int32 binWidth, int32 binHeight, int32 numRectangles,
									 RectangleSize *sizes, RectangleCoords *coords)
{
	size_t bestMaxRectIndex	= -1;
	size_t bestRectIndex	= -1;

	memset(coords, -1, numRectangles * 2 * sizeof(int32));											//no known coords, 1 max rect which entirely occupies the bin
	mMaxRects.push_back(BoundingBox(0, 0, binWidth, binHeight));

	
	for (int32 numPlacedRectangles = 0; numPlacedRectangles < numRectangles; ++numPlacedRectangles)	// find a position for each rectangle
	{
		findBestChoice(numRectangles, sizes, coords, bestMaxRectIndex, bestRectIndex);
		if ((-1 == bestMaxRectIndex) || (-1 == bestRectIndex))
			return false;

		BoundingBox usedSpace = placeRectIntoBin(sizes, coords, bestMaxRectIndex, bestRectIndex);
		subdivideMaxRects(usedSpace);
		processPendingRects();
	}

	return true;
}

void RectanglePacker::findBestChoice(int32 numRectangles,
									 RectangleSize *sizes, RectangleCoords *coords,
									 size_t &bestMaxRectIndex, size_t &bestRectIndex)
{
	size_t	numMaxRects	= mMaxRects.size();
	int32	bestValue	= numeric_limits<int>::max();													// smallest min(maxRectW - rectW, maxRectH - rectH)
	bestMaxRectIndex	= -1;
	bestRectIndex		= -1;

	for (int32 rectCounter = 0; rectCounter < numRectangles; ++rectCounter)
	{
		RectangleCoords position = coords[rectCounter];													// was the rectangle already placed?
		if (position.mX != -1)
			continue;

		RectangleSize rectSize	  = sizes[rectCounter];													// check this rect

		for (size_t maxRectCounter = 0; maxRectCounter < numMaxRects; ++maxRectCounter)
		{
			BoundingBox maxRect = mMaxRects[maxRectCounter];

			int32 diffWidth	  = maxRect.mWidth  - rectSize.mWidth;												// better value than best value?
			int32 diffHeight  = maxRect.mHeight - rectSize.mHeight;
			int32 shorterDiff = (diffWidth < diffHeight ? diffWidth : diffHeight);	

			if ((shorterDiff < 0) || (shorterDiff >= bestValue))
				continue;

			bestValue		 = shorterDiff;																		// new best choice
			bestMaxRectIndex = maxRectCounter;
			bestRectIndex	 = rectCounter;
		}
	}
}

BoundingBox RectanglePacker::placeRectIntoBin(RectangleSize *sizes, RectangleCoords *coords,
											  size_t bestMaxRectIndex, size_t bestRectIndex)
{
	BoundingBox chosenMaxRect	= mMaxRects[bestMaxRectIndex];										// get and remove chosen max rect
	mMaxRects[bestMaxRectIndex] = mMaxRects.back();
	mMaxRects.pop_back();

	RectangleSize rect = sizes[bestRectIndex];														// add rects for remaining space of used max rect

	int32 diffWidth = chosenMaxRect.mWidth - rect.mWidth;												// new max rect on the right side of the placed rect
	if (diffWidth > 0)
	{
		mPendingRects.push_back(BoundingBox(chosenMaxRect.mX + rect.mWidth, chosenMaxRect.mY,
											diffWidth, chosenMaxRect.mHeight));
	}

	int32 diffHeight = chosenMaxRect.mHeight - rect.mHeight;											// new max rect on top of the placed rect
	if (diffHeight > 0)
	{
		mPendingRects.push_back(BoundingBox(chosenMaxRect.mX, chosenMaxRect.mY + rect.mHeight,
											chosenMaxRect.mWidth, diffHeight));
	}

	coords[bestRectIndex] = RectangleCoords(chosenMaxRect.mX, chosenMaxRect.mY);					// save the coords of the placed rectangle

	return BoundingBox(chosenMaxRect.mX, chosenMaxRect.mY, rect.mWidth, rect.mHeight);
}

void RectanglePacker::subdivideMaxRects(const BoundingBox &usedSpace)
{
	size_t numMaxRects = mMaxRects.size();
	for (size_t i = 0; i < numMaxRects;)
	{
		BoundingBox maxRect = mMaxRects[i];

		int32 maxRectRight1		= maxRect.mX + maxRect.mWidth;										// right + 1 and top + 1 of the bounding boxes
		int32 maxRectTop1		= maxRect.mY + maxRect.mHeight;										// maxRectTop1 - 1 is the topmost line belonging to max rect 
		int32 usedSpaceRight1	= usedSpace.mX + usedSpace.mWidth;
		int32 usedSpaceTop1		= usedSpace.mY + usedSpace.mHeight;
																	
		if (usedSpace.mX >= maxRectRight1 || usedSpace.mY >= maxRectTop1 ||							// skip it if max rect is not intersected
		   maxRect.mX >= usedSpaceRight1 || maxRect.mY >= usedSpaceTop1)
		{
			++i;
			continue;
		}
			
		mMaxRects[i] = mMaxRects.back();															// remove it since its space is not completely free anymore
		mMaxRects.pop_back();
		--numMaxRects;
																									// there are up to 4 new max rects: left
		int32 width = usedSpace.mX - maxRect.mX;														// left rectangle
		if (width > 0)
			mPendingRects.push_back(BoundingBox(maxRect.mX, maxRect.mY, width, maxRect.mHeight));

		int32 height = usedSpace.mY - maxRect.mY;
		if (height > 0)																					// bottom rectangle
			mPendingRects.push_back(BoundingBox(maxRect.mX, maxRect.mY, maxRect.mWidth, height));

		width = maxRectRight1 - usedSpaceRight1;														// right rectangle
		if (width > 0)
			mPendingRects.push_back(BoundingBox(usedSpaceRight1, maxRect.mY, width, maxRect.mHeight));

		height = maxRectTop1 - usedSpaceTop1;															// top rectangle
		if (height > 0)
			mPendingRects.push_back(BoundingBox(maxRect.mX, usedSpaceTop1, maxRect.mWidth, height));
	}
}

void RectanglePacker::processPendingRects()
{
	size_t numMaxRects = mMaxRects.size();

	for (size_t i = 0; i < numMaxRects; ++i)															// is any intersection result rectangle (IRR) completely
	{																								// contained by a max rect?
		BoundingBox maxRect = mMaxRects[i];

		for (list<BoundingBox>::iterator it = mPendingRects.begin(); it != mPendingRects.end(); )
		{
			if (maxRect.contains(*it))																	// remove the IRR if it is not a maximum rectangle
			{
				it = mPendingRects.erase(it);
				continue;
			}

			++it;
		}
	}

	// TODO: looks dangerous - does this really work?
	for (list<BoundingBox>::iterator bb1 = mPendingRects.begin();									// does any IRR completely contain another IRR?
		bb1 != mPendingRects.end(); ++bb1)
	{
		for (list<BoundingBox>::iterator bb2 = mPendingRects.begin();
			bb2 != mPendingRects.end(); )
		{
			if ((bb1 != bb2) && bb1->contains(*bb2))
			{
				bb2 = mPendingRects.erase(bb2);
				continue;
			}

			++bb2;
		}
	}

	for (list<BoundingBox>::iterator it = mPendingRects.begin(); it != mPendingRects.end(); ++it)	// add all IRRs which are max rects to the mMaxRects													
		mMaxRects.push_back(*it);
	mPendingRects.resize(0);
}
