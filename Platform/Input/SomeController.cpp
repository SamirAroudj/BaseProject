/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#include "Platform/FailureHandling/InputException.h"
#include "Platform/Input/SomeController.h"
#include "Platform/Window.h"

using namespace FailureHandling;
using namespace Input;
using namespace Platform;

#ifdef _WINDOWS
SomeController::SomeController(LPDIRECTINPUTDEVICE8 gameController) :
		mDevice(gameController), mZAxisSupport(false)
{
	mStickSupport[0] = mStickSupport[1] = mStickSupport[2] = false;
	mConnected = true;

	GameControllerDescription description;
	gameController->EnumObjects(onFoundDevice, &description, DIDFT_PSHBUTTON | DIDFT_ABSAXIS | DIDFT_POV);	// find available buttons buttons and axes

	setSticksAndZAxis(description);

	mDevice->SetDataFormat(&c_dfDIJoystick);																// set data format and cooperative level, get access
	// must be exclusive for force feedback	
	mDevice->SetCooperativeLevel(Window::getSingleton().getWindowHandle(),
										DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	mDevice->Acquire();
}

BOOL CALLBACK SomeController::onFoundDevice(LPCDIDEVICEOBJECTINSTANCE deviceObject,
											LPVOID controllerDescription)
{
	GameControllerDescription *description = (GameControllerDescription *) controllerDescription;

	if (deviceObject->dwType & DIDFT_PSHBUTTON)														// get button data
	{
		int32 objectInstance = DIDFT_GETINSTANCE(deviceObject->dwType);
		if (description->mNumOfButtons < objectInstance + 1)
			description->mNumOfButtons = objectInstance + 1;
	}
	else if (deviceObject->dwType & DIDFT_ABSAXIS)													// get axis data
	{
		int32 index = -1;
		if (GUID_XAxis == deviceObject->guidType)
			index = 0;
		else if (GUID_YAxis == deviceObject->guidType)
			index = 1;
		else if (GUID_ZAxis == deviceObject->guidType)
			index = 2;
		else if (GUID_RxAxis == deviceObject->guidType)
			index = 3;
		else if (GUID_RyAxis == deviceObject->guidType)
			index = 4;
		else
			return DIENUM_CONTINUE;	// not of interest

		description->mSupportedAxes[index] = true;
		description->mAxisIDs[index] = deviceObject->dwType;
	}
	else if (deviceObject->dwType & DIDFT_POV)
	{
		description->mSupportedAxes[5] = true;
		description->mSupportedAxes[6] = true;
	}

	return DIENUM_CONTINUE;
}

void SomeController::setSticksAndZAxis(GameControllerDescription &description)
{
	AxisDescription properties[CONTROLLER_RANGED_AXES];
	queryAxisDescriptions(description, properties);
																											// stick and z-axis support
	if (description.mSupportedAxes[0] && description.mSupportedAxes[1] &&										// x- and y-axis -> stick 0?
	   properties[0] == properties[1])
	{
		mProperties[0] = properties[1];
		mStickSupport[0] = true;
	}

	mZAxisSupport = description.mSupportedAxes[2];																// z-axis?
	mProperties[1] = properties[2];

	if (description.mSupportedAxes[3] && description.mSupportedAxes[4] &&										// rx- and ry-axis -> stick 1?
	   properties[3] == properties[4])	
	{
		mProperties[2] = properties[3];
		mStickSupport[1] = true;
	}

	if (description.mSupportedAxes[5] && description.mSupportedAxes[6])											// POVX- and POVY-axis - stick 2?
		mStickSupport[2] = true;	
}
			
void SomeController::queryAxisDescriptions(GameControllerDescription &controllerDescription, AxisDescription *properties)
{
	DIPROPDWORD deadzoneProperty;																			// get dead zone, min and max range of each ranged axis																	
	deadzoneProperty.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	deadzoneProperty.diph.dwHow			= DIPH_BYID;
	deadzoneProperty.diph.dwSize		= sizeof(DIPROPDWORD);

	DIPROPRANGE rangeProperty;
	rangeProperty.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	rangeProperty.diph.dwHow		= DIPH_BYID;
	rangeProperty.diph.dwSize		= sizeof(DIPROPRANGE);

	for (uint32 i = 0; i < CONTROLLER_RANGED_AXES; ++i)														// properties for x, y, z, rx, ry
	{
		if (!controllerDescription.mSupportedAxes[i])
			continue;
		
		deadzoneProperty.diph.dwObj = controllerDescription.mAxisIDs[i];
		HRESULT result = mDevice->GetProperty(DIPROP_DEADZONE, &deadzoneProperty.diph);								// query dead zone
		if (DI_OK != result)
		{
			controllerDescription.mSupportedAxes[i] = false;
			continue;
		}
		
		rangeProperty.diph.dwObj = controllerDescription.mAxisIDs[i];		
		result = mDevice->GetProperty(DIPROP_RANGE, &rangeProperty.diph);											// query min and max range
		if (DI_OK != result)
		{
			controllerDescription.mSupportedAxes[i] = false;
			continue;
		}
		
		Real deadZone = (Real) deadzoneProperty.dwData / (Real) CONTROLLER_DEAD_ZONE_MAXIMUM;					// dead zone on a percentage basis (0.0f - 1.0f)
		if (deadZone < CONTROLLER_DEAD_ZONE_MINIMUM)
			deadZone = CONTROLLER_DEAD_ZONE_MINIMUM;

		properties[i].mMaxMagnitude = (rangeProperty.lMax - rangeProperty.lMin) / 2;								// compute values for normalization
		properties[i].mCenter = rangeProperty.lMin + properties[i].mMaxMagnitude;
		properties[i].mDeadZone = static_cast<int32>(deadZone * properties[i].mMaxMagnitude);
		properties[i].mInvertedRangeFactor = 1.0f / (properties[i].mMaxMagnitude - properties[i].mDeadZone);
	}	
}
#endif // _WINDOWS

SomeController::~SomeController()
{
	#ifdef _WINDOWS
		mDevice->Unacquire();
		mDevice->Release();
	#endif // _WINDOWS
}

uint32 SomeController::getNumberOfSupportedSticks() const
{
	uint32 numOfSupportedSticks = 0;
	for (uint32 i = 0; i < NUM_OF_STICKS; ++i)
		if (mStickSupport[i])
			++numOfSupportedSticks;

	return numOfSupportedSticks;
}

#ifdef _WINDOWS
	void SomeController::getProductTypeGUID(GUID &guid) const
	{
		DIDEVICEINSTANCE deviceInfo;
		deviceInfo.dwSize = sizeof(DIDEVICEINSTANCE);

		mDevice->GetDeviceInfo(&deviceInfo);
		memcpy(&guid, &deviceInfo.guidProduct, sizeof(GUID));
	}
#endif // _WINDOWS

#ifdef _WINDOWS
	void SomeController::reacquire()
	{
		reset();
		mDevice->Acquire();
	}
#endif // _WINDOWS
	
bool SomeController::update()
{
	#ifdef _WINDOWS
		bool connectionStateChanged = false;
		DIJOYSTATE  state;																		// get new game controller state
		memset(&state, 0, sizeof(DIJOYSTATE));

		mDevice->Poll();																					// get state and update connection state change
		HRESULT result = mDevice->GetDeviceState(sizeof(DIJOYSTATE), &state);
		if (S_OK != (result))
		{
			connectionStateChanged = mConnected;
			mConnected = false;
			return connectionStateChanged;
		}

		if (!mConnected)
		{
			mConnected = true;
			connectionStateChanged = true;
		}


		mPreviousButtonStates = mButtonStates;																// update buttons
		mButtonStates = 0;
		for (uint32 i = 0; i < CONTROLLER_BUTTON_COUNT; ++i)
			if (state.rgbButtons[i])
				mButtonStates |= (0x1 << i);

		updateAxes(state);																					// update axes	
		adaptStickYAxes();

		return connectionStateChanged;
	#endif // _WINDOWS
}
	
#ifdef _WINDOWS	
	void SomeController::updateAxes(DIJOYSTATE &state)
	{				
		if (mStickSupport[0])																		// normalize values for left stick
		{
			int32 x = state.lX - mProperties[0].mCenter;
			int32 y = state.lY - mProperties[0].mCenter;
			computeNormalizedStickPosition(x, y, mProperties[0].mDeadZone,
				mProperties[0].mMaxMagnitude, mProperties[0].mInvertedRangeFactor,
				mSticks[0]);
		}
		if (mZAxisSupport)																			// normalize value for z-axis
		{
			int32 value = state.lZ - mProperties[1].mCenter;
			if (abs(value) < mProperties[1].mDeadZone)
			{
				mZAxis = 0.0f;
			}
			else
			{
				value = value + (value > 0 ? -mProperties[1].mDeadZone : +mProperties[1].mDeadZone);
				mZAxis = value * mProperties[1].mInvertedRangeFactor;

				if (mZAxis > 1.0f)
					mZAxis = 1.0f;
				else if (mZAxis < -1.0f)
					mZAxis = -1.0f;
			}
		}
		if (mStickSupport[1])																		// normalize values for right stick
		{
			int32 x = state.lRx - mProperties[2].mCenter;
			int32 y = state.lRy - mProperties[2].mCenter;
			computeNormalizedStickPosition(x, y, mProperties[2].mDeadZone,
				mProperties[2].mMaxMagnitude, mProperties[2].mInvertedRangeFactor,
				mSticks[1]);
		}
		if (mStickSupport[2])
			updatePOVStick(state.rgdwPOV[0]);	
	}

	void SomeController::updatePOVStick(int32 pov)
	{																					// update AXIS_POVX and AXIS_POVY
		if (-1 == pov || pov == 0xFFFF)
		{
			mSticks[2].x = mSticks[2].y = 0.0f; 
		}
		else
		{
			Real angle = pov * (1.0f / (360 * 100.0f)) * Math::TWO_PI;
			mSticks[2].x = sinr(angle);
			mSticks[2].y = cosr(angle);
		}
	}
#endif // _WINDOWS
