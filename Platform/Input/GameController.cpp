/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#include <cstring>
#include "GameController.h"

using namespace Input;
using namespace Math;
using namespace std;

GameController::GameController() : mConnected(false), mInUse(false), mInvertYAxes(false)
{
	reset();
}

GameController::Stick GameController::convertToStick(const string &stickName)
{
	if (0 == stickName.compare("LEFT_STICK"))
		return LEFT_STICK;
	if (0 == stickName.compare("RIGHT_STICK"))
		return RIGHT_STICK;
	if (0 == stickName.compare("POV_STICK"))
		return POV_STICK;
	/*if (stickName == "NO_STICK")
		return NO_STICK;
	if (stickName == "NUM_OF_STICKS")
		return NUM_OF_STICKS;*/

	return NO_STICK;
}

bool GameController::isButtonDown(uint32 buttonNumber) const
{
	assert(buttonNumber < CONTROLLER_BUTTON_COUNT);

	return 0 != (mButtonStates & (0x1 << buttonNumber));
}

bool GameController::isButtonPressed(uint32 buttonNumber) const
{
	assert(buttonNumber < CONTROLLER_BUTTON_COUNT);

	uint32 flag = 0x1 << buttonNumber;
	return (mButtonStates & flag) && !(mPreviousButtonStates & flag);
}

bool GameController::isButtonReleased(uint32 buttonNumber) const
{
	assert(buttonNumber < CONTROLLER_BUTTON_COUNT);

	uint32 flag = 0x1 << buttonNumber;
	return !(mButtonStates & flag) && (mPreviousButtonStates & flag);
}

bool GameController::isButtonUp(uint32 buttonNumber) const
{
	assert(buttonNumber < CONTROLLER_BUTTON_COUNT);

	return 0 == (mButtonStates & (0x1 << buttonNumber));
}

void GameController::reset()
{
	mButtonStates = mPreviousButtonStates = 0;		// reset buttons and axes
	mZAxis = 0.0f;
	for (uint32 i = 0; i < NUM_OF_STICKS; ++i)
		mSticks[i].x = mSticks[i].y = 0.0f;
}

void GameController::computeNormalizedStickPosition(int32 x, int32 y, uint32 deadZone, uint32 maximum,
													Real invertedRangeFactor, Vector2 &result)
{
	uint32 lengthSquared = x * x + y * y;

	if (lengthSquared <= deadZone * deadZone)
	{
		result.x = result.y = 0.0f;
		return;
	}

	Real length = sqrtr(static_cast<Real>(lengthSquared));
	Real oneLength = 1.0f / length;
	result.x = x * oneLength;
	result.y = y * oneLength;

	if (length > maximum)
		length = static_cast<Real>(maximum);

	length -= deadZone;

	length *= invertedRangeFactor;

	result.x *= length;
	result.y *= length;
}

void GameController::adaptStickYAxes()
{
	if (!mInvertYAxes)
		return;

	for (uint32 i = 0; i < NUM_OF_STICKS; ++i)
		mSticks[i].y *= -1.0f;
}