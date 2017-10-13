/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _GRAPHICS_MANAGER_H_
#define _GRAPHICS_MANAGER_H_

#include <vector>
#include "D3D10Misc.h"
#include "Patterns/Singleton.h"
#include "VertexLayout.h"
#include "ViewportDescription.h"
#include "Platform/Window.h"

/// Contains classes necessary for rendering.  This namespace contains all basic classes which are needed to render some stuff on the screen.
namespace Graphics
{
	/// Initializes the basic graphics API which is used to leverage hardware acceleration.
	/** This class is used to set up the graphics device, the back buffer, the depth- / stencil buffer etc. */
	class GraphicsManager : public Patterns::Singleton<GraphicsManager>
	{
	public:
		/** Initializes the graphics device and create back and depth / stencil buffers.
		@param refreshRateHertz The refresh rate used in Hertz. Typical values are 50/s or 60/s.
		@param multisamplingCount The number of pixel samples used to determine the final colour of a pixel. (Used for stair step effect dilution.)
		@param multisamplingQuality The quality level used for multisampling to dilute stairstep effects. Limited by the hardware device and multisampling count.
		@param enableVSync Set this value to true to wait for vertical synchronisation of the screen to avoid tearing effects.*/
		GraphicsManager(uint32 refreshRateHertz, uint32 multisamplingCount, uint32 multisamplingQuality, bool enableVSync);

		/** Release requested resources. */
		virtual ~GraphicsManager();

		/** Clears the back and depth stencil buffer.
			The back buffer is filled with BACK_BUFFER_CLEAR_COLOR.
			The depth buffer is filled with  DEPTH_BUFFER_MAX_DEPTH and the 
			stencil buffer is filled with STENCIL_BUFFER_CLEAR_VALUE.*/
		void clearBackAndDepthStencilBuffer();

		/** Prepare the rendering of a frame by resetting depth stencil state and blend state. */
		void clearOutputMergerStates();

		/** Retrieves the chief DirectX 10 interface used to control the graphics device.
		@return The returned value points to the DirectX 10 interface which is intended for using functionality of the graphics device.*/
		ID3D10Device *getDevice() { return mDevice; }

		/** Returns the maximum quality level that can be used with this device and back buffer format.
		@param backBufferFormat The structure of the back buffer pixels the multisampling is applied to.
		@param multisamplingCount The number of samples which are used to compute the final color of a pixel.
		@return The returned value is the maximum quality level (inclusive) that can be used with the given combination.*/
		uint32 getMaxMultisamplingQualityLevel(DXGI_FORMAT backBufferFormat, uint32 multisamplingCount) const;

		/** Swaps the front and back buffer to show the content of the back buffer and enable rendering into the back buffer. */
		void presentBackBuffer();

		/** Specifies the structure of the input elements / vertices the graphics device is requested to render next.
		@param vertexLayout The vertex layout describes the structure of every input element to render. See VertexLayout. */
		void setVertexLayout(const VertexLayout *vertexLayout);

		/** Set the viewports that are used for rendering.
		@param viewportDescriptions An array of viewport descritptions that define to what parts of the back buffer the 3D scene is rendered.
		@param numOfDescriptions Should contain the array size of viewportDescriptions.*/
		void setViewports(const ViewportDescription *viewportDescriptions, uint32 numOfDescriptions);

	private:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a back and depth stencil buffers. </summary>
		///
		/// <remarks>	Samir, 20.11.2011. </remarks>
		///
		/// <param name="multisamplingCount">	[in] The number of pixel samples used to determine the final colour of a pixel. (Used for stair step effect dilution.) </param>
		/// <param name="multisamplingQuality">	[in] The quality level used for multisampling to dilute stairstep effects. Limited by the hardware device and multisampling count </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void createBackAndDepthStencilBuffers(uint32 multisamplingCount, uint32 multisamplingQuality);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a swap chain that provides functionality for frame buffer swapping. </summary>
		///
		/// <remarks>	Samir, 19.11.2011. </remarks>
		///
		/// <param name="GIFactory">			[in] A DirectX graphics interface factory which to create the swap chain.	</param>
		/// <param name="refreshRateHertz">		[in] The refresh rate used in Hertz. Typical values are 50 or 60. </param>
		/// <param name="multisamplingCount">	[in] The number of pixel samples used to determine the final colour of a pixel. (Used for stair step effect dilution.) </param>
		/// <param name="multisamplingQuality">	[in] The quality level used for multisampling to dilute stairstep effects. Limited by the hardware device and multisampling count </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void createSwapChain(IDXGIFactory *GIFactory, uint32 refreshRateHertz, uint32 multisamplingCount, uint32 multisamplingQuality);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Enumerate all available adapters. (e.g. video cards) </summary>
		///
		/// <remarks>	Samir, 19.11.2011. </remarks>
		///
		/// <param name="GIFactory">	[in] A DirectX graphics interface factory which is needed for enumeration. </param>
		/// <param name="directX10Capable"> [in] Set this value to true if you only want to get adapters which support DirectX10. </param>
		/// <param name="adapters"> 	[out] The vector contains all available adapters. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void enumerateAdapters(IDXGIFactory *GIFactory, bool directX10Capable, std::vector<IDXGIAdapter *> &adapters);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Obtain all outputs, such as monitors, that are connected to the specified adapter. </summary>
		///
		/// <remarks>	Samir, 19.11.2011. </remarks>
		///
		/// <param name="adapter">	[in] The outputs connected to the specified adapter are enumerated. </param>
		/// <param name="outputs">	[out] Contains all outputs, such as monitors, which are connected to the specified adapter. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void enumerateOutputs(IDXGIAdapter *adapter, std::vector<IDXGIOutput *> &outputs);

#ifdef _DEBUG
		/** Enumerates all DXGI_FORMAT_R8G8B8A8_UNORM supporting display modes of each output of each adapter and
			prints their dimensions and refresh rates to the visual studio ouput window.
		@param GIFactory The factory is needed for enumberation. */
		void outputDisplayModes(IDXGIFactory *GIFactory);
#endif // _DEBUG

		ID3D10Device			*mDevice;					/// chief DirectX 10 interface used to control the graphics device
		IDXGISwapChain			*mSwapChain;				/// the swap chain describes the back buffer method and double buffering is used
		ID3D10RenderTargetView	*mBackBufferView;			/// a view of the back buffer that is bound to the output merger stage of the DirectX pipeline 
		ID3D10Texture2D			*mDepthStencilBuffer;		/// a 2D texture storing the depth of the pixel from the back buffer and some additional stencil info
		ID3D10DepthStencilView	*mDepthStencilBufferView;	/// a view of the depth / stencil buffer which is bound to the output merger stage
		bool					mWaitForVSync;				/// If the value is true then the buffer swap chain is synchronized with vertical blank of the screen.
	};
}

#endif // _GRAPHICS_MANAGER_H_

