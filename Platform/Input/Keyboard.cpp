/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _LINUX
#include <cstring>
#endif // _LINUX
#include <string>
#include "Platform/FailureHandling/InputException.h"
#include "Platform/Input/Keyboard.h"
#include "Platform/Window.h"

using namespace FailureHandling;
using namespace Input;
using namespace Platform;
using namespace std;

Keyboard::Keyboard()
{
	#ifdef _WINDOWS
		mKeyboardDevice = NULL;
	#endif // _WINDOWS

	reset();
}

Keyboard::~Keyboard()
{
	#ifdef _WINDOWS
		// free keyboard?
		if (!mKeyboardDevice)
			return;

		mKeyboardDevice->Unacquire();
		mKeyboardDevice->Release();
		mKeyboardDevice = NULL;
	#endif // _WINDOWS
}

void Keyboard::getKeyName(uint32 key, wstring &keyName)
{
	#ifdef _WINDOWS
		assert(key < MAX_NUM_OF_KEYBOARD_KEYS);
		DIPROPSTRING keyNameProperty;	// get the localized name by Direct Input
		keyNameProperty.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		keyNameProperty.diph.dwSize = sizeof(DIPROPSTRING);
		keyNameProperty.diph.dwObj = key;
		keyNameProperty.diph.dwHow = DIPH_BYOFFSET;
		mKeyboardDevice->GetProperty(DIPROP_KEYNAME, &keyNameProperty.diph);
		keyName = keyNameProperty.wsz;
	#endif // _WINDOWS
}

#ifdef _WINDOWS
	void Keyboard::init(IDirectInput8 *directInput)
	{
		// create keyboard
		int32 returnValue = directInput->CreateDevice(GUID_SysKeyboard, &mKeyboardDevice, NULL);					
		if (S_OK != returnValue)
		{
			mKeyboardDevice = NULL;
			throw InputException("Could not create a representation of the system mouse. ", 
									 returnValue);
		}
		mKeyboardDevice->SetCooperativeLevel(Window::getSingleton().getWindowHandle(), 
											 DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY);
		mKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
		mKeyboardDevice->Acquire();
	}

	void Keyboard::reacquire()
	{
		reset();
		mKeyboardDevice->Acquire();
	}
#endif // _WINDOWS

#ifdef _LINUX
	void Keyboard::onEndFrame()
	{
		// previous key states = current key states
		// only copy values as mKeyStates are changed by events
		memcpy(mPreviousKeyStates, mKeyStates, sizeof(unsigned char) * MAX_NUM_OF_KEYBOARD_KEYS);
	}

    void Keyboard::processKeyEvent(Key key, bool pressed)
	{
		// changed from up to down
		if (pressed)
		{
			mKeyStates[key]			= 1;
			mPreviousKeyStates[key]	= 0;
		}
		// changed from down to up
		else
		{
			mKeyStates[key]			= 0;
			mPreviousKeyStates[key]	= 1;
		}
	}
#endif // _LINUX

void Keyboard::reset()
{
	mAnyKeyDown = false;
	memset(mKeyStates, 0, sizeof(unsigned char) * MAX_NUM_OF_KEYBOARD_KEYS);
	memset(mPreviousKeyStates, 0, sizeof(unsigned char) * MAX_NUM_OF_KEYBOARD_KEYS);
}

void Keyboard::update()
{
	unsigned char keys[MAX_NUM_OF_KEYBOARD_KEYS];
	memset(keys, 0, MAX_NUM_OF_KEYBOARD_KEYS * sizeof(unsigned char));	

	#ifdef _WINDOWS
		// get keyboard state
		int32 returnValue = mKeyboardDevice->GetDeviceState(MAX_NUM_OF_KEYBOARD_KEYS, keys);			
		if (DI_OK != returnValue)
			return;

		// update previous key states and key states
		memcpy(mPreviousKeyStates, mKeyStates, sizeof(unsigned char) * MAX_NUM_OF_KEYBOARD_KEYS);		
		memcpy(mKeyStates, keys, sizeof(unsigned char) * MAX_NUM_OF_KEYBOARD_KEYS);

		// is any key down?
		mAnyKeyDown = false;
		for (uint32 i = 0; i < MAX_NUM_OF_KEYBOARD_KEYS; ++i)											
		{
			if (0 != mKeyStates[i])
			{
				mAnyKeyDown = true;
				break;
			}
		}
	#endif // _WINDOWS
}
