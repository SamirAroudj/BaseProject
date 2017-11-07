/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Platform/FailureHandling/InputException.h"
#include "Platform/Input/MagicConstants.h"
#include "Math/MathHelper.h"
#include "Platform/Input/Mouse.h"
#include "Platform/Window.h"

using namespace FailureHandling;
using namespace Input;
using namespace Platform;

Mouse::Mouse() :
	#ifdef _WINDOWS
		mRelativeToAbsoluteScaleFactorX(0),
		mRelativeToAbsoluteScaleFactorY(0),
	#endif // _WINDOWS
	mInUse(false)
{
	#ifdef _WINDOWS		
		mMouseDevice = NULL;
		mRelativeToAbsoluteScaleFactorY  = MOUSE_REALTIVE_TO_ABSOLUTE_SCALE;
		mRelativeToAbsoluteScaleFactorX  = MOUSE_REALTIVE_TO_ABSOLUTE_SCALE;
		if (Platform::Window::exists())
			mRelativeToAbsoluteScaleFactorX *= (float) Platform::Window::getSingleton().getAspectRatio();
	#endif // _WINDOWS

// todo implement flexible mapping some day?
//	setButtonMapping();

	reset();
}

//void Mouse::setButtonMapping()
//{
//	uint32	size = 32;
//	uint8	mapping[32];

//	#ifdef _LINUX
//		Display &display = Platform::Window::getSingleton().getDisplay();
//		uint32 buttonCount = XGetPointerMapping(display, mapping, size);

//		if (MappingSuccess != XSetPointerMapping(display, mapping, count))
//			throw InputException("Could not set mapping from physical to logical mouse buttons by xlib.");

//		// mapping is based on test runs with ubuntu Linux system with kde desktop manager
//		mapping[0] = 1; // primary button (usually left mouse button)
//		mapping[1] = 3; // secondary button (usually right mouse button)
//		mapping[2] = 6; // middle mouse button
//		mapping[3] = 4; // shitty - used for mouse wheel input
//		mapping[4] = 5; // shitty - used for mouse wheel input

//		uint32 buttonCount = XGetPointerMapping(display, mapping, size);
//		#include <iostream>
//		for (uint32 i = 0; i < buttonCount; ++i)
//			std::cout << (uint32) mapping[i] << std::endl;

//	#elif _WINDOWS
//		assert(false);

//	#endif // _LINUX
//}

Mouse::~Mouse()
{
	#ifdef _WINDOWS
		// release mouse
		if (mMouseDevice)																
		{
			mMouseDevice->Unacquire();
			mMouseDevice->Release();
			mMouseDevice = NULL;
		}
	#endif // _WINDOWS
}

#ifdef _WINDOWS	
	void Mouse::init(IDirectInput8 *directInput)
	{	
		// create mouse
		int32 result = directInput->CreateDevice(GUID_SysMouse, &mMouseDevice, NULL);					
		if (S_OK != result)
		{
			mMouseDevice = NULL;
			throw InputException("Could not create a representation of the system mouse.", result);
		}

		//  set cooperative level and data format, get access
		DWORD flags = DISCL_FOREGROUND;
		if (Window::exists() && Window::getSingleton().isFullScreen())
			flags |= DISCL_EXCLUSIVE;
		mMouseDevice->SetCooperativeLevel(Window::getSingleton().getWindowHandle(), flags);

		mMouseDevice->SetDataFormat(&c_dfDIMouse2);
		mMouseDevice->Acquire();
	}
#endif // _WINDOWS

bool Mouse::isButtonDown(Button button) const
{
	return 0 != (mButtonStates & (0x1 << button));
}

bool Mouse::isButtonPressed(Button button) const
{
	unsigned char buttonID = (0x1 << button);
	return (mButtonStates & buttonID) && !(mPreviousButtonStates & buttonID);
}

bool Mouse::isButtonReleased(Button button) const
{
	unsigned char buttonID = (0x1 << button);
	return !(mButtonStates & buttonID) && (mPreviousButtonStates & buttonID);
}

