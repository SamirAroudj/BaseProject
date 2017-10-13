/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _GAME_CLIENT_MAGIC_CONSTANTS_H_
#define _GAME_CLIENT_MAGIC_CONSTANTS_H_

#include "Graphics3D/Color.h"
#include "Math/MathHelper.h"
#include "Network/MagicConstants.h"
#include "Platform/DataTypes.h"

namespace GamePlay
{
	/// <summary> maximum angular velocity of a tank in radians / second </summary>
	const Real TANK_MAX_ANGULAR_VELOCITY = Math::HALF_PI;

	/// <summary> angular acceleration of a tank in radians / second * second </summary>
	const Real TANK_ANGULAR_ACCELERATION = TANK_MAX_ANGULAR_VELOCITY * 1.5f;

	/// <summary> maximum velocity of a tank in units / second </summary>
	const Real TANK_MAX_VELOCITY = 5.0f;

	/// <summary> acceleration of a tank in units / second * second </summary>
	const Real TANK_ACCELERATION = TANK_MAX_VELOCITY / 3.0f;

	/// <summary> acceleration of the tank which is applied when braking </summary>
	const Real TANK_BRAKE_ACCELERATION = TANK_ACCELERATION * 2.0f;

	/// <summary> friction loss in percentage which is applied to the angular velocity of a tank</summary>
	const Real TANK_ANGULAR_FRICTION = 0.5f;

	/// <summary> Remote tank views should be updated if the position of the views differs more than this threshold from the actual position of the local tank.
	/// 		  Value is defined in units. </summary>
	const Real TANK_SEND_THRESHOLD_POSITION = 0.1f;

	/// <summary> Remote tank views should be updated if the angle of the views differs more than this threshold from the actual angle of the local tank.
	/// 		  Value is defined in radian measure. </summary>
	const Real TANK_SEND_THRESHOLD_ANGLE = Math::PI / 36.0f;

	/// <summary> Defines the time period over which the prediction of a tank is linearly converted into its newly updated prediction. </summary>
	const Real TANK_SMOOTHING_TIME = 1.0f;
}

namespace GUI
{
	extern const Graphics::Color TEXT_COLOR;
}

namespace Network
{
	const MessageType TANK_UPDATE_MESSAGE = MIN_USER_UDP_MESSAGE_IDENTIFIER;

	const uint32 LAN_ADVERTISER_PORT_NUMBER = 3722;
	const char PASSWORD[] = "0";
}

#endif // _GAME_CLIENT_MAGIC_CONSTANTS_H_
