/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <cassert>
#include "Platform/DataTypes.h"

#ifdef _WINDOWS
#include "dinput.h"
#endif // _WINDOWS

namespace Input
{
	/// Representation of the mouse device which includes relative and absolute coordinates and mouse button states.
	class Mouse
	{
	public:
        /// Defines logical mouse buttons which can be queried for their state.
		enum Button
		{
			BUTTON_PRIMARY		= 0,	/// usually left mouse button
			BUTTON_SECONDARY	= 1,	/// usually right mouse button
			BUTTON_MIDDLE		= 2,	/// usually triggered if scroll wheel is pushed
			BUTTON_BACK			= 3,	/// usually back / first left side button which is closer to the user than BUTTON_FORWARD
			BUTTON_FORWARD		= 4,	/// usually front / second left side botton which is farther from the user than BUTTON_BACK
			BUTTON_FAIL			= 5		/// used if mouse button could not be identified
		};

		/** Creates the mouse and reset its values. See reset(). */
		Mouse();

		/** Releases the mouse device. */
		~Mouse();

		/** Gets the absolute x-coordinate of the mouse which is between -1.0f and 1.0f.
			-1.0f and 1.0f correspond to the left and right edge of the main window respectively.
		@return The absolute x-coordinate is normalized to a range from -1.0f to 1.0f. */
		Real getAbsoluteX() const { return mAbsoluteX; }

		/** Gets the absolute y-coordinate of the mouse which is between -1.0f and 1.0f.
			-1.0f and 1.0f correspond to the bottom and top edge of the main window respectively.
		@return The absolute x-coordinate is normalized to a range from -1.0f to 1.0f.*/
		Real getAbsoluteY() const { return mAbsoluteY; }

		#ifdef _WINDOWS
			/** Gets the scale factor to translate relative mouse motions along the x-axis into changes of the absolute x-coordinate.
			*	It is smaller than 1.0f and bigger than 0.0f.
			@return The factor which translates relative mouse motions along the x-axis into changes of the absolute x-coordinate is returned.
					factor > 0.0f && factor <= 1.0f*/
			Real getRelativeToAbsoluteScaleFactorX() const { return mRelativeToAbsoluteScaleFactorX; }

			/** Gets the scale factor to translate relative mouse motions along the y-axis into changes of the absolute y-coordinate.
			*	It is smaller than 1.0f and bigger than 0.0f.
			@return The factor which translates relative mouse motions along the y-axis into changes of the absolute y-coordinate is returned.
					factor > 0.0f && factor <= 1.0f*/
			Real getRelativeToAbsoluteScaleFactorY() const { return mRelativeToAbsoluteScaleFactorY; }
		#endif // _WINDOWS

		/** Gets the normalized relative motion of the mouse along the x-axis. Ranges from -1.0f to 1.0f,
		*	whereas negative values indicate mouse movements to the left of the user. The value is relative to the x-coordinates from the last update.	
		@return The normalized relative mouse motion along the x-Axis is returned. ([-1.0f, 1.0f]) */
		Real getRelativeXMotion() const { return mRelativeXMotion; }

		/** Gets the normalized relative motion of the mouse along the y-axis. Ranges from -1.0f to 1.0f,
		*	whereas negative values indicate mouse movements towards the user. The value is relative to the y-coordinate from the last update.
		@return The normalized relative mouse motion along the y-Axis is returned. ([-1.0f, 1.0f])*/
		Real getRelativeYMotion() const { return mRelativeYMotion; }

		/** Gets the normalized relative motion of the mouse along the z-axis. Z-axis movements are caused by the mouse wheel.
		*	Ranges from -1.0f to 1.0f whereas negative values indicate mouse wheel movements towards the user.
		*	The value is relative to the z-coordinate from the last update.
		@return The normalized relative mouse motion along the z-Axis is returned. ([-1.0f, 1.0f]) */
		Real getRelativeZMotion() const { return mRelativeZMotion; }
		
