/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef _WINDOWS
#include <Windows.h>

#elif _LINUX
    #include<X11/X.h>
    #include<X11/Xlib.h>
    #include<GL/gl.h>
    #include<GL/glx.h>
    #include "Platform/Input/KeyboardMapping.h"

#endif // _WINDOWS

#include <cassert>
#include <string>
#include "Platform/DataTypes.h"
#include "Platform/IWindowObserver.h"
#include "Platform/MagicConstants.h"
#include "Patterns/Subject.h"
#include "Patterns/Singleton.h"
#include "Utilities/Size2.h"

/// Provides platform dependent functionality.
/** This name space contains platform dependent functionality such as window creation, file access and timer functionality.*/
namespace Platform
{
	/// This class represents the main window of the application.
	/** This class is an implementation for creation and display of the main window of the application.
		The window is a popup-window without caption and border.
	*/
	class Window : public Patterns::Singleton<Window>, public Patterns::Subject<IWindowObserver>
	{
	public:
		/** The constructor creates and displays a window of the specified solution and without border etc.
		@param applicationHandle (Windows OS only) A windows handle to identify the application. This handle should be retrieved from WinMain.
		@param title This is the name and title of the window. (You won't see a title in a caption since there is no caption.)
		@param fullscreen The whole display will be used if full screen is true. If it is false a window with the solution width x height is created.
		@param size This is the desired size (= resolution = width and height) of the window and render area in pixel.
		@param depthBitsPerPixel This number specifies how many bits per pixel are used to store the depth of a pixel.
		@param stencilBitsPerPixel This number specifies how many bits per pixel are used to store pixel stencil information. */
		Window
		(
			#ifdef _WINDOWS
				HINSTANCE applicationHandle,
			#endif // _WINDOWS
			const std::string &title, bool fullscreen = false,
			const Utilities::ImgSize &size = Utilities::ImgSize(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT),
			uint16 depthBitsPerPixel = WINDOW_DEFAULT_DEPTH_BITS_PER_PIXEL, uint16 stencilBitsPerPixel = WINDOW_DEFAULT_STENCIL_BITS_PER_PIXEL
		);

		/** Frees requested resources. */
		virtual ~Window();

		/** Gets the aspect ratio (width / height) of the window.
		@return The aspect ratio (width / height) of the window. */
		inline Real getAspectRatio() const;

		/** Returns the number of bits per pixel storing the depth (z-value) of a pixel.
		@return Returns the number of bits per pixel which store the pixel depth. */
		inline uint16 getDepthBitsPerPixel() const;

		#ifdef _WINDOWS
			/** Queries the maximum window / screen size / resolution.
			@return Returns the size / resolution of the screen in pixels. */
			inline const Utilities::ImgSize &getScreenSize() const;
		#endif // _WIDNOWS

		/** Returns the window size / dimensions / resolution in pixels..
		@return The returned size describes how many pixels are along a window pixel row [0] and a window pixel column[1]. */
		inline const Utilities::ImgSize &getSize() const;

		/** Returns the number of bits per pixel storing stencil data.
		@return Returns the number of bits per pixel which store its stencil information. */
		inline uint16 getStencilBitsPerPixel() const;

		/** Queries if this object is active. This is true when the window is focused and in the foreground and it is false
			when another window was activated or if the user switched to the Desktop.
		@return Returns true when the window is active / has the user focus. */
		inline bool isActive() const;

		/** Returns weather the program runs in fullscreen mode.
		@return Function returns true if the whole screen is occupied or false if just a part of the screen is used. */
		inline bool isFullScreen() const;

		/** This function is used to trigger the display of the icon which represents the cursor.
		@param show If show is true the cursor is shown otherwise it is hidden.*/
		//void showCursor(bool show);

		#ifdef _WINDOWS
			/** Gets the application handle.
			@return The application handle is the identifier of this application which was obtained from the WINAPI WinMain function. */
			inline HINSTANCE getApplicationHandle() const;

			/** Returns a Windows window handle to identify the created window.
			@return This is a Windows window handle which identifies the main window. */
			inline HWND getWindowHandle() const;

		#elif _LINUX
			/** Returns a Linux window handle to identify the created window.
			@return This is a Linux window handle which identifies the main window. */
			inline ::Window &getWindowHandle();

			/** Returns a pointer to the Linux representation of the default display device.
			@return Returns a pointer to the Linux representation of the default display device. */
			inline Display *getDisplay();

			/** Reads and processes a single event if there is at least one event in the queue of the window.
			@return Returns true if there are more events queued. */
			virtual bool processQueuedEvent();

			/** Changes the current visual which requires window recreation.
			@param visualInfo Describes the used buffer swap chain and contains properties like depth bits per pixel, color depth etc.
				See xlib documentation of XVisualInfo for more details.*/
			void setVisual(XVisualInfo &visualInfo);
		#endif	// _WINDOWS

	private:
		/** Copy constructor is forbidden.
		@param copy Copy constructor is forbidden.*/
		inline Window(const Window &copy);

		/** Assignment operator is forbidden.
		@param rhs Operator is forbidden.
		@return Operator is forbidden*/
		inline Window &operator =(const Window &rhs);

