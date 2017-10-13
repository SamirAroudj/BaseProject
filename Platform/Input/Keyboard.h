/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <string>
#include "KeyboardMapping.h"
#include "Platform/Input/MagicConstants.h"

#ifdef _WINDOWS
	#include "dinput.h"
#endif // _WINDOWS

namespace Input
{
	/// This is a representation of the system keyboard.
	class Keyboard
	{
	public:
		/** Default constructor creates the keyboard and resets all its values. */
		Keyboard();

		/** Releases the keyboard device */
		~Keyboard();

		/** Gets a localized key name by Direct Input.
		@param key The Direct Input key identifier with 0 <= key <= 255. 
		@param keyName [out] Localized key name. */
		void getKeyName(uint32 key, std::wstring &keyName);

		#ifdef _WINDOWS
			/** Does windows specific initialization by means of direct input context.
			@param directInput This is a pointer to the main direct input object. */
			void init(IDirectInput8 *directInput);
		#endif // _WINDOWS

		/** Query if any key is currently down regardless whether it was down before or not.
		@return Returns true if any key is currently down whether it was down before or not. */
		bool isAnyKeyDown() const { return mAnyKeyDown; }

		/** Query if the entered key is currently down regardless whether it was down before or not.
		@param key That is the key identifier. Take a look at the key enumeration in KeybaordMapping.h.
		@return Returns true if the entered key is currently down regardless whether it was down before or not.*/
		bool isKeyDown(Key key) const { return 0 != mKeyStates[key]; }

		/** Query if the entered key has been pressed down. (It changed its state.)
		*	That is, it is down since the latest update and was not down during the second last update.
		@param key That is the key identifier. Take a look at the key enumeration in KeybaordMapping.h. 
		@return Returns true if the entered key has been pressed down.
				That is, it is down since the latest update and was not down during the second last update.*/
		bool isKeyPressed(Key key) const { return (0 != mKeyStates[key]) && (0 == mPreviousKeyStates[key]); }

		/** Query if the entered key has been released. (It changed its state.)
		*	That is, it is up since the latest update and it was down during the second last update.
		@param key That is the key identifier. Take a look at the key enumeration in KeybaordMapping.h.
		@return Returns true if the entered key has been released.
				That is, it is up since the latest update and was down during the second last update.*/
		bool isKeyReleased(Key key) const { return (0 == mKeyStates[key]) && (0 != mPreviousKeyStates[key]); }

		/** Query if the entered key is up regardless whether it was up before or not.
		@param key That is the key identifier. Take a look at the key enumeration in KeybaordMapping.h.
		@return Returns true if the entered key is up regardless whether it was up before or not.*/
		bool isKeyUp(Key key) const { return 0 == mKeyStates[key]; }

		#ifdef _LINUX
			/** Should only be called by InputManager object to finish xlib event processing of this frame. */
			void onEndFrame();

			/** Should be used to process keyboard events from xlib.
			@param button Identifies the pressed button by containing an xlib key code.
			@param pressed Set this to true if the event has been triggered by a key that has been pressed down last frame.
				Set pressed to false if the key has been released last frame. */
            void processKeyEvent(Key keyCode, bool pressed);
		#endif // _LINUX
		
		#ifdef _WINDOWS
			/** Reacquire keyboard device the control of which is lost when another application is set to the foreground. */
			void reacquire();
		#endif // _WINDOWS

		/** Resets the key states. Every key will be considered as beeing up and there are no state changes stored after this function call. */
		void reset();

		/** Updates the key states. The array should contain MAX_NUM_OF_KEYBOARD_KEYS keys.
		*	Every key that is up should be 0 and every key that is down should not be 0. */
		void update();

	private:
		#ifdef _WINDOWS	
			LPDIRECTINPUTDEVICE8 mKeyboardDevice;						/// The Direct Input representation of the system keyboard.
		#endif // _WINDOWS

		unsigned char	mKeyStates[MAX_NUM_OF_KEYBOARD_KEYS];			/// Contains the states of the keys from the last update. 
																		/// If mKeyStates[x] == 0 then key x is up otherwise it is down. 
		unsigned char	mPreviousKeyStates[MAX_NUM_OF_KEYBOARD_KEYS];	/// Contains the states of the keys from the second last update.
																		/// If mKeyStates[x] == 0 then key x is up otherwise it is down.
		bool			mAnyKeyDown;									/// true when any key is currently down				
	};
}

#endif // _KEYBOARD_H_
