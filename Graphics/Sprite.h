/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "Graphics/IRenderable.h"
#include "Graphics/MagicConstants.h"
#include "Graphics/Texture.h"
#include "Math/Vector2.h"

namespace Graphics
{
	/// A sprite is a 2D-quad which shows a tile of a texture and it can be transformed in 2D-space.
	class Sprite : public IRenderable
	{
	public:
		/** Defines what kind of Sprites are implemented / supported. */
		enum SpriteType
		{
			ANIMATED_SPRITE,		/// see AnimatedSprite.h
			SPRITE					/// see Sprite.h
		};

		/** Creates a sprite (simple 2D-object) that shows a part ( = tile) of a texture.
		@param textureName This is the name of the texture resource to be used (without path).
		@param tileIndex Specify the index of the tile in the used texture. (It's a grid alignment. 0 = bottom, left, n = top, right) */
		Sprite(const std::string &textureName, uint32 tileIndex = 0);

		/** Delete the sprite and end usage of its texture. */
		virtual ~Sprite();

		/** Returns its rotation around the z-axis.
		@return The returned value is the angle of rotation around the z-axis in radians. */
		Real getAngle() const { return mAngle; }

		/** Returns the depth (position on the z-axis) of the sprite.
		@return Returned value € [0, ORTHO_PROJECTION_Z_FAR). (0 = front) */
		Real getDepth() const { return mDepth; }

		/** Returns the pivot point which is relative to the sprite center. ([0/0] = center, [-0.5/-0.5] = left bottom, [0.5, 0.5] = top right)
		@return The returned pivot point is relative to the sprite center. */
		const Math::Vector2	&getPivot() const { return mPivot; }

		/** Returns the position of the sprite in world space. The position refers to the pivot point of the sprite. (not the center)
		@return Returns the position of the sprite in world coordinates. (refers to the pivot point) */
		const Math::Vector2 &getPosition() const { return mPosition; }

		/** Returns scale of the sprite whereas [1,1] is the default value. Scale is relative to sprite's own axes.
		@return The returned vector contains the scale along sprites x-axis and y-axis. */
		const Math::Vector2 &getScaling() const { return mScaling; }

		/** Queries the actual type of the Sprite object.
		@return Returns SPRITE. */
		virtual SpriteType GetType() const { return SPRITE; }

		/** Draw the sprite. That means its transformation matrix is computed and pushed.
			Then, a textured quad is rendered and sprite's transformation is released. */
		virtual void render() const;

		/** Adds a rotation to the orientation of the sprite.
		@param deltaAngle This is the rotation angle in radians which is added to the current angle of the sprite. */
		void rotate(Real deltaAngle) { mAngle += deltaAngle; }

		/** Scales this Sprite object relative to its own coordinate axis.
		@param factor this.scaleX *= factor.x and this.scaleY *= factor.Y. */
		void scale(const Math::Vector2 &factor) { mScaling.x *= factor.x; mScaling.y *= factor.y; }

		/** Specifiy the orientation of the sprite (angle of rotation around the z-axis).
		@param angle An positive angle (radian measure) specifies a counterclockwise rotation from the default orientation. */
		void setAngle(Real angle) { mAngle = angle; }

		/** Sets the position of the sprite on the z-axis. Choose zero to draw the sprite in front of the scene.
		@param depth The entered value specifies the distance on the z-axis. (depth € [0, ORTHO_PROJECTION_Z_FAR); 0 = front, Z_FAR = back)*/
		void setDepth(Real depth) { assert(depth >= 0); assert(depth < Graphics::ORTHO_PROJECTION_Z_FAR); mDepth = depth; }

		/** Set the pivot point rotation and position transformations refer to. Pivot point is relative to sprite's center.
		@param newPivotPoint The entered vector is relative to sprite's center. ([0/0] = center, [-0.5/-0.5] = left bottom, [0.5, 0.5] = top right)*/
		void setPivot(const Math::Vector2 &newPivotPoint) { mPivot = newPivotPoint; }

		/** Set sprite's world coordinates to newPosition whereas the position refers to the pivot point of the sprite.
		@param newPosition Specifies the new position of the sprite in world space. Sprite position refers to the pivot point of the sprite. */
		void setPosition(const Math::Vector2 &newPosition) { mPosition = newPosition; }

		/** Change the size of the sprite. The entered vector is relative to the original coordinate axis of the sprite.
		@param scaling The entered vector specifies scale along sprite's own x-axis and y-axis. */
		void setScaling(const Math::Vector2 &scaling) { mScaling = scaling; }

		/** Choose the tile to be rendered. Only this tile is drawn. Tiles are placed in grid order in the texture.
		@param index Index specifies which tile is rendered. (0 = bottom left tile; numOfTiles - 1 = top right tile)*/
		void setTileIndex(uint32 index);

	protected:
		Texture *mTexture;			/// the texture to be used containing the tiles which are actually shown
		Math::Vector2 mPivot;		/// default: middle of a sprite; rotation, position, scale refer to this point
		Math::Vector2 mPosition;	/// position which refers to the pivot point
		Math::Vector2 mScaling;		/// relative to pivot, (1.0, 1.0) = standard
		Real mAngle;				/// rotation around pivot point in radians
		Real mDepth;				/// 0.0f = front
		Real mTileX;				/// x-coordinate of the currently used tile in mTexture
		Real mTileY;				/// y-coordinate of the currently used tile in mTexture

		/** Copy constructor is forbidden. */
		Sprite(const Sprite &copy) { assert(false); }

		/** asignment operator is forbidden. */
		const Sprite &operator =(const Sprite &rhs) { assert(false); return *this; }
	};
}

#endif // _SPRITE_H_