		#ifdef _WINDOWS
			/** This is the application's window procedure which is used by windows in order to send this app messages.
				It delegates its messages to the window procedure.
			@param windowHandle This handle identifies the window the message is sent to
			@param messageIdentifier This value contains the message type.
			@param wParam This is additional information which depends on the message type.
			@param lParam This is some more additional information which depends on the message type.
			@return Result of the message processing which depends on the message. I know that doesn't help very much, but that's what MSDN says ;). */
			static LRESULT CALLBACK applicationProcedure(HWND windowHandle, uint32 messageIdentifier, WPARAM wParam, LPARAM lParam);

			/** This function actually creates the main window.
			@param applicationHandle A windows handle to identify the application. This handle should be retrieved from WinMain.
			@return If the creation works fine the function will return true otherwise false is returned if the window cannot be created.*/
			bool createWindow(HINSTANCE applicationHandle);
		#endif // _WINDOWS

		#ifdef _LINUX
			/** Creates and shows a window.
			@param visualInfo Defines what visual to use for the window. Determines bit depth etc. See xlib documentation.
			@return If the creation works fine the function will return true otherwise false is returned if the window cannot be created. */
			bool createWindow(XVisualInfo &visualInfo);

			/** Translates an xlib KeySym key value to a Key value of this project which is platform independent.
			@param keySym Set this to an xlib value which can be mapped to a Key value.
			@return The Key value which corresponds to keySym is returned. If the mapping is not possible then KEY_FAIL is returned.*/
            static Input::Key keySymToKey(KeySym keySym);
		#endif // _LINUX

		/** Call this if windows focus has recently changed.
		@param active Set this to true if the window has become active recently. (Gets input focus etc.)
			Set active to false if the window has become inactive recently. (Lost input focus etc.) */
		void onActivityChange(bool active);

		#ifdef _WINDOWS
			/** Registers a description of the window that will be created.
			@param applicationHandle A windows handle to identify the application. This handle should be retrieved from WinMain. */
			void registerWindowType(HINSTANCE applicationHandle) const;


			/** This is the window procedure call which is used by this window in order to process windows messages.
			@param windowHandle This handle identifies the window the message is sent to.
			@param messageIdentifier This value contains the message type.
			@param wParam This is additional information which depends on the message type.
			@param lParam This is some more additional information which depends on the message type.
			@return Result of the message processing which depends on the message. I know that doesn't help very much, but that's what MSDN says ;). */
			LRESULT CALLBACK windowProcedure(HWND windowHandle, uint32 messageIdentifier, WPARAM wParam, LPARAM lParam);
		#endif // _WINDOWS

	private:
			static const char *CLASS_NAME; /// Defines window class name for Windows.

	private:
		#ifdef _WINDOWS
			HINSTANCE mApplicationHandle;	/// This is a handle to identify the application.
			HWND mWindowHandle;				/// This is a windows handle to identify the window which is created by the constructor.
			Utilities::ImgSize mScreenSize;	/// screen / maximum size in pixel
		#endif // _WINDOWS

		#ifdef _LINUX
			::Window mWindowHandle;	/// This is the Linux representation of the main window of the application.
			Display *mDisplay;		/// This refers to the display which manages possibly multiple screens.
		#endif // _LINUX

		std::string mTitle;				/// Contains the window title.
		Utilities::ImgSize mSize;		/// window resolution in pixel counts = (width in pixels, height in pixels)
		uint16 mDepthBitsPerPixel;		/// number of bits per pixel which store depth info
		uint16 mStencilBitsPerPixel;	/// number of bits per pixel which store stencil info
		bool mActive;					/// True if the window is active. For example, activation of another window deactivates this window.
		bool mFullScreen;				/// is true if the application runs in fullscreen mode and is not windowed
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	inline Window::Window(const Window &copy)
	{
		assert(false);
	}

	inline Window &Window::operator =(const Window &rhs)
	{
		assert(false);
		return *this;
	}

	inline Real Window::getAspectRatio() const
	{
		return static_cast<Real>(mSize[0]) / static_cast<Real>(mSize[1]);
	}

	inline uint16 Window::getDepthBitsPerPixel() const
	{
		return mDepthBitsPerPixel;
	}

	inline const Utilities::ImgSize &Window::getSize() const
	{
		return mSize;
	}

	inline uint16 Window::getStencilBitsPerPixel() const
	{
		return mStencilBitsPerPixel;
	}

	inline bool Window::isActive() const
	{
		return mActive;
	}

	inline bool Window::isFullScreen() const
	{
		return mFullScreen;
	}

	#ifdef _WINDOWS
		inline const Utilities::ImgSize &Window::getScreenSize() const
		{
			return mScreenSize;
		}

		inline HINSTANCE Window::getApplicationHandle() const
		{
			return mApplicationHandle;
		}

		inline HWND Window::getWindowHandle() const
		{
			return mWindowHandle;
		}

	#elif _LINUX
			inline ::Window &Window::getWindowHandle()
			{
				return mWindowHandle;
			}

			inline Display *Window::getDisplay()
			{
				return mDisplay;
			}

	#endif // _WIDNOWS
}

#endif // _WINDOW_H_
