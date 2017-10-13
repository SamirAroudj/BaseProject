/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Graphics/AnimatedSprite.h"
#include "Platform/ApplicationTimer.h"

using namespace Graphics;
using namespace Platform;
using namespace Utilities;

AnimatedSprite::AnimatedSprite(const std::string &textureName, uint32 tileIndex, Real timePerTile) :
	Sprite(textureName, tileIndex), mTimePerTile(timePerTile), mTimeTileShown(0.0f)
{
}

AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::update()
{
	mTimeTileShown += ApplicationTimer::getSingleton().getDeltaTime();

	int32 tileMovements = static_cast<int32>(mTimeTileShown / mTimePerTile);
	if (0 == tileMovements)
		return;

	// all tiles of an animation are in a single texture
	// the animation pictures are placed in grid order (pricture rows in a single texture)
	// animation is created by moving through this grid line by line
	mTimeTileShown -= tileMovements * mTimePerTile;
	const Size2<Real> &tileSize = mTexture->getTileSize();

	// seems to work - but looks like it is unstable and does not work as fast as the alternative following below (in the average case)
	/*
	int32 columns		 = 1.0f / tileWidth;
	int32 rows			= 1.0f / tileHeight;

	int32 x = mTileX / tileWidth;
	int32 y = mTileY / tileHeight;

	x = (x + tileMovements) % columns;
	y = (y + (int32)(tileMovements / rows)) % rows;

	mTileX = x * tileWidth;
	mTileY = y * tileHeight;*/


	for (int32 i = 0; i < tileMovements; ++i)	// does the sprite show the next tile?
	{
		// next tile to the right
		mTileX += tileSize[0];	

		// right border reached?
		if (mTileX < 1.0f)								
			continue;

		// then back to the left and on up
		mTileX = 0.0f;									
		mTileY += tileSize[1];

		// top border reached?
		if (mTileY < 1.0f)								
			continue;

		// then back to the bottom row
		mTileY = 0.0f;									
	}
}
