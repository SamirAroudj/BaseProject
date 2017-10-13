/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <vector>

#include "Platform/DataTypes.h"
#ifdef _WINDOWS
	#include "dinput.h"
#endif // _WINDOWS
#include "Platform/Input/Keyboard.h"
#include "Platform/Input/Mouse.h"
#include "Patterns/Singleton.h"
#include "Platform/Input/SomeController.h"
#include "Platform/Input/Xbox360Controller.h"

/// Contains classes that are responsible for user input.
namespace Input
{
	/// The input manager takes care of basic initialization and creation of particular input device representations. 
	class InputManager : public Patterns::Singleton<InputManager>
	{
	public:
		/** Executes basic initialization and creates available devices. */
		InputManager();

		/** Destructor releases requested resources, such as device representations. */
		virtual ~InputManager();

		/** Obtain one of the created game controller representations.
		@param number Defines what game controller is returned. Make sure that: number < CONTROLLER_MAX_COUNT.
		@return The representation of the requested game controller is returned. */
		const GameController &getGameController(uint32 number) const;

		/** Obtain one of the created game controller representations.
		@param number Defines what game controller is returned. Make sure that: number < CONTROLLER_MAX_COUNT.
		@return The representation of the requested game controller is returned. */
		GameController &getGameController(uint32 number);

		/** Obtain keyboard representation to query key states.
		@return The system keyboard representation is returned. */
		const Keyboard &getKeyboard() const { return mKeyboard; }

		/** Obtains the mouse to query its normalized analogue values or its digital values.
		@return The representation of the system mouse is returned.*/
		Mouse &getMouse() { return mMouse; }

		/** Obtains the mouse to query its normalized analogue values or its digital values.
		@return The representation of the system mouse is returned.*/
		const Mouse &getMouse() const { return mMouse; }

		/** Queries the number of found and created game controllers.
		@return The number of exisiting game controller representations is returned. always true: return value <= CONTROLLER_MAX_COUNT*/
		uint32 getNumberOfConnectedGameControllers() const;

		#ifdef _LINUX
			/** Should only be called by Application object to finish xlib event processing for this frame.*/
			void onEndFrame();
		#endif // _LINUX

		#ifdef _WINDOWS
			/** Reacquire input devices the control of which is lost when another application is set to the foreground. */
			void reacquireDevices();
		#endif // _WINDOWS

		/** Resets the devices to a state as if no input was received. */
		void resetDevices();

		/** Updates the state of every input device. Should be called once every main loop iteration. */
		void update();

	private:
		/** Searches game controllers and initializes devices that were found. */
		void initGameControllers();

		#ifdef _WINDOWS
			/** This callback function is called for every game controller found.
			@param deviceInstance This structure describes the device instance.
			@param referenceValue This is an application defined value passed to EnumDevices().
			@return Returns either DIENUM_CONTINUE to continue the enumeration or DIENUM_STOP to stop the enumeration. */
			static BOOL CALLBACK onFoundGameController(LPCDIDEVICEINSTANCE deviceInstance, LPVOID referenceValue);
		#endif // _WINDOWS

		/** Sets the elements of mConnectedControllers whereas Xbox360 controllers are in front of this array. */
		void updateConnectedControllers();

	private:
		Keyboard mKeyboard;												/// the representation of the system keyboard
		Mouse mMouse;													/// the representation of the sysetm mouse with normalized values
		std::vector<SomeController *> mSomeControllers;					/// contains all attached game controllers of some product type (not Xbox360)	
		GameController *mConnectedControllers[CONTROLLER_MAX_COUNT];	/// contains up to four controllers which are ordered

		#ifdef _WINDOWS
			Xbox360Controller *mXboxControllers[CONTROLLER_MAX_COUNT];	/// contains all Xbox360Controllers - each controller can be disconnected
			IDirectInput8 *mDirectInput;								/// Chief Direct Input interface.
		#endif // _WINDOWS
	};
}

#endif // _INPUT_MANAGER_H_
