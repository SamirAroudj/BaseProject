/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "Patterns/Singleton.h"
#include "Platform/Input/TextInput.h"
#include "Platform/Profiling/FrameRateCalculator.h"
#include "Platform/Window.h"

namespace Platform
{
	/// This is the main object of the application which controls the main loop of the application
	class Application : public Patterns::Singleton<Application>, public IWindowObserver
	{
	public:
		/** Creates an application object (load config data, create a window, start the application timer etc.)
		@param applicationHandle The applicationHandle parameter should be set to the value of the HINSTANCE parameter from WinMain.
		@param configFile (optional) Contains information which are necessary to create the main window. See ApplicationConfigurationLoader.*/
		Application
		(
			#ifdef _WINDOWS
				HINSTANCE applicationHandle,
			#endif // _WINDOWS
			const std::string &configFile = DEFAULT_CONFIG_FILE
		);

		/** Destroy the window and release other requested resources. */
		virtual ~Application();

		/** End the interpretation of keystrokes as text.
		@return The returned wide character string is the final text the user entered.*/
		std::wstring endTextInput();

		/** Exit the main loop of the application. */
		inline void exit() { mRunning = false; }

		/** Returns how many frames per second were rendered last frame rate period or -1.0f if it isn't measured.
			The frame rate period length is defined in the application configuration file.
		@return Returns the number of frames rendered last frame rate period or -1.0f if it isn't measured. */
		inline Real getFrameRate() const { return (mFrameRateCalculator ? mFrameRateCalculator->getFPS() : -1.0f); }

		/** Obtain access to the text entered by the user.
		@return The returned text input is a representation of the text the user entered. */
		inline Input::TextInput &getTextInput() { return mTextInput; }

		/** Obtain wanted / desired frame time in seconds. (e.g. 1 / 60 Hz = 0.017666s)
		@return The desired time per frame in seconds is returned.*/
		inline Real getWantedFrameTime() const { return mWantedFrameTime; }

		/** Query if the application is currently interpreting key strokes as text input.
		@return Returns true if the application is currently interpreting key strokes as text input.
				That is, startTextInput() has been called recently and has not been finished by endTextInput().*/
		inline bool isInterpretingTextInput() const { return mInterpretingTextInput; }

		/** Query whether the wanted delta time in milliseconds was achieved or not.
		@return Returns true if the elapsed frame delta time was greater than the wanted frame delta time. */
		inline bool isRunningSlowly() const { return mRunningSlowly; }

		/** If the main window is set to the foreground then this function is called.
			Classes that overwrite this function must call this base function in the overwriting implementation. */
		virtual void onActivation();

		/** If the main window is set to the background then this function is called.
			Classes that overwrite this function must call this base function in the overwriting implementation. */
		virtual void onDeactivation();

		/** Start the main loop, process events, update the application, render some stuff, execute post render functionality. (in this order). */
		virtual int64 run();

		/** Start to process users key strokes as text. */
		void startTextInput();

	protected:
		/** Function which can be overridden to implement functionality which is executed after completion of the virtual void render() = 0; function. */
		virtual void postRender() = 0;

		/** This function can be overridden to implement own rendering functionality. */
		virtual void render() = 0;

		/** This function can be overridden to implement functionality which is executed every frame.*/
		virtual void update() = 0;

	private:
		/** Copy constructor is forbidden.
		@param rhs Don't call it, it fails.*/
		Application(const Application &rhs) : mFrameRateCalculator(NULL), mWantedFrameTime(-1.0f),
			mInterpretingTextInput(false), mRunning(false), mRunningSlowly(false)
		{
			assert(false);
		}

		/** Assignment operator is forbidden.
		@param rhs Don't call it, it fails.
		@return Don't call it, it fails. */
		Application &operator =(const Application &rhs)
		{
			assert(false);
			return *this;
		}

		/** Creates a window according to the application configuration file. */
		#ifdef _WINDOWS
			void createWindow(HINSTANCE applicationHandle);
		#else
			void createWindow();
		#endif // _WINDOWS

		/** Contains last executions that are done before waiting for the next frame to be started. */
		void endFrame();

		/** Does everything which needs to be done to update the complete application in a reasonable order.
		   For example, it includes updating the ApplicationTimer object, calling update, etc. */
		void updateCompletely();

		/** Application waits at the end of the current frame to restrict the frame rate according to user configuration.
		   See constructor and configurationFileName. */
		void wait();
		
	public:
		static const char *DEFAULT_CONFIG_FILE;					/// The default path and name of the configuration file to load and apply

	protected:
		Input::TextInput mTextInput;							/// This is a representation of what the text the user enters.
		Profiling::FrameRateCalculator *mFrameRateCalculator;	/// Is responsible for FPS calculation.
		Real mWantedFrameTime;									/// Defines how many seconds a frame should take at least. This is used to cap the frame rate.
																/// Main loop thread sleeps if it actually requires less time to update, render etc.
		bool mInterpretingTextInput;							/// This value is true if the application is interpreting key strokes by the user as text input.
																/// startTextInput() has been called but not endTextInput().
		bool mRunning;											/// The main loop is executed as long as this value is true.
		bool mRunningSlowly;									/// render() is skipped if this variable is true. This variable is set to true when
																/// the program runs too slow (delta time >  mWantedFrameTime).
	};
}

#endif // _APPLICATION_H_
