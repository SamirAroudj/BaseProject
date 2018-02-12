/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifdef _WINDOWS
	// necessary for XInput and DirectInput
	#include <wbemidl.h>
	#include <oleauto.h>
#endif // _WINDOWS

#include "Platform/FailureHandling/InputException.h"
#include "Platform/Input/InputManager.h"
#include "Platform/Window.h"

using namespace FailureHandling;
using namespace Input;
using namespace Platform;

#ifdef _WINDOWS
	#define SAFE_RELEASE(x) if (x) { x->Release(); x = NULL; }

	//-----------------------------------------------------------------------------
	// Enum each PNP device using WMI and check each device ID to see if it contains 
	// "IG_" (ex. "VID_045E&PID_028E&IG_00").  If it does, then it's an XInput device
	// Unfortunately this information can not be found by just using DirectInput 
	// Code from: XInput and DirectInput DirectX Software Development Kit (June 2010) Documentation
	//-----------------------------------------------------------------------------
	BOOL IsXInputDevice(const GUID* pGuidProductFromDirectInput);
#endif // _WINDOWS

InputManager::InputManager() 
{
	#ifdef _WINDOWS 
		mDirectInput = NULL;
		memset(mXboxControllers, NULL, sizeof(Xbox360Controller *) * CONTROLLER_MAX_COUNT);
	#endif // _WINDOWS
		
	// set controller pointers to NULL
	memset(mConnectedControllers, 0, sizeof(GameController *) * CONTROLLER_MAX_COUNT);
	mSomeControllers.clear();

	// for windows extra mouse & keyboard initialization
	#ifdef _WINDOWS
		// valid window for getting the handle?
		if (!Platform::Window::exists())
			return;

		// initialize Direct Input
		HINSTANCE handle = Window::getSingleton().getApplicationHandle();
		int32 result = DirectInput8Create(handle, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<LPVOID *>(&mDirectInput), NULL);
		if (S_OK !=  result)
		{
			mDirectInput = NULL;
			throw InputException("Could not initialize Direct Input", result);
		}
		
		// init devices
		mKeyboard.init(mDirectInput);
		mMouse.init(mDirectInput);
	#endif // _WINDOWS

	initGameControllers();
}

void InputManager::initGameControllers()
{
	#ifdef _WINDOWS
		for (uint32 i = 0; i < CONTROLLER_MAX_COUNT; ++i)
			mXboxControllers[i] = new Xbox360Controller(i);

		int32 callbackReferenceValue = 0;
		mDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL, onFoundGameController,
								  &callbackReferenceValue, DIEDFL_ATTACHEDONLY);
		updateConnectedControllers();
	#endif // _WINDOWS
}

#ifdef _WINDOWS
	BOOL CALLBACK InputManager::onFoundGameController(LPCDIDEVICEINSTANCE device, LPVOID referenceValue)
	{
		if (0 == (device->dwDevType & DI8DEVTYPE_GAMEPAD))
			return DIENUM_CONTINUE;

		// Xbox360 game controller - ignore it as it is managed in mXboxControllers
		if (IsXInputDevice(&device->guidProduct))										
			return DIENUM_CONTINUE;

		// create the representation of a game controller (not Xbox 360)
		InputManager			&manager = InputManager::getSingleton();
		LPDIRECTINPUTDEVICE8	controller;												
		HRESULT					result = manager.mDirectInput->CreateDevice(device->guidInstance, &controller, NULL);
		if (S_OK != result)
			throw InputException("Could not create a game controller representation.", result);
		
		manager.mSomeControllers.push_back(new SomeController(controller));

		return DIENUM_CONTINUE;
	}
#endif // _WINDOWS

