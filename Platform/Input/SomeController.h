/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _SOME_CONTROLLER_H_
#define _SOME_CONTROLLER_H_

#ifdef _WINDOWS
	#include "dinput.h"
#endif // _WINDOWS
#include "GameController.h"

namespace Input
{
	#ifdef _WINDOWS
		/// Describes a game controller: how many buttons, what axes etc.
		struct GameControllerDescription
		{
			GameControllerDescription() : mNumOfButtons(0)
			{
				for (uint32 i = 0; i < CONTROLLER_RANGED_AXES; ++i)
					mAxisIDs[i] = 0;
				for (uint32 i = 0; i < CONTROLLER_AXES; ++i)
					mSupportedAxes[i] = false;
			}

			int32	mNumOfButtons;						/// stores number of available buttons - maximum 32
			int32	mAxisIDs[CONTROLLER_RANGED_AXES];	/// stores the IDs for each axis with a range property: x, y, z, rx, ry
			bool	mSupportedAxes[CONTROLLER_AXES];	/// axes: x, y, z, rx, ry, POV x, POV y - true when available
		};
	#endif // _WINDOWS

	/// Contains the data that is necessary for analoge axis (or stick) normalization so that axis values are mapped to [-1.0f, 1.0f].
	struct AxisDescription
	{
		/** Sets everything to zero. */
		AxisDescription() : mCenter(0), mDeadZone(0), mMaxMagnitude(0), mInvertedRangeFactor(0)
		{
		}

		/** Checks whether both axis descriptions are exactly equal. 
		@param rhs All values of rhs must be exactly equal to the values of this object to return true.
		@return True is only returned if both objects are exactly equal. */
		bool operator ==(const AxisDescription &rhs)
		{
			return mCenter == rhs.mCenter && 
				   mDeadZone == rhs.mDeadZone &&
				   mMaxMagnitude == rhs.mMaxMagnitude &&
				   mInvertedRangeFactor == rhs.mInvertedRangeFactor;
		}

		int32 mCenter;				/// = minValue + mMaxMagnitude -> center values around zero
		int32 mDeadZone;			/// = quieried deadZone (percentage) * mAxisMaxMagnitudes -> remove erroneous input data when axis is physically centered
		int32 mMaxMagnitude;		/// = (maxRange - minRange) / 2;
		Real mInvertedRangeFactor;	/// = 1.0f / (mMaxMagnitude - mDeadZone)  -> normalize to [-1.0f, 1.0f]
	};

	/// Represents each game controller device that is not an Xbox 360 controller
	class SomeController : public GameController
	{
	public:
		#ifdef _WINDOWS
			/** Creates representation of a game controller.
			@param gameController A representation for this device is created. */
			SomeController(LPDIRECTINPUTDEVICE8 gameController);
		#endif // _WINDOWS

		/** Releases the game controller and frees resources. */
		virtual ~SomeController();		
		
		/** Returns the number of controller "sticks" that are supported.
			A stick is essentially a coupling of an x-axis with a y-axis.
			Examples: four digital direction buttons, thumb stick of a Xbox 360 controller, point-of-view controller etc. 
		@return Returns the number of controller sticks a controller supports. The returned value is always smaller than NUM_OF_STICKS. */
		virtual uint32 getNumberOfSupportedSticks() const;

		#ifdef _WINDOWS
			/** Queries the GUID of the product. It is the same for all controllers of the same product type.
			@param guid It is filled with the GUID that identifies the product type of the game controller. */
			void getProductTypeGUID(GUID &guid) const;
		#endif // _WINDOWS

		/** Queries the type of the game controller, see GameController.h for supported types.
		@return SOME_CONTROLLER is returned. Returns the actual (most specialized) type of the game controller. */
		virtual ControllerType getType() const { return SOME_CONTROLLER; }

		/** Queries whether the game controller device supports the stick idntified by number.
		@param number Up to NUM_OF_STICKS sticks are supported. Therefore number must be smaller than NUM_OF_STICKS. 
		@return True is returned when the stick identified by number returns other values than Vector(0.0f, 0.0f) as the device has this stick. */
		virtual bool isStickSupported(Stick number) const
		{
			assert(number < NUM_OF_STICKS);
			return mStickSupport[number];
		}

		/** Queries whether the game controller has a z-Axis.
		@return True is returned when z-Axis values are set. Otherwise z-Axis values are always 0.0f. */
		virtual bool isZAxisSupported() const { return mZAxisSupport; }

		#ifdef _WINDOWS
			/** Reacquire game controller device the control of which is lost when another application is set to the foreground. */
			void reacquire();
		#endif // _WINDOWS

		/** Updates the state of this game controller. Should be called every frame.
		@return Returns true when the connection state of the controller changed. */
		virtual bool update();

	private:
        /** Copy constructor is forbidden.
        @param copy Copy constructor is forbidden. */
        SomeController(SomeController &copy) { assert(false); }

//        /** Operator is forbidden.
//        @param rhs Operator is forbidden. */
//        SomeController &operator =(const SomeController &rhs) { assert(false); return this; }

		#ifdef _WINDOWS
			/** Is called for every device object of a controller (button, axis etc.) that was found.
			@param deviceObject This structure describes the device object (button, axis etc.) that was found.
			@param gameController This is a pointer to the game cntroller object the function is called for.
			@return Returns either DIENUM_CONTINUE to continue the enumeration or DIENUM_STOP to stop the enumeration. */
			static BOOL CALLBACK onFoundDevice(LPCDIDEVICEOBJECTINSTANCE deviceObject, LPVOID gameController);

			/** Queries dead zone size and min and max values and computes axis descriptions which are necessary for normalization. 
			@param controllerDescription Describes the game controller, e.g. what axes are supported.
			@param axisDescriptions These are filled with the data that is necessary to normalize axes' input data. */
			void queryAxisDescriptions(GameControllerDescription &controllerDescription, AxisDescription *axisDescriptions);

			/** Queries game controller properties and sets support data as well as axis descriptions for supported sticks and the z-axis if it is supported.  
			@param controllerDescription Describes the game controller, e.g. enumerates the axes a controller has.*/
			void setSticksAndZAxis(GameControllerDescription &controllerDescription);

			/** Computes normalized values for z-axis, left and right stick. */
			void updateAxes(DIJOYSTATE &state);

			/** Updates the POV stick so that the stick axes are normalized.
			@param pov Contains the angle of the POV controller. */
			void updatePOVStick(int32 pov);
        #endif // _WINDOWS

    private:
        #ifdef _WINDOWS
            LPDIRECTINPUTDEVICE8 mDevice;	/// The DirectInput representation of this game controller.
        #endif // _WINDOWS

		AxisDescription mProperties[NUM_OF_STICKS];	  /// left stick (lx desc = ly desc), z-axis and right stick (rx = ry), used to normalize values to [-1.0f, 1.0f]
		bool			mStickSupport[NUM_OF_STICKS]; /// values depend on the axes the game controller has: 0: x, y, 1: rx, ry, 2: POVX, POVY
		bool			mZAxisSupport;				  /// true when controller has an z-Axis 
	};
}

#endif // _SOME_CONTROLLER_H_