bool Mouse::isButtonUp(Button button) const
{
	return 0 == (mButtonStates & (0x1 << button));
}

#ifdef _LINUX
	void Mouse::onEndFrame()
	{
		mRelativeXMotion = 0.0f;
		mRelativeYMotion = 0.0f;
		mRelativeZMotion = 0.0f;

		// only copy states as mButtonStates are changed by events
		mPreviousButtonStates = mButtonStates;
	}

	void Mouse::processButtonEvent(Button button, bool pressed)
	{
		uint32 flag = 0x1 << button;
		if (pressed)
		{
			mButtonStates			|= flag;
			mPreviousButtonStates	&= ~flag;
		}
		else
		{
			mButtonStates			&= ~flag;
			mPreviousButtonStates	|= flag;
		}
	}

	void Mouse::processMotionEvent(Real absoluteX, Real absoluteY)
	{
		// check input values
		absoluteX = Math::clamp(absoluteX, 1.0f, -1.0f);
		absoluteY = Math::clamp(absoluteY, 1.0f, -1.0f);

		// update relative motion
		mRelativeXMotion = absoluteX - mAbsoluteX;
		mRelativeYMotion = absoluteY - mAbsoluteY;
		clampRelativeMotion();

		mAbsoluteX = absoluteX;
		mAbsoluteY = absoluteY;
	}

	void Mouse::processWheelMotionEvent(bool forward)
	{
		if (forward)
			mRelativeZMotion += MOUSE_RELATIVE_Z_MOTION_PER_EVENT;
		else
			mRelativeZMotion -= MOUSE_RELATIVE_Z_MOTION_PER_EVENT;
	}
#endif // _LINUX

#ifdef _WINDOWS
	void Mouse::reacquire()
	{
		reset();
		mMouseDevice->Acquire();
	}
#endif // _WINDOWS

void Mouse::reset()
{
	mRelativeXMotion = 0.0f;
	mRelativeYMotion = 0.0f;
	mRelativeZMotion = 0.0f;

	mAbsoluteX = 0.0f;
	mAbsoluteY = 0.0f;

	mButtonStates			= 0;
	mPreviousButtonStates	= 0;
}

void Mouse::update()
{
	#ifdef _WINDOWS
		// get new mouse state
		DIMOUSESTATE2 newMouseState;															
		memset(&newMouseState, 0, sizeof(DIMOUSESTATE2));
		int32 result = mMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &newMouseState);
		if (DI_OK != result)
			return;

		// get relative motion 
		mRelativeXMotion =  newMouseState.lX * ONE_MAX_RELATIVE_MOUSE_MOTION;						
		mRelativeYMotion = -newMouseState.lY * ONE_MAX_RELATIVE_MOUSE_MOTION;
		mRelativeZMotion =  newMouseState.lZ * ONE_MAX_RELATIVE_MOUSE_MOTION;

		clampRelativeMotion();

		// update the absolute x and y coordinates of the mouse and clamp them to -1.0f or 1.0f
		mAbsoluteX += mRelativeXMotion * mRelativeToAbsoluteScaleFactorX;		
		mAbsoluteY += mRelativeYMotion * mRelativeToAbsoluteScaleFactorY;
		mAbsoluteX = Math::clamp<float>(mAbsoluteX, 1.0f, -1.0f);
		mAbsoluteY = Math::clamp<float>(mAbsoluteY, 1.0f, -1.0f);

		// update buttons
		mPreviousButtonStates	= mButtonStates;
		mButtonStates			= 0;

		for (uint32 i = 0; i < BUTTON_FAIL; ++i)
			if (newMouseState.rgbButtons[i])
				mButtonStates |= 0x1 << i;
	#endif // _WINDOWS
}

void Mouse::clampRelativeMotion()
{
	// cap the values to -1.0f or 1.0f
	mRelativeXMotion = Math::clamp<float>(mRelativeXMotion, 1.0f, -1.0f);
	mRelativeYMotion = Math::clamp<float>(mRelativeYMotion, 1.0f, -1.0f);
	mRelativeZMotion = Math::clamp<float>(mRelativeZMotion, 1.0f, -1.0f);
}
