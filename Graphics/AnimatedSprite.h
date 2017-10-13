/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _ANIMATED_SPRITE_H_
#define _ANIMATED_SPRITE_H_

#include "Graphics/Sprite.h"

namespace Graphics
{
	/// An animated sprite uses several texture tiles rendered in turn.
	class AnimatedSprite : public Sprite
	{
	public:
		/** Creates a sprite which uses a texture.
		@param textureName This is the name of the texture resource to be used. (without path)
		@param tileIndex Specify the start tile index of the tile in the used texture.
			   (It's a grid alignment. 0 = bottom, left, n = top, right)
		@param timePerTile Specifies the time in milliseconds a single tile is shown*/
		AnimatedSprite(const std::string &textureName, uint32 tileIndex = 0, Real timePerTile = 0.040f);

		/** Destroy the sprite and end usage of its texture. */
		~AnimatedSprite();

		/** Queries the actual type of the Sprite object.
		@return Returns ANIMATED_SPRITE. */
		virtual SpriteType GetType() const { return ANIMATED_SPRITE; }

		/** Calculate the tile of the texture to be shown. */
		void update();

	private:
		const Real	mTimePerTile;		/// contains how long a tile is shown (in ms)
		Real		mTimeTileShown;		/// contains how long the current tile has been shown
		
		/** Copy constructor is forbidden. */
		AnimatedSprite(const AnimatedSprite &copy) : Sprite("", 0), mTimePerTile(0.0f) { assert(false); }

		/** asignment operator is forbidden. */
		const AnimatedSprite &operator =(const AnimatedSprite &rhs) { assert(false); return *this; }
	};
}

#endif // _ANIMATED_SPRITE_H_