#ifdef _WINDOWS
	BOOL IsXInputDevice( const GUID* pGuidProductFromDirectInput )
	{
		IWbemLocator*           pIWbemLocator  = NULL;
		IEnumWbemClassObject*   pEnumDevices   = NULL;
		IWbemClassObject*       pDevices[20]   = {0};
		IWbemServices*          pIWbemServices = NULL;
		BSTR                    bstrNamespace  = NULL;
		BSTR                    bstrDeviceID   = NULL;
		BSTR                    bstrClassName  = NULL;
		DWORD                   uReturned      = 0;
		bool                    bIsXinputDevice= false;
		UINT                    iDevice        = 0;
		VARIANT                 var;
		HRESULT                 hr;

		// CoInit if needed
		hr = CoInitialize(NULL);
		bool bCleanupCOM = SUCCEEDED(hr);

		// Create WMI
		hr = CoCreateInstance( __uuidof(WbemLocator),
							   NULL,
							   CLSCTX_INPROC_SERVER,
							   __uuidof(IWbemLocator),
							   (LPVOID*) &pIWbemLocator);
		if ( FAILED(hr) || pIWbemLocator == NULL )
			goto LCleanup;

		bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" );if ( bstrNamespace == NULL ) goto LCleanup;        
		bstrClassName = SysAllocString( L"Win32_PNPEntity" );   if ( bstrClassName == NULL ) goto LCleanup;        
		bstrDeviceID  = SysAllocString( L"DeviceID" );          if ( bstrDeviceID == NULL )  goto LCleanup;        
    
		// Connect to WMI 
		hr = pIWbemLocator->ConnectServer( bstrNamespace, NULL, NULL, 0L, 
										   0L, NULL, NULL, &pIWbemServices );
		if ( FAILED(hr) || pIWbemServices == NULL )
			goto LCleanup;

		// Switch security level to IMPERSONATE. 
		CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, 
						   RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE );                    

		hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, NULL, &pEnumDevices ); 
		if ( FAILED(hr) || pEnumDevices == NULL )
			goto LCleanup;

		// Loop over all devices
		for ( ;; )
		{
			// Get 20 at a time
			hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );
			if ( FAILED(hr) )
				goto LCleanup;
			if ( uReturned == 0 )
				break;

			for ( iDevice=0; iDevice<uReturned; iDevice++ )
			{
				// For each device, get its device ID
				hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, NULL, NULL );
				if ( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL )
				{
					// Check if the device ID contains "IG_".  If it does, then it's an XInput device
						// This information can not be found from DirectInput 
					if ( wcsstr( var.bstrVal, L"IG_" ) )
					{
						// If it does, then get the VID/PID from var.bstrVal
						DWORD dwPid = 0, dwVid = 0;
						WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
						if ( strVid && swscanf_s( strVid, L"VID_%4X", &dwVid ) != 1 )
							dwVid = 0;
						WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
						if ( strPid && swscanf_s( strPid, L"PID_%4X", &dwPid ) != 1 )
							dwPid = 0;

						// Compare the VID/PID to the DInput device
						DWORD dwVidPid = MAKELONG( dwVid, dwPid );
						if ( dwVidPid == pGuidProductFromDirectInput->Data1 )
						{
							bIsXinputDevice = true;
							goto LCleanup;
						}
					}
				}   
				SAFE_RELEASE( pDevices[iDevice] );
			}
		}

	LCleanup:
		if (bstrNamespace)
			SysFreeString(bstrNamespace);
		if (bstrDeviceID)
			SysFreeString(bstrDeviceID);
		if (bstrClassName)
			SysFreeString(bstrClassName);
		for ( iDevice=0; iDevice<20; iDevice++ )
			SAFE_RELEASE( pDevices[iDevice] );
		SAFE_RELEASE( pEnumDevices );
		SAFE_RELEASE( pIWbemLocator );
		SAFE_RELEASE( pIWbemServices );

		if ( bCleanupCOM )
			CoUninitialize();

		return bIsXinputDevice;
	}
#endif // WINDOWS


