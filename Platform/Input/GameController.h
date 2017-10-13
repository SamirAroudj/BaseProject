/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _GAME_CONTROLLER_H_
#define _GAME_CONTROLLER_H_

#include <cassert>
#include <string>
#include "Math/Vector2.h"
#include "Platform/DataTypes.h"
#include "Platform/Input/MagicConstants.h"

namespace Input
{
	/// Represents a game controller device.
	class GameController
	{
	public:
		/** Defines the game controller stick identifiers / types. */
		enum Stick
		{
			LEFT_STICK,		/// left thumb stick of a game controller
			RIGHT_STICK,	/// right thumb stick of a game controller
			POV_STICK,		/// also coupling of an x-axis with a y-axis, e.g. four digitial arrow buttons
			NUM_OF_STICKS,	/// number of supported stick types
			NO_STICK		/// indicates that no stick is used
		};

		/** Defines what kind of controller representations are implemented / available */
		enum ControllerType
		{
			XBOX_360,			/// Xbox 360 game pad
			SOME_CONTROLLER		/// a game pad of some unknown type
		};

    public:
		/** Creates a controller device representation.*/
		GameController();

		/** Releases the game controller device. */
		virtual ~GameController() { }

		/** Converts a stick name string to a value of the Stick enumeration.
		@param stickName This is the string that is converted, examples: "LEFT_STICK", "NO_STICK". 
		@return A value of the Stick enumeration is returned that corresponds to the entered name.
				NO_STICK is returned when stickName is invalid. */
		static Stick convertToStick(const std::string &stickName);

		/** Returns the number of controller "sticks" that are supported.
			A stick is essentially a coupling of an x-axis with a y-axis.
			Examples: four digital direction buttons, thumb stick of a Xbox 360 controller, point-of-view controller etc. 
		@return Returns the number of controller sticks a controller supports. The returned value is always smaller than NUM_OF_STICKS. */
		virtual uint32 getNumberOfSupportedSticks() const = 0;

		/** Queries the current position of an controller stick, e.g. a thumb stick or four digital arrow buttons. 
			Check whether the stick identified by number is supported by means of isStickSupported(uint32 number).
		@param number Defines what stick position is returned. Up to NUM_OF_STICKS controller sticks are supported.
					  Therefore number must be smaller than NUM_OF_STICKS.
		@return The returned vector is "normalized" for easy usage. length € [0.0f, 1.0f], each coordinate € [-1.0f, 1.0f] */
		const Math::Vector2 &getStickPosition(Stick number) const
		{
			assert(number < NUM_OF_STICKS);
			return mSticks[number];
		}

		/** Queries the type of the game controller, see GameController.h for supported types.
		@return Returns the actual (most specialized) type of the game controller. */
		virtual ControllerType getType() const = 0;

		/** Queries the current position of the z-Axis of this controller.
		@return The returned value represents the current position of the controller's z-axis. It is normalized, result € [-1.0f, 1.0f]. */
		Real getZAxisValue() const { return mZAxis; }
				
		/** Query if any button is down regardless whether it was down before or not.
		@return Returns true if any button is down regardless whether it was down before or not. */
		bool isAnyButtonDown() const { return 0 != mButtonStates; }

		/** Query if the button identified by buttonNumber is down regardless whether it was down before or not. 
		@param buttonNumber This is the button identifier.
		@return Returns true if the button is down regardless wheter it was down before or not.*/
		bool isButtonDown(uint32 buttonNumber) const;

		/** Query if the button identified by buttonNumber has been pressed down during the latest update and was up during the update before. 
		@param buttonNumber This is the button identifier.
		@return Returns true if the button has been pressed down during the latest update and was up during the update before. */
		bool isButtonPressed(uint32 buttonNumber) const;

		/** Query if the button identified by buttonNumber has been realeased (is up now) during the latest update and was down during the update before. 
		@param buttonNumber This is the button identifier.
		@return Returns true if the indicated button has been realeased (is up now) during the latest update and was down during the update before. */
		bool isButtonReleased(uint32 buttonNumber) const;

		/** Query if the button identified by buttonNumber is up regardless whether it was up before or not. 
		@param buttonNumber This is the button identifier.
		@returns Returns true if the button is up regardless wheter it was up before or not.*/
		bool isButtonUp(uint32 buttonNumber) const;

