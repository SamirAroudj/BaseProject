/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _XBOX_360_CONTROLLER_H_
#define _XBOX_360_CONTROLLER_H_
#ifdef _WINDOWS

#include "GameController.h"

namespace Input
{
	/** This class is used to represent Microsoft's Xbox 360 controllers. */
	class Xbox360Controller : public GameController
	{
	public:
		/// Buttons that an Xbox 360 controller provides
		enum Buttons
		{
			DPAD_UP,
			DPAD_DOWN,
			DPAD_LEFT,
			DPAD_RIGHT,
			START,
			BACK,
			LEFT_THUMB,
			RIGHT_THUMB,
			L1,
			R1,
			L2,
			R2,
			A,
			B,
			X,
			Y,
			NUM_OF_BUTTONS
		};
		
    public:
		/** Creates a Xbox 360 controller device representation.
		@param number This is the ID of the controller. Up to four Xbox 360 controllers are supported.*/
		Xbox360Controller(uint32 number);
		
		/** Releases the Xbox 360 controller device representation. */
		virtual ~Xbox360Controller();

		/** Returns three. The supported "sticks" are: left thumb stick, right thumb stick and the four digital directions buttons
		@return Returns three. The supported "sticks" are: left thumb stick, right thumb stick and the four digital directions buttons */
		virtual uint32 getNumberOfSupportedSticks() const { return 3; }

		/** Queries the type of the game controller, see GameController.h for supported types.
		@return XBOX_360 is returned. Returns the actual (most specialized) type of the game controller. */
		virtual ControllerType getType() const { return XBOX_360; }

		/** The maximum number of sticks is supported by the Xbox 360 controller.
			There are the left and right thumb stick as well as the four arrow buttons.
		@param number True is and number is ignored.
		@return True is always returned. */
		virtual bool isStickSupported(Stick number) const { return true; }

		/** Returns true. The z-axis is controlled by the two analog triggers. Whereas the left trigger is reponsible for values � [-1.0f, 0.0f) and
			the right trigger is responsible for the z-axis values � (0.0f, 1.0f].
		@return True is returned as the z-axis is represented by the two analoge triggers of an Xbox 360 controller. */
		virtual bool isZAxisSupported() const { return true; }
		
		/** Updates the state of the Xbox 360 controller. Should be called every frame.
		@return Returns true when the connection state of the controller changed. */
		virtual bool update();

	private:
        /** Copy constructor is forbidden.
        @param copy Copy constructor is forbidden. */
        Xbox360Controller(const Xbox360Controller &copy) { assert(false); }

        /** Operator is forbidden.
        @param rhs Operator is forbidden. */
        Xbox360Controller &operator =(const Xbox360Controller &rhs) { assert(false); return *this; }

		/** Sets the values of the axes AXIS_POVX and AXIS_POVY.
		@param This should be the value of XINPUT_GAMEPAD.wButtons. */
		void updatePOVAxes(uint32 buttons);

		/** Computes the value for the axis AXIS_Z according to trigger values.
		@param leftTrigger This should be the value of XINPUT_GAMEPAD.bLeftTrigger.
		@param rightTrigger This should be the value of XINPUT_GAMEPAD.bRightTrigger. */
		void updateZAxis(unsigned char leftTrigger, unsigned char rightTrigger);

    private:
		uint32 mNumber;		/// controller ID � [0, 3] - up to four Xbox 360 controllers are supported
	};
}

#endif // _WINDOWS
#endif // _XBOX_360_CONTROLLER_H_