InputManager::~InputManager()
{	
	// release Xbox controllers
	#ifdef _WINDOWS
		for (uint32 i = 0; i < CONTROLLER_MAX_COUNT; ++i)
			delete mXboxControllers[i];
	#endif // _WINDOWS

	// release other game controllers
	size_t numOfSomeControllers = mSomeControllers.size();
	for (size_t i = 0; i < numOfSomeControllers; ++i)
		delete mSomeControllers[i];

	// release direct input
	#ifdef _WINDOWS
		if (mDirectInput)
			mDirectInput->Release();
	#endif // _WINDOWS
}

const GameController &InputManager::getGameController(uint32 number) const
{
	assert(number < CONTROLLER_MAX_COUNT);
	assert(number < getNumberOfConnectedGameControllers());

	return *(mConnectedControllers[number]);
}	

GameController &InputManager::getGameController(uint32 number)
{
	assert(number < CONTROLLER_MAX_COUNT);
	assert(number < getNumberOfConnectedGameControllers());

	return *(mConnectedControllers[number]);
}

uint32 InputManager::getNumberOfConnectedGameControllers() const
{
	uint32 number = 0;

	for (uint32 i = 0; i < CONTROLLER_MAX_COUNT; ++i)
		if (mConnectedControllers[i])
			++number;

	return number;
}

#ifdef _LINUX
	void InputManager::onEndFrame()
	{
		mKeyboard.onEndFrame();
		mMouse.onEndFrame();
	}
#endif // _LINUX

#ifdef _WINDOWS
	void InputManager::reacquireDevices()
	{
		// reacquire access to mouse, keyboard and SomeController devices								
		mMouse.reacquire();
		mKeyboard.reacquire();

		size_t number = mSomeControllers.size();
		for (size_t i = 0; i < number; ++i)
			mSomeControllers[i]->reacquire();
	}
#endif // _WINDOWS

void InputManager::resetDevices()
{
	// reset mouse, keyboard and 												
	mMouse.reset();
	mKeyboard.reset();

	// reset Xbox controllers
	#ifdef _WINDOWS
		for (uint32 i = 0; i < CONTROLLER_MAX_COUNT; ++i)
			mXboxControllers[i]->reset();
	#endif // _WINDOWS

	// reset other game controllers
	size_t number = mSomeControllers.size();
	for (size_t i = 0; i < number; ++i)
		mSomeControllers[i]->reset();
}

void InputManager::update()
{
	// update mouse, keyboard
	mKeyboard.update();
	mMouse.update();

	// game controllers
	// did the connection state of any controller change?
	bool connectionStateChanged = false;								

	size_t number = mSomeControllers.size();
	for (size_t i = 0; i < number; ++i)
		connectionStateChanged |= mSomeControllers[i]->update();

	#ifdef _WINDOWS
		for (uint32 i = 0; i < CONTROLLER_MAX_COUNT; ++i)
			connectionStateChanged |= mXboxControllers[i]->update();
	#endif //_WINDOWS

	// reorder game controllers if a connection changed		
	if (!connectionStateChanged)											
		return;

	updateConnectedControllers();
}

void InputManager::updateConnectedControllers()
{
	// gather up to 4 connected controllers
	uint32 j = 0;
	#ifdef _WINDOWS
		// first Xbox 360 controllers
		for (uint32 i = 0; i < CONTROLLER_MAX_COUNT; ++i)
			if (mXboxControllers[i]->isConnected())
				mConnectedControllers[j++] = mXboxControllers[i];
	#endif // _WINDOWS

	// then other controllers
	size_t number = mSomeControllers.size();
	for (size_t i = 0; i < number && j < CONTROLLER_MAX_COUNT; ++i)
		if (mSomeControllers[i]->isConnected())
			mConnectedControllers[j++] = mSomeControllers[i];

	//  set rest to NULL
	for (uint32 i = j; i < CONTROLLER_MAX_COUNT; ++i)									
		mConnectedControllers[i] = NULL;
}
