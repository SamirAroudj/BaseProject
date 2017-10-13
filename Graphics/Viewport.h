/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _VIEW_PORT_H_
#define _VIEW_PORT_H_

#include <cassert>
#include "Graphics/MagicConstants.h"
#include "Math/Vector2.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"
#include "Platform/DataTypes.h"
#include "Utilities/Size2.h"

namespace Graphics
{
	/// Represents a particular region of the back buffer stuff is rendered into.
	/** Viewport is defined relatively to the back buffer (in pixel). It can occupy just a part of the buffer or fully occupy it. */
	class Viewport
	{
	public:
		/** Computes and returns a matrix V which maps normalized device coordinates to pixel space coordinates
			z values are mapped from [-1, 1] (OpenGL convention) to [0, 1].
		@param viewportSize Defines the width and height of the viewport in pixels. (= image size)
		@param considerPixelCenterOffset If this is set to true then central pixel rays refer to integer pixel coordinates since an offset of half a pixel is subtracted from the pixel coordinates.
		@param principlePoint Defines which normalized coordinates (x, y) map to the viewport center. (Defines where the principle axis of the camera hits the camera plane.)
		@param sizeOfNDCCube Defines the size of the normalized device coordinates cube. Usually set to 2 (NDC coords within sizeOfNDCCube * [-0.5, 0.5] are mapped to [0, width] x [0, height] if pp = (0, 0).
		@return Computes and returns a matrix V which maps maps normalized device coordinates to pixel space cordinates. */
		static Math::Matrix4x4 computeMatrix(const Utilities::ImgSize &viewportSize, const bool considerPixelCenterOffset = true,
			const Math::Vector2 &principlePoint = Math::Vector2(0.5f, 0.5f), const Real sizeOfNDCCube = 2);
		
		/** Computes and returns a matrix V^-1 that maps normalized homogenous pixel space (z = 1) cordinates to normalized device coordinates.
		@param viewportSize Defines the width and height of the camera's viewport in pixels. (= image size)
		@param considerPixelCenterOffset If this is set to true then integer pixel coordinates refer to pixel centers since an offset of half a pixel is added to the pixel coordinates.
			If the parameter is set to false then integer coordinates refer to upper left pixel corners. 
		@param principlePoint Defines which normalized coordinates (x, y) map to the viewport center. (Defines where the principle axis of the camera hits the camera plane.)
		@param sizeOfNDCCube Defines the size of the normalized device coordinates cube. Usually set to 2 (-> inverse of (NDC coords within sizeOfNDCCube * [-0.5, 0.5] are mapped to [0, width] x [0, height] if pp = (0, 0))).
		@return Computes and returns a matrix V^-1 that maps normalized homogenous pixel space cordinates (z = 1) to normalized device coordinates. */
		static Math::Matrix3x3 computeInverseMatrix(const Utilities::ImgSize &viewportSize, const bool considerPixelCenterOffset = true,
			const Math::Vector2 &principlePoint = Math::Vector2(0.5f, 0.5f), const Real sizeOfNDCCube = 2);

		/** Provides access to the viewport that is currently the target of render calls that change the back / depth-stencil buffer.
		@return The viewport that is currently the target for render calls that affect the back / depth-stencil buffer is returned. */
		inline static const Viewport &getActiveViewport();

		/** Transforms a 2D position in normalized device coordinates (NDC) to a position in pixel coordinates (PC).
			Maps [-1, 1]^2 to [0, resolution[0]] x [0, resolution[1]]. Also works for points outside the normalized device coordinates cube [-1, 1]^2.
			Assumes a principle point of (0.5, 0.5).
		@param pNDC Set this to a 2D point with normalized device coordinates. (You get these coordinates after application of view & projection matrix & perspective division.)
		@param resolution Set this to the resolution of the window you want to get pixel coordinates for.
		@param addPixelCenterOffset (0.5, 0.5) is added at the end if this is set to true so that pixel centers are returned instead of corner coordinates.
		@return Returns the pixel coordinates that correspond to pNDC. (result = (pNDC * 0.5 + (0.5, 0.5) * resolution) */
		static Math::Vector2 transformNDCToPC(const Math::Vector2 &pNDC, const Utilities::ImgSize &resolution, const bool addPixelCenterOffset);

