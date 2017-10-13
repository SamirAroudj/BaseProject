/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
	#include <Windows.h>
#endif // _WINDOWS

#include <chrono>
#include <thread>
#include "Platform/Application.h"
#include "Platform/ApplicationTimer.h"
#include "Platform/FrameRateCalculator.h"
#include "Platform/Input/InputManager.h"
#include "Platform/Multithreading/Manager.h"
#include "Platform/ParametersManager.h"
#include "Platform/ResourceManagement/MemoryManager.h"
#include "Platform/Storage/File.h"
#include "Platform/Window.h"
#include "Utilities/RandomManager.h"

using namespace Input;
using namespace Platform;
using namespace std;
using namespace Storage;
using namespace Utilities;

Application::Application
(
	#ifdef _WINDOWS
		HINSTANCE applicationHandle,
	#endif // _WINDOWS
	const string &configurationFileName
) :
	mFrameRateCalculator(NULL),
	mWantedFrameTime(1.0f / 60.0f),
	mInterpretingTextInput(false),
	mRunning(true),
	mRunningSlowly(true)
{
	// create managers
	ParametersManager *paramsManager = new ParametersManager(configurationFileName);
	Multithreading::Manager *workManager = new Multithreading::Manager();
	ApplicationTimer *applicationTimer	= new ApplicationTimer();
	RandomManager *randomManager = new RandomManager();

	// create pool of secondary threads
	uint32 secondaryThreadCount;
	if (!paramsManager->get(secondaryThreadCount, "Platform::Multithreading::secondaryThreadsCount"))
		secondaryThreadCount = 8;
	workManager->runWork(secondaryThreadCount);

	// create window
	createWindow
	(
		#ifdef _WINDOWS
			applicationHandle
		#endif // _WINDOWS
	);

	// input manager depends on window
	InputManager *inputManager = new InputManager();
}

void Application::createWindow
(
	#ifdef _WINDOWS
		HINSTANCE applicationHandle
	#endif // _WINDOWS
)
{
	const ParametersManager &manager = ParametersManager::getSingleton();
		
	// create window for rendering?
	bool createWindow;
	manager.get(createWindow, "Platform::createWindow");
	if (!createWindow)
		return;

	// window & very basic rendering parameters
	string title;
	ImgSize size;
	Real timePeriodPerFPSMeasurement;
	uint32 depthBitsPerPixel;
	uint32 stencilBitsPerPixel;
	uint32 maxFrameRate;
	bool fullscreen;
		
	// get parameters
	if (!manager.get(title, "Platform::windowTitle"))
		title = "Title";
	if (!manager.get(size[0], "Platform::width"))
		size[0] = 800;
	if (!manager.get(size[1], "Platform::height"))
		size[1] = 600;
	if (!manager.get(fullscreen, "Platform::fullscreen"))
		fullscreen = false;
	if (!manager.get(depthBitsPerPixel, "Platform::depthBitPerPixel"))
		depthBitsPerPixel = 24;
	if (!manager.get(stencilBitsPerPixel, "Platform::stencilBitsPerPixel"))
		stencilBitsPerPixel = 8;
	if (!manager.get(maxFrameRate, "Platform::maxFrameRateHz"))
		maxFrameRate = 60;
	if (!manager.get(timePeriodPerFPSMeasurement, "Platform::timePeriodPerFPSMeasurement"))
		timePeriodPerFPSMeasurement = 1.0f;

	// create window & register observer
	Window *window = 
		new Window
		(
			#ifdef _WINDOWS
				applicationHandle, 
			#endif // _WINDOWS
			title, fullscreen, size, depthBitsPerPixel, stencilBitsPerPixel
		);

	// desired maximum time per frame in seconds
	mWantedFrameTime = (Real) 1.0f / maxFrameRate;
	window->registerObserver(this);

	// over which time period do we measure / average FPS measurements
	if (timePeriodPerFPSMeasurement > 0.0f)
		mFrameRateCalculator = new FrameRateCalculator(timePeriodPerFPSMeasurement);
}

Application::~Application()
{
	delete mFrameRateCalculator;

	// release singletons
	if (Platform::Window::exists())
	{
		Window::getSingleton().deregisterObserver(this);
		delete Window::getSingletonPointer();
	}
	if (InputManager::exists())
		delete InputManager::getSingletonPointer();
	if (RandomManager::exists())
		delete Utilities::RandomManager::getSingletonPointer();
	if (ApplicationTimer::exists())
		delete ApplicationTimer::getSingletonPointer();
	if (Multithreading::Manager::exists())
		delete Multithreading::Manager::getSingletonPointer();
	if (ParametersManager::exists())
		delete ParametersManager::getSingletonPointer();

	#ifdef _DEBUG
		assert(0 == File::getOpenFilesCount());
	#endif // _DEBUG
}

wstring Application::endTextInput()
{
	mInterpretingTextInput = false;
	return mTextInput.getText();
}

void Application::onActivation()
{
	#ifdef _WINDOWS
		InputManager::getSingleton().reacquireDevices();
	#endif // _WINDOWS
}

void Application::onDeactivation()
{
	InputManager::getSingleton().resetDevices();
}

int64 Application::run()
{
	// no window? -> shortened main loop
	if (!Window::exists())
	{
		while(mRunning)
			updateCompletely();

		return 0;
	}

	while(mRunning)
	{ 
		// 00 process system messages
		#ifdef _WINDOWS 	
			MSG msg;		

			if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (WM_QUIT == msg.message)
					return msg.wParam;

				TranslateMessage(&msg);
				DispatchMessageA(&msg);
				continue;
			}
		#else
			if (Window::getSingleton().processQueuedEvent())
				continue;
		#endif // _WINDOWS

		// 01 input
		InputManager::getSingleton().update();	

		// 02 update
		updateCompletely();

		// 03 render
		render();
		if (mFrameRateCalculator)
			mFrameRateCalculator->onFrameRendered();

		// 04 post render
		postRender();

		// 05 last things to do before next frame, is necessary due to crappy xlib input management
		endFrame();

		// wait to restrict frame rate
		wait();
	}

	return 0;
}

void Application::updateCompletely()
{
	ApplicationTimer::getSingleton().update();
	if (mFrameRateCalculator)
		mFrameRateCalculator->update();
	update();
}

void Application::endFrame()
{
	#ifdef _LINUX
		InputManager::getSingleton().onEndFrame();
	#endif // _LINUX
}

void Application::wait()
{
	// cap frame rate or acitvate skipping of frames when program is running too slow
	Real	deltaTime		= ApplicationTimer::getSingleton().getDeltaTime();
			mRunningSlowly	= false;

	// sleep to cap frame rate?
	if (mWantedFrameTime > deltaTime)
	{
		// TODO rather wait for vsync / vblanc signal
		Real diff = mWantedFrameTime - deltaTime;

		#ifdef _WINDOWS // extra case as there is a bug in visual studio which prevents compiling the latter, easier version
			chrono::duration<Real, chrono::seconds::period> temp(diff);
			chrono::duration<uint64> sleepingTime = chrono::duration_cast<chrono::duration<uint64>>(temp);
		#else
			chrono::duration<Real, chrono::seconds::period> sleepingTime(diff);
		#endif // _WINDOWS

		this_thread::sleep_for(sleepingTime);
	}
	else
	{
		mRunningSlowly = true;
	}
}

void Application::startTextInput()
{
	mTextInput.clear();
	mInterpretingTextInput = true;
}
