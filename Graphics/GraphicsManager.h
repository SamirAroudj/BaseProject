/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _GRAPHICS_MANAGER_H_
#define _GRAPHICS_MANAGER_H_

#include <cassert>
#ifdef _WINDOWS
#include <Windows.h>
#elif _LINUX
#include <GL/glx.h>
#endif // _WINDOWS
#include <GL/gl.h>

#include "Graphics/Color.h"
#include "Graphics/MagicConstants.h"
#include "Graphics/RenderGroup.h"
#include "Patterns/Singleton.h"
#include "Platform/DataTypes.h"

namespace Graphics
{
	/// Manager and Singleton object which is the basis for graphics
	/** Must be created to initialize the graphics system and to do further things with respect to graphics. */
	class GraphicsManager : public Patterns::Singleton<GraphicsManager>
	{
	public:
		/** Creates the the one and only GraphicsManager object and initializes the graphics system.
		@param colorResolution The color resolution used by the application can be 16bit or 32bit per pixel.
		@param clearColor The color buffer is set to clearColor at the beginning of each frame rendering. */
		GraphicsManager(uint32 colorResolution, const Color &clearColor = BACK_BUFFER_CLEAR_COLOR);

		/** Destroy the one and only manager object and shut this graphics system down. */
		virtual ~GraphicsManager();

		/** Clears back, depth and stencil buffers and resets model view transformations.*/
		void clearBackAndDepthStencilBuffer();

		/** Request the game to create some RenderGroup objects until there is a particular number of RenderGroup objects.
		@param numberInTheEnd This is the number of groups that will at least exist after this call. 
							  Must be greater or equal to the current number of render groups.
							  That means, that the number of render groups that are actually created is:
							  max(numberInTheEnd - oldNumber, 0) */
		void createRenderGroups(uint32 numberInTheEnd);

		/** Turns off blending. */
		void disableBlending() const;

		/** Turns on alpha blending. (alpha blending source factor = SRC_ALPHA, destination factor = ONE_MINUS_SRC_ALPHA) */
		void enableAlphaBlending() const;
		
		/** Query the number of existing render groups.
		@return The number of existing render groups is returned. */
		size_t getNumberOfRenderGroups() const { return mRenderGroups.size(); }

		/** Access RenderGroup objects which are rendered one after the other.
		@param number This is the index / number of the RenderGroup. The lower the index the later the group is rendered.
					  Number must be smaller than or equal to the number of currently exisiting RenderGroup objects.
					  If it is smaller than the number of group objects then the RenderGroup object with this index is returned.
					  If number is equal to the number of existing group objects then a new RenderGroup instance with this number is created.
		@return The returned RenderGroup object has the index number. */
		RenderGroup &getRenderGroup(size_t number);

		/** Swaps the front and back buffer to show the content of the back buffer and enable rendering to the back buffer. */
		void presentBackBuffer();

		/** Draws the members of each RenderGroup object. The first RenderGroup to be rendered has the highest index and
		the RenderGroup instance with index 0 is the last RenderGroup to be rendered. */
		void renderRenderGroups();

		/** Defines with which color the back buffer is filled before rendering everything else.
		@param Set this to the clear / background color with which the back buffer is filled before rendering.*/
		void setClearColor(const Graphics::Color &color);

	private:
		/** Copy constructor is forbidden. */
		GraphicsManager(const GraphicsManager &copy) : mClearColor(0, 0, 0, 0) { assert(false); }

		/** Assign operator is forbidden. */
		GraphicsManager &operator =(const GraphicsManager &rhs) { assert(false); return *this; }

		#ifdef _WINDOWS
			/** Sets width and height of the screen display as well as its color resolution.
			@param colorResolution Display's color resolution is set to this value. */
			void changeDisplaySettings(uint32 colorResolution);

			/** Finds a pixel format which is used by the device context.
			Pixel format specifies several values such as depth or stencil bits per pixel, color resolution, double buffering etc.
			@param colorResolution The function tries to choose a format which supports the entered color resolution. */
			void choosePixelFormat(uint32 colorResolution);
		#endif // _WINDOWS

	private:
		#ifdef _WINDOWS
			HDC mDeviceContext;						/// This device context from the OS is used for rendering.
			HGLRC mRenderingContext;				/// This is the OpenGL rendering context. (Windows)
		#elif _LINUX
			GLXContext	mRenderingContext;			/// This is the OpenGL rendering context. (LINUX)
		#endif // _WINDOWS

		std::vector<RenderGroup *> mRenderGroups;	/// Contains all render groups which are used for the purpose of render order definition.
		Color mClearColor;							/// The color buffer is set to mClearColor at the beginning of each frame rendering.
	};
}

#endif // _GRAPHICS_MANAGER_H_
