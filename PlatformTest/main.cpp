/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
#include <Windows.h>
#elif _LINUX
#include <cstdio>
#endif // _WINDOWS

#include <sstream>
#include "Platform/Application.h"
#include "Platform/Input/InputManager.h"
#include "Platform/Multithreading/Manager.h"
#include "Platform/ResourceManagement/MemoryManager.h"
#include "Platform/Timing/TimePeriod.h"

using namespace Input;
using namespace Platform;
using namespace std;
using namespace Timing;

#ifdef MEMORY_MANAGEMENT
	const uint32 ResourceManagement::DEFAULT_POOL_BUCKET_NUMBER = 5;
	const uint16 ResourceManagement::DEFAULT_POOL_BUCKET_CAPACITIES[DEFAULT_POOL_BUCKET_NUMBER] = { 1024, 1024, 1024, 1024, 1024 };
	const uint16 ResourceManagement::DEFAULT_POOL_BUCKET_GRANULARITIES[DEFAULT_POOL_BUCKET_NUMBER] = { 16, 32, 64, 128, 256 };
#endif // MEMORY_MANAGEMENT

std::mutex osMutex;

class OutputTask : public Multithreading::Task
{
public:
	OutputTask(wostringstream &os, char c, uint32 count) :
		Task(), mOS(os), mChar(c), mCount(count)
	{

	};

	virtual void function()
	{
		for (uint32 i = 0; i < mCount; ++i)
		{
			osMutex.lock();
			mOS << mChar;
			osMutex.unlock();
			//Sleep(10);
		}

		osMutex.lock();
		mOS << "\n";
		osMutex.unlock();
	}

private:
	wostringstream	&mOS;
	char			mChar;
	uint32			mCount;
};

class MyApp : public Application
{
public:
#ifdef _WINDOWS
	MyApp(HINSTANCE instanceHandle) : Application(instanceHandle), mPeriod(5.0f) { }
#elif _LINUX
    MyApp() : mPeriod(5.0f) {}
#endif // _WINDOWS
    
	virtual ~MyApp() { }

	virtual void onActivation() { Application::onActivation(); }
	virtual void onDeactivation() { Application::onDeactivation(); }

	void testMultithreading(wostringstream	&os)
	{
		os << "Test multithreading: \n";
		for (uint32 i = 0; i < 100; ++i)
		{
			OutputTask task1(os, 'a', 4);
			OutputTask task2(os, 'b', 10);
			OutputTask task3(os, 'c', 12);

			Multithreading::Manager &manager = Multithreading::Manager::getSingleton();
			manager.enqueue(&task1);
			manager.enqueue(&task2);
			manager.enqueue(&task3);

			task1.waitUntilFinished();
			osMutex.lock();
			os << "Task 1 has finished!\n";
			osMutex.unlock();

			task2.waitUntilFinished();
			osMutex.lock();
			os << "Task 2 has finished!\n";
			osMutex.unlock();

			task3.waitUntilFinished();
			osMutex.lock();
			os << "Task 3 has finished!\n";
			osMutex.unlock();
		}
	}

protected:
	virtual void postRender() { }
	virtual void render() { }

	virtual void update()
	{
		const Keyboard	&keyboard	= InputManager::getSingleton().getKeyboard();
		const Mouse		&mouse		= InputManager::getSingleton().getMouse();
		wostringstream	os(wostringstream::out);
		os.clear();

		if (keyboard.isKeyDown(Input::KEY_ESCAPE))
			mRunning = false;

		bool change = false;
		if (mInterpretingTextInput && keyboard.isKeyPressed(Input::KEY_RETURN))
		{
			wstring text = endTextInput();
			os << L"Finished text:\n";
			os << text << L"\n";
			change = true;
		}
		else
		{
			if (!mInterpretingTextInput && keyboard.isKeyPressed(Input::KEY_RETURN))
			{
				mLastTextLength = 0;
				os << L"Start text input.\n";
				startTextInput();
				change = true;
			}
		
			if (keyboard.isKeyDown(Input::KEY_M))
			{
				change = true;
				testMultithreading(os);
			}
		}

		if (isInterpretingTextInput())
		{
			if (keyboard.isKeyPressed(Input::KEY_LEFT))
			{
				change = true;
				mTextInput.moveCursorToTheLeft();
			}
			if (keyboard.isKeyPressed(Input::KEY_RIGHT))
			{
				change = true;
				mTextInput.moveCursorToTheRight();
			}
			if (keyboard.isKeyPressed(Input::KEY_DELETE))
			{
				change = true;
				mTextInput.removeCharacterAtCursorPosition();
			}
			if (keyboard.isKeyPressed(Input::KEY_BACKSPACE))
			{
				change = true;
				mTextInput.removeCharacterBeforeCursorPosition();
			}
		}

		if (mLastTextLength != mTextInput.getTextLength())
			change = true;
		if (change && mInterpretingTextInput)
		{
			wstring text = mTextInput.getText();
			text.insert(mTextInput.getCursorPosition(), L"|");
			os << text << L"\n";
		}

		Real mouseXMotion = mouse.getRelativeXMotion();
		Real mouseYMotion = mouse.getRelativeYMotion();
		if (mouseXMotion != 0.0f && mouseYMotion != 0.0f)
		{
			change = true;
			os << L"\nmouse x: " << mouseXMotion;
			os << L"\nmouse y: " << mouseYMotion;
		}
        
#ifdef _LINUX
            if (mPeriod.hasExpired())
            {
                printf("%f seconds have elapsed.\n", mPeriod.getElapsedTime());
                mPeriod.reset();
            }
#endif // _LINUX

		if (change)
        {
#ifdef _WINDOWS
			OutputDebugString(os.str().c_str());
#elif _LINUX
            wprintf(os.str().c_str());
#endif // _WINDOWS etc
        }
		mLastTextLength = mTextInput.getTextLength();
	}

    TimePeriod	mPeriod;
	uint32		mLastTextLength;
};

#ifdef _WINDOWS
	int32 WINAPI WinMain(HINSTANCE applicationHandle, HINSTANCE unused, LPSTR commandLineString, int32 windowShowState)
	{
#else
	int main(int argc, char *argv[])
	{
#endif // _WINDOWS

	{
		#ifdef _WINDOWS
			MyApp application(applicationHandle);
		#else
			MyApp application;
		#endif // _WINDOWS

		application.run();
	}
	
	#ifdef MEMORY_MANAGEMENT
		ResourceManagement::MemoryManager::shutDown();
	#endif // MEMORY_MANAGEMENT
	return 0;
}