		#ifdef _WINDOWS
			/** Does windows specific initialization by means of direct input context.
			@param directInput This is a pointer to the main direct input object. */
			void init(IDirectInput8 *directInput);
		#endif // _WINDOWS

		/** Query if any button is down regardless whether it was down before or not.
		@return Returns true if any button is down regardless whether it was down before or not. */
		bool isAnyButtonDown() const { return 0 != mButtonStates; }

		/** Query if the button is down regardless whether it was down before or not.
		@param button This is the button identifier. See Mouse::Button enumeration.
		@return Returns true if the button is down regardless whether it was down before or not.*/
		bool isButtonDown(Button button) const;

		/** Query if the button has been pressed down during the latest update and was up during the update before.
		@param button This is the button identifier. See Mouse::Button enumeration.
		@return Returns true if the button has been pressed down during the latest update and was up during the update before. */
		bool isButtonPressed(Button button) const;

		/** Query if the button has been realeased (is up now) during the latest update and was down during the update before.
		@param button This is the button identifier. See Mouse::Button enumeration.
		@return Returns true if the indicated button has been realeased (is up now) during the latest update and was down during the update before. */
		bool isButtonReleased(Button button) const;

		/** Query if the button is up regardless whether it was up before or not.
		@param button This is the button identifier. See Mouse::Button enumeration.
		@returns Returns true if the button is up regardless wheter it was up before or not.*/
		bool isButtonUp(Button button) const;

		// todo move this
		/** Returns true when a user / player uses this mouse device.
		@return False is returned if nobody uses this device. */
		bool isInUse() const { return mInUse; }

		#ifdef _LINUX
			/** Should only be called by InputManager object to finish xlib event processing of this frame. */
			void onEndFrame();

			/** Should be used to process mouse button events from xlib.
			@param button This is the button identifier. See Mouse::Button enumeration.
			@param pressed Set this to true if the event has been triggered by a button that has been pressed down last frame.
				Set pressed to false if the button has been released last frame. */
			void processButtonEvent(Button button, bool pressed);

			/** Should be used to process mouse motion events from xlib.
			@param absoluteX The new absolute x-coordinate must be normalized and relative to the main window. (coordinate \in [-1, 1])
					-1.0f, 0.0f and 1.0f identify the left edge, center and right edge of the main window respectively.
			@param absoluteY The new absolute y-coordinate must be normalized and relative to the main window. (coordinate \in [-1, 1])
					-1.0f, 0.0f and 1.0f identify the bottom edge, center and top edge of the main window respectively.*/
			void processMotionEvent(Real absoluteX, Real absoluteY);

			/** Should be used to process mouse motion events from xlib. They are actually button 4 or button 5 events in xlib for some strange reason.
			@param forward Set this to true if the mouse wheel is turned forward (away from the user, positive z motion).
				Set this to false if the mouse wheel is turned backward (toward the user, negative z motion).*/
			void processWheelMotionEvent(bool forward);
		#endif // _LINUX

		#ifdef _WINDOWS
			/** Reacquire mouse device the control of which is lost when another application is set to the foreground. */
			void reacquire();
		#endif // _WINDOWS

		/** Resets this object. That means that all buttons are up and not pressed or released, 
		*	relative motions are zero and absolute coordinates are set to zero, too. */
		void reset();

		/** Sets the absolute x-coordinate of the mouse which must be normalized (coordinate \in [-1, 1]).
		@param absoluteX The new absolute x-coordinate must be normalized (coordinate \in [-1, 1]).
				-1.0f, 0.0f and 1.0f identify the left edge, center and right edge of the main window respectively.*/
		void setAbsoluteX(Real absoluteX)
		{
			assert(absoluteX >= -1.0f);
			assert(absoluteX <=  1.0f);
			mAbsoluteX = absoluteX;
		}

		/** Sets the absolute y-coordinate of the mouse which must be normalized (coordinate \in [-1, 1]).
		@param absoluteY The new absolute y-coordinate must be normalized (coordinate \in [-1, 1]).
				-1.0f, 0.0f and 1.0f identify the bottom edge, center and top edge of the main window respectively.*/
		void setAbsoluteY(Real absoluteY)
		{
			assert(absoluteY >= -1.0f);
			assert(absoluteY <=  1.0f);
			mAbsoluteY = absoluteY;
		}

