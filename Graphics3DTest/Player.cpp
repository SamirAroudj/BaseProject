/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "AudioManager.h"
#include "Platform/Input/InputManager.h"
#include "Player.h"
#include "Platform/Window.h"

using namespace Audio;
using namespace Graphics;
using namespace Input;
using namespace Math;
using namespace Platform;

const Real Player::CAMERA_ANGULAR_VELOCITY = HALF_PI;
const Real Player::CAMERA_VELOCITY = 10.0f;

Player::Player()
{
	mCamera.setAsActiveCamera();
	mCamera.lookAtLH(Vector4(5.0f, 5.0f, 5.0f, 1.0f), Vector4(-2.0f, 5.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
	mCamera.setAspectRatio(static_cast<Real>(Window::getSingleton().getWidth()) / (static_cast<Real>(Window::getSingleton().getHeight()) * 0.5f));
}

Player::~Player()
{
}


void Player::update(Real deltaTime)
{
	Vector3 forward(0.0f, 0.0f, 1.0f), up(0.0f, 1.0f, 0.0f);
	Real mouseXMotion = InputManager::getSingleton().getMouse().getRelativeXMotion();
	Real mouseYMotion = InputManager::getSingleton().getMouse().getRelativeYMotion();
	Real cameraMaxDeltaAngle = deltaTime * CAMERA_ANGULAR_VELOCITY;
	Real cameraMovementDistance = deltaTime * CAMERA_VELOCITY;

	// camera movement
	if (InputManager::getSingleton().getKeyboard().isKeyDown(KEY_F)) 
		mCamera.moveUpwards(-cameraMovementDistance);
	if (InputManager::getSingleton().getKeyboard().isKeyDown(KEY_R))
		mCamera.moveUpwards(cameraMovementDistance);
	if (InputManager::getSingleton().getKeyboard().isKeyDown(KEY_A)) 
		mCamera.moveSidewards(-cameraMovementDistance);
	if (InputManager::getSingleton().getKeyboard().isKeyDown(KEY_D))
		mCamera.moveSidewards(cameraMovementDistance);
	if (InputManager::getSingleton().getKeyboard().isKeyDown(KEY_W))	
		mCamera.moveForwards(cameraMovementDistance);
	if (InputManager::getSingleton().getKeyboard().isKeyDown(KEY_S))
		mCamera.moveForwards(-cameraMovementDistance);

	// rotate camera
	mCamera.yaw(mouseXMotion, true);
	mCamera.pitch(-mouseYMotion);
	if (InputManager::getSingleton().getKeyboard().isKeyDown(KEY_Q))	// roll
		mCamera.roll(1.0f * cameraMaxDeltaAngle);
	if (InputManager::getSingleton().getKeyboard().isKeyDown(KEY_E))
		mCamera.roll(-1.0f * cameraMaxDeltaAngle);

	// update audio listener
	const Vector4 &position = mCamera.getPosition();
	const Quaternion &orientation = mCamera.getOrientation();
	orientation.rotateVector(forward, forward);
	orientation.rotateVector(up, up);
	AudioManager::getSingleton().setListenerAttributes(0, Vector3(position.x, position.y, position.z), Vector3(), forward, up);
}