/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Camera.h"

class Player
{
public:
	Player(void);
	~Player(void);

	Graphics::Camera &getCamera() { return mCamera; }

	void update(Real deltaTime);

private:
	static const Real CAMERA_ANGULAR_VELOCITY;
	static const Real CAMERA_VELOCITY;

	Graphics::Camera mCamera;
};

#endif // _PLAYER_H_