		/** Query whether this Xbox 360 controller is connected.
		@return Returns true when the game controller is connected and can be used. */
		bool isConnected() const { return mConnected; }

		/** Returns true when a user / player uses this game controller device.
		@return False is returned if nobody uses this device. */
		bool isInUse() const { return mInUse; }

		/** Queries whether the game controller device supports the stick idntified by number.
		@param number Up to NUM_OF_STICKS sticks are supported. Therefore number must be smaller than NUM_OF_STICKS. 
		@return True is returned when the stick identified by number returns other values than Vector(0.0f, 0.0f) as the device has this stick. */
		virtual bool isStickSupported(Stick number) const = 0;

		/** Queries whether the game controller has a z-Axis.
		@return True is returned when z-Axis values are set. Otherwise z-Axis values are always 0.0f. */
		virtual bool isZAxisSupported() const = 0;

		/** Reset the game controller so that each button is considered to be up and axis are considered to be in start position. */
		void reset();

		/** Sets if someone uses this game controller device.
		@inUse Set this to true if someone uses this device otherwise set it to false. */
		void setInUse(bool inUse) { mInUse = inUse; }

		/** Sets if y-axes of game controller sticks are inverted.
		@param invertYAxes If this is true then all game controller stick y-axes are multiplied by -1.0f. */
		void setInvertYAxes(bool invertYAxes) { mInvertYAxes = invertYAxes; }

		/** Updates the state of the game controller. Should be called every frame. 
		@return Returns true when the connection state of the controller changed. */
		virtual bool update() = 0;

	protected:
		/** Inverts the y-axis of each stick if mInvertYAxis is true. */
		void adaptStickYAxes();

		/* Computes the "normalized" axis position according to raw input data.
		@param axisValue This is a raw axis value of the game controller.
		@param deadZone Defines a minimum displacement for the stick. Requirement: minAxisRange <= deadZone <= maxAxisRange
						If the displacement is smaller then center position is reported.
						(-> circular dead zone where x and y values are set to fit center)
						This is used to avoid reported axis positions that are not equal to the center position although the stick is not moved.
		@param invertedRangeFactor This should be equal to 1.0f / (maxMagnitude - deadZone).
		@return Describes the "normalized" axis position. returned value € [-1.0f, 1.0f]
		Real computeNormalizedAxisValue(int32 axisValue, uint32 deadZone, Real invertedRangeFactor); */

		/** Computes the "normalized" game controller stick position according to raw input data.
		@param x This is a raw x-axis value of the stick of the game controller.
		@param y This is a raw y-axis value of the stick of the game controller.
		@param deadZone Defines a minimum displacement for the stick. Requirement: minAxisRange <= deadZone <= maxAxisRange
						If the displacement is smaller than the deadzone then center position  (= [0.0f/0.0f]) is reported.
						(So this is a circular dead zone where x and y values are set to fit the physical center.)
						This is used to avoid reported axis positions that are not equal to the center position although the stick is not moved.
		@param maxMagnitude This value must contain the maximum magnitude of both axes. (e.g. 32767 for a Xbox 360 controller)
		@param invertedRangeFactor This must be equal to 1.0f / (maxMagnitude - deadZone).
		@param result This vector contains the "normalized" stick position upon return whereas the following is always true:
					  result.length € [0.0f, 1.0f], result.x € [-1.0f, 1.0f], result.y € [-1.0f, 1.0f]. */
		void computeNormalizedStickPosition(int32 x, int32 y, uint32 deadZone, uint32 maxMagnitude,
											Real invertedRangeFactor, Math::Vector2 &result);

	protected:
		Math::Vector2	mSticks[NUM_OF_STICKS];			/// max number of supported "sticks", each vector: length € [0.0f, 1.0f], each coordinate € [-1.0f, 1.0f]
		Real			mZAxis;							/// current value of the z-Axis, value € [-1.0f, 1.0f]

		uint32			mButtonStates;					/// states of the buttons (last update) - down or up
		uint32			mPreviousButtonStates;			/// states of the buttons (second last update) - down or up
		bool			mConnected;						/// true when this Xbox 360 device is connected
		// todo move this
		bool			mInUse;							/// true when this game controller device is used by a player
		bool			mInvertYAxes;					/// y-axis value of sticks is inverted if this is set to true
	};
}

#endif // _GAME_CONTROLLER_H_

