/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
#include <Windows.h>
#include <XInput.h>
#include "Platform/FailureHandling/InputException.h"
#include "Platform/Input/MagicConstants.h"
#include "Platform/Input/Xbox360Controller.h"

using namespace FailureHandling;
using namespace Input;

const int32 L2_FLAG = (0x1 << Xbox360Controller::L2);
const int32 R2_FLAG = (0x1 << Xbox360Controller::R2);

const int32 STICK_MAX_MAGNITUDE		= 32767;
const int32 TRIGGER_MAX_MAGNITUDE	= 255;

const Real LEFT_STICK_INVERTED_RANGE_FACTOR	= 1.0f / (STICK_MAX_MAGNITUDE   - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
const Real RIGHT_STICK_INVERTED_RANGE_FACTOR	= 1.0f / (STICK_MAX_MAGNITUDE   - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
const Real TRIGGER_INVERTED_RANGE_FACTOR		= 1.0f / (TRIGGER_MAX_MAGNITUDE - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

Xbox360Controller::Xbox360Controller(uint32 number) : 
	mNumber(number)
{
}


Xbox360Controller::~Xbox360Controller()
{
}

bool Xbox360Controller::update()
{
	bool connectionStateChanged = false;
	XINPUT_STATE state;		
	XINPUT_GAMEPAD &data = state.Gamepad;																					// get controller state
	int32 result = XInputGetState(mNumber, &state);

	if (ERROR_SUCCESS != result)																						// react to missing connection
	{
		if (ERROR_DEVICE_NOT_CONNECTED != result)
		{
			throw InputException("Error occurred while getting data from Xbox 360 controller.", result);
		}
		else if (mConnected)
		{
			reset();
			connectionStateChanged = true;
			mConnected = false;
		}

		return connectionStateChanged;
	}

	if (!mConnected)																									// react to existing connection
	{
		connectionStateChanged = true;
		mConnected = true;
	}


	mPreviousButtonStates = mButtonStates;																			// update buttons
	mButtonStates = 0;
	mButtonStates |= data.wButtons;

	if (XINPUT_GAMEPAD_TRIGGER_THRESHOLD < data.bLeftTrigger)
		mButtonStates |= L2_FLAG;
	if (XINPUT_GAMEPAD_TRIGGER_THRESHOLD < data.bRightTrigger)
		mButtonStates |= R2_FLAG;

	computeNormalizedStickPosition(data.sThumbLX, data.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,
								   STICK_MAX_MAGNITUDE, LEFT_STICK_INVERTED_RANGE_FACTOR, mSticks[0]);
	computeNormalizedStickPosition(data.sThumbRX, data.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE,
								   STICK_MAX_MAGNITUDE, RIGHT_STICK_INVERTED_RANGE_FACTOR, mSticks[1]);

	updatePOVAxes(data.wButtons);
	updateZAxis(data.bLeftTrigger, data.bRightTrigger);
	adaptStickYAxes();

	return connectionStateChanged;
}

void Xbox360Controller::updatePOVAxes(uint32 buttons)
{
	if (buttons & XINPUT_GAMEPAD_DPAD_LEFT)																				// go through possible cases
	{																													// and set axis values
		if (buttons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			mSticks[2].x = -Math::ONE_SQRT_TWO;
			mSticks[2].y = -Math::ONE_SQRT_TWO;
		}
		else if (buttons & XINPUT_GAMEPAD_DPAD_UP)
		{
			mSticks[2].x = -Math::ONE_SQRT_TWO;
			mSticks[2].y =  Math::ONE_SQRT_TWO;
		}
		else
		{
			mSticks[2].x = -1.0f;
			mSticks[2].y = 0.0f;
		}
	}
	else if (buttons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		if (buttons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			mSticks[2].x =  Math::ONE_SQRT_TWO;
			mSticks[2].y = -Math::ONE_SQRT_TWO;
		}
		else if (buttons & XINPUT_GAMEPAD_DPAD_UP)
		{
			mSticks[2].x = Math::ONE_SQRT_TWO;
			mSticks[2].y = Math::ONE_SQRT_TWO;
		}
		else
		{
			mSticks[2].x = 1.0f;
			mSticks[2].y = 0.0f;
		}
	}
	else
	{
		if (buttons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			mSticks[2].x =  0.0f;
			mSticks[2].y = -1.0f;
		}
		else if (buttons & XINPUT_GAMEPAD_DPAD_UP)
		{
			mSticks[2].x = 0.0f;
			mSticks[2].y = 1.0f;
		}
		else
		{
			mSticks[2].x = 0.0f;
			mSticks[2].y = 0.0f;
		}
	}
}
	
void Xbox360Controller::updateZAxis(unsigned char leftTrigger, unsigned char rightTrigger)
{
	mZAxis = 0.0f;
	if (mButtonStates & R2_FLAG && mButtonStates & L2_FLAG)																			// 0.0f if both are pressed
		return;

	if (mButtonStates & L2_FLAG)																											// left: -[1.0f, 0.0f)
		mZAxis = -(leftTrigger - XINPUT_GAMEPAD_TRIGGER_THRESHOLD) * TRIGGER_INVERTED_RANGE_FACTOR;
	else if (mButtonStates & R2_FLAG)																									// right: (0.0f, 1.0f]
		mZAxis = +(rightTrigger - XINPUT_GAMEPAD_TRIGGER_THRESHOLD) * TRIGGER_INVERTED_RANGE_FACTOR;
}

#endif // _WINDOWS