		/** Sets if someone uses this mouse.
		@inUse Set this to true if someone uses this device otherwise set it to false. */
		void setInUse(bool inUse) { mInUse = inUse; }

		#ifdef _WINDOWS
			/** Sets the factor to translate relative mouse motions along the x-axis into changes of the absolute x-coordinate.
			*	The factor must be greater than 0.0f and smaller than 1.0f.
			@param factor This is the factor to translate relative mouse motions along the x-axis into changes of the absolute x-coordinate.
						  The factor must be greater than 0.0f and smaller than 1.0f.*/
			void setRelativeToAbsoluteScaleFactorX(Real factor)
			{
				assert(factor > 0.0f && factor < 1.0f);
				mRelativeToAbsoluteScaleFactorX = factor;
			}

			/** Sets the factor to translate relative mouse motions along the y-axis into changes of the absolute y-coordinate.
			*	The factor must be greater than 0.0f and smaller than 1.0f.
			@param factor The factor to translate relative mouse motions along the y-axis into changes of the absolute y-coordinate.
						  The factor must be greater than 0.0f and smaller than 1.0f.*/
			void setRelativeToAbsoluteScaleFactorY(Real factor)
			{
				assert(factor > 0.0f && factor < 1.0f);
				mRelativeToAbsoluteScaleFactorY = factor;
			}
		#endif // _WINDOWS

		/** Should be called every frame to update the state of the mouse. */
		void update();

	private:
		/** Clamps all relative motion values to stay inside [-1.0f, 1.0f];*/
		void clampRelativeMotion();

//		/** Assigns physical mosue buttons to logical buttons.
//			E.g. Makes sure that primary button (usually left mouse button) has always ID=0,
//			secondary button (usually right mouse button) is assigned ID=1 and
//			the middle mouse button is always identified by 2.**/
//		void setButtonMapping();

	private:
		#ifdef _WINDOWS
			LPDIRECTINPUTDEVICE8 mMouseDevice;			/// The Direct Input representation of the system mouse.
		#endif // _WINDOWS

		Real			mRelativeXMotion;				/// the relative x coordinate motion which is betweeen -1.0f and 1.0f
		Real			mRelativeYMotion;				/// the relative y coordinate motion which is between -1.0f and 1.0f
		Real			mRelativeZMotion;				/// the relative z coordinate motion which is between -1.0f and 1.0f

		Real			mAbsoluteX;						/// the absolute x coordinate of the mouse which is between -1.0f and 1.0f, -1 = left edge, 0 = center, 1 = right edge (of main window)
		Real			mAbsoluteY;						/// the absolute y coordinate of the mouse which is between -1.0f and 1.0f, -1 = bottom edge, 0 = center, 1 = top edge (of main window)

		#ifdef _WINDOWS
            Real mRelativeToAbsoluteScaleFactorX;       /// Defines how the relative mouse motions along the x-Axis are added to the absolute position.
                                                        /// Should be smaller than 1.0f.
            Real mRelativeToAbsoluteScaleFactorY;       /// Defines how the relative mouse motions along the y-Axis are added to the absolute position.
                                                        ///  Should be smaller than 1.0f.
		#endif // _WINDOWS

		unsigned char	mButtonStates;					/// Contains up to 8 button states from the last update with values like mButtonStates & 0x1 == 1 -> 
														/// button 0 is down or mButtonStates & 0x1 == 0 -> button 0 is up.
		unsigned char	mPreviousButtonStates;			/// Contains up to 8 button states from the second last update with values like mButtonStates & 0x1 == 1 ->
														/// button 0 is down or mButtonStates & 0x1 == 0 -> button 0 is up.

		// todo: move this
		bool			mInUse;							/// true when this mouse is used by a player
	};
}

#endif // _MOUSE_H_
