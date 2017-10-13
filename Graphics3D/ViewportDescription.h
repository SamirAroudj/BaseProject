/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _VIEWPORT_DESCRIPTION_H_
#define _VIEWPORT_DESCRIPTION_H_

#include "Platform/DataTypes.h"
#include "MagicGraphicsConstants.h"

namespace Graphics
{
	/// The ViewportDescription class is used to define what part of the back buffer is used to show a perspective of the 3D scene. See also GraphicsManager.
	/** Usually a viewport covers the whole back buffer. But you can use several viewports for split screen games or special effects, such as a mirror for a racing game. */
	class ViewportDescription
	{
	public:
		/** Creates a rectangular viewport that is put on the back buffer.
		@param topLeftX The x coordinate of the upper left corner of the viewport relative to the back buffer. 
		@param topLeftY The y coordinate of the upper left corner of the viewport relative to the back buffer. 
		@param width The width of the viewport.
		@param height The height of the viewport.
		@param maxDepth The maximum depth of a pixel in the viewport, should normally be the same as the maximum depth value a depth buffer texel can contain.
		@param minDepth The minimum depth of a pixel in the viewport, should normally be the same as the minimum depth value a depth buffer texel can contain.*/
		ViewportDescription(uint32 topLeftX, uint32 topLeftY, uint32 width, uint32 height, Real maxDepth = DEPTH_BUFFER_MAX_DEPTH, Real minDepth = DEPTH_BUFFER_MIN_DEPTH);
		
		/** Obtain the maximum depth of a pixel in the viewport. Should usually be the same as the maximum depth value a depth buffer texel can contain.
		@returns the maximum depth value of a pixel in the viewport. */
		Real getMaxDepth() const { return mMaxDepth; }

		/** Obtain the minimum depth of a pixel in the viewport, should usually be the same as the minimum depth value a depth buffer texel can contain.
		@returns the minimum depth value of a pixel in the viewport. */
		Real getMinDepth() const { return mMinDepth; }

		/** Obtain the height of the viewport in pixel.
		@returns the number of pixel rows in this viewport. */
		uint32 getHeight() const { return mHeight; }

		/** Obtain the x coordinate of the upper left corner of the viewport relative to the back buffer.
		@returns the x coordinate of the upper left corner of the viewport relative to the back buffer. */
		uint32 getTopLeftX() const { return mTopLeftX; }

		/** Obtain the y coordinate of the upper left corner of the viewport relative to the back buffer.
		@returns the y coordinate of the upper left corner of the viewport relative to the back buffer. */
		uint32 getTopLeftY() const { return mTopLeftY; }

		/** Obtain the width of the viewport in pixel.
		@returns the number of columns of pixel in this viewport. */
		uint32 getWidth() const { return mWidth; }

		/** Set the maximum depth of a pixel in the viewport. Should usually be the same as the maximum depth value a depth buffer texel can contain.
		@param maxDepth the maximum depth value of a pixel in the viewport. */
		void setMaxDepth(Real maxDepth) { mMaxDepth = maxDepth; }
		
		/** Set the minimum depth of a pixel in the viewport, should usually be the same as the minimum depth value a depth buffer texel can contain.
		@param minDepth the minimum depth value of a pixel in the viewport. */
		void setMinDepth(Real minDepth) { mMinDepth = minDepth; }

		/** Set the height of the viewport in pixel.
		@param height the number of pixel rows in this viewport. */
		void setHeight(uint32 height) { mHeight = height; }

		/** Set the x coordinate of the upper left corner of the viewport relative to the back buffer.
		@param topLeftX the x coordinate of the upper left corner of the viewport relative to the back buffer. */
		void setTopLeftX(uint32 topLeftX) { mTopLeftX = topLeftX; }
		
		/** Set the y coordinate of the upper left corner of the viewport relative to the back buffer.
		@param topLeftY the y coordinate of the upper left corner of the viewport relative to the back buffer. */
		void setTopLeftY(uint32 topLeftY) { mTopLeftY = topLeftY; }

		/** Set the width of the viewport in pixel.
		@param width the number of columns of pixel in this viewport. */
		void setWidth(uint32 width) { mWidth = width; }

	private:
		/// maximum depth of the pixels
		Real mMaxDepth;

		/// minimum depth of the pixels
		Real mMinDepth;

		/// number of pixels along the y-axis
		uint32 mHeight;

		/// the x-coordinate of the upper left corner of the viewport relative to the back buffer
		uint32 mTopLeftX;

		/// the y-coordinate of the upper left corner of the viewport relative to the back buffer
		uint32 mTopLeftY;

		/// number of pixels along the x-axis
		uint32 mWidth;
	};
}

#endif // _VIEWPORT_DESCRIPTION_H_