		/** Transforms a 2D position in normalized device coordinates (NDC) to a position in pixel coordinates (PC).
			Maps [-1, 1]^2 to [0, resolution[0]] x [0, resolution[1]]. Also works for points outside the normalized device coordinates cube [-1, 1]^2.
		@param pNDC Set this to a 2D point with normalized device coordinates. (You get these coordinates after application of view & projection matrix & perspective division.)
		@param resolution Set this to the resolution of the window you want to get pixel coordinates for.
		@param addPixelCenterOffset (0.5, 0.5) is added at the end if this is set to true so that pixel centers are returned instead of corner coordinates.
		@param principlePoint Set this to the principle point which defines the center of the image / where the ray x=0, y=0 is mapped to. Normally this is (0.5, 0.5).
		@param sizeOfNDCCube Defines how large the normalized device coordinates cube is.
			E.g., usually sizeOfNDCCube = 2 means, it ranges from -1 to 1 (-> if pp = (0, 0): xNDC = -1 -> xPS = 0, xNDC = 0 -> xPS = 0.5 width, xNDC = 1 -> xPS = width).
		@return Returns the pixel coordinates that correspond to pNDC. (result = (pNDC/sizeOfNDCCube + pp) * resolution)  */
		static Math::Vector2 transformNDCToPC(const Math::Vector2 &pNDC, const Utilities::ImgSize &resolution, const bool addPixelCenterOffset, const Math::Vector2 &principlePoint, const Real sizeOfNDCCube);

	public:		
		/** Creates a viewport that fills the whole window. */
		Viewport();

		/** Creates a viewport with specific position and size.
		@param position Contains the coordinates of the lower left corner of the viewport. (in pixel)
		@param size Contains width and height of the viewport. (in pixel) */
		Viewport(const uint32 position[2], const Utilities::ImgSize &size);

		/** Destroys the viewport and sets the active viewport to NULL if it is the active viewport. */
		~Viewport();

		/** There can only be one viewport that is active and responsible for the current rendering calls.
			Sets this viewport to be the active viewport. */
		void becomeActiveViewport() const;

		/** Computes the aspect ratio of the viewport.
		@return Returns the ratio of viewports width to viewports height: width / height.*/
		inline Real getAspectRatio() const;

		/** Queries the y-coordinate that corresponds to the bottom edge of the viewport when rendering GUI elements.
		@return The returned value is the y-coordinate of the bottom edge of the viewport in GUI space. */
		inline Real getGUISpaceBottom() const;

		/** Queries the x-coordinate that corresponds to the left edge of the viewport when rendering GUI elements.
		@return The returned value is the x-coordinate of the left edge of the viewport in GUI space. */
		inline Real getGUISpaceLeft() const;

		/** Queries the x-coordinate that corresponds to the right edge of the viewport when rendering GUI elements.
		@return The returned value is the x-coordinate of the right edge of the viewport in GUI space. */
		inline Real getGUISpaceRight() const;

		/** Queries the y-coordinate that corresponds to the top edge of the viewport when rendering GUI elements.
		@return The returned value is the y-coordinate of the top edge of the viewport in GUI space. */
		inline Real getGUISpaceTop() const;

		/** Changes the viewport by setting a new position and size.
		@param position Contains the coordinates of the lower left corner of the viewport. (in pixel)
		@param size Contains width and height of the viewport. (in pixel) */
		void set(const uint32 position[2], const Utilities::ImgSize &size);

	private:
		static const Viewport *sActiveViewport;	/// this is the viewport that is currently the target of rendering calls
		static const Real MAX_DEPTH;			/// Maximum z values within viewport coordinate systems. (z values \in [0, 1])

	private:
		uint32 mPosition[2];		/// coordinates of the lower left corner of the viewport (in pixel)
		Utilities::ImgSize mSize;	/// width and height of the viewport (in pixel)
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	inline const Viewport &Viewport::getActiveViewport()
	{
		assert(sActiveViewport);
		return *sActiveViewport;
	}

	inline Real Viewport::getAspectRatio() const
	{
		return static_cast<Real>(mSize[0]) / static_cast<Real>(mSize[1]);
	}

	inline Real Viewport::getGUISpaceBottom() const
	{
		return ORTHO_PROJECTION_BOTTOM;
	}

	inline Real Viewport::getGUISpaceLeft() const
	{
		return getAspectRatio() * ORTHO_PROJECTION_LEFT;
	}

	inline Real Viewport::getGUISpaceRight() const
	{
		return getAspectRatio() * ORTHO_PROJECTION_RIGHT;
	}

	inline Real Viewport::getGUISpaceTop() const
	{
		return ORTHO_PROJECTION_TOP;
	}
}

#endif // _VIEW_PORT_H_

