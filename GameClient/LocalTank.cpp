/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <iostream>
#include "Client.h"
#include "Platform/Input/InputManager.h"
#include "LocalTank.h"
#include "MagicGameConstants.h"

using namespace GamePlay;
using namespace Graphics;
using namespace Input;
using namespace Math;
using namespace Network;
using namespace Platform;
using namespace std;

LocalTank::LocalTank(uint16 identifier, const Vector2 &position, Real angle) :
	Tank(identifier, position, angle),
	mGraphics("Tank.mat", "Tank.mesh"),
	mRemoteView(identifier, position, angle),
	mDiscrepancy(false)
{

}

LocalTank::LocalTank(const LocalTank &copy) :
	Tank(copy),
	mGraphics(copy.mGraphics),
	mRemoteView(copy.mRemoteView),
	mDiscrepancy(copy.mDiscrepancy)
{

}

LocalTank::~LocalTank()
{

}


LocalTank &LocalTank::operator =(const LocalTank &rhs)
{
	this->Tank::operator=(rhs);
	mGraphics = rhs.mGraphics;
	mRemoteView = rhs.mRemoteView;
	mDiscrepancy = rhs.mDiscrepancy;
	return *this;
}
		
void LocalTank::render(const Matrix4x4 &viewProjection)
{
	Matrix4x4 scale;
	scale._11 = scale._22 = scale._33 = 0.005f;
	Matrix4x4 world = scale * Matrix4x4::createRotationY(mAngle);
	world.addTranslation(mPosition.x, 0.0f, mPosition.y);
	mGraphics.render(world * viewProjection, world);
}

void LocalTank::requestUpdate()
{
#ifdef _DEBUG
	cout << "Update was requested." << endl;
#endif // _DEBUG

	mDiscrepancy = true;
	mRemoteView = *this;
}

void LocalTank::update(Real deltaTime)
{
	Keyboard keyboard = InputManager::getSingleton().getKeyboard();

	if (keyboard.isKeyPressed(KEY_S)) // movement?
		startAcceleratingForwards();
	else if (keyboard.isKeyReleased(KEY_S))
		stopAcceleratingForwards();
	if (keyboard.isKeyPressed(KEY_W))
		startAcceleratingBackwards();
	else if (keyboard.isKeyReleased(KEY_W))
		stopAcceleratingBackwards();

	if (keyboard.isKeyPressed(KEY_A))	// turning?
		startTurningLeft();
	else if (keyboard.isKeyReleased(KEY_A))
		stopTurningLeft();
	if (keyboard.isKeyPressed(KEY_D))	
		startTurningRight();
	else if (keyboard.isKeyReleased(KEY_D))
		stopTurningRight();

	if (keyboard.isKeyPressed(KEY_SPACE))	// breaking?
		startBraking();
	else if (keyboard.isKeyReleased(KEY_SPACE))
		stopBraking();

	Tank::update(deltaTime);
	mRemoteView.update(deltaTime);

	if (!isSimilarToRemoteView())
	{
#ifdef _DEBUG
		cout << "Discrepency between local tank and tank's view. Update is necessary." << endl;
#endif // _DEBUG
		mRemoteView = *this;
		mDiscrepancy = true;
	}

	if (mDiscrepancy)// discrepancy is diminished if the update fits into the next udp packet
		mDiscrepancy = !Client::getSingleton().addUDPMessage(TANK_UPDATE_MESSAGE, this, 1, UDPPacket::LOSS_INFO, this); 
}
		
bool LocalTank::isSimilarToRemoteView()
{
	if (fabsr(getAngle() - mRemoteView.getAngle()) > TANK_SEND_THRESHOLD_ANGLE)
		return false;

	Real dX = getXCoordinate() - mRemoteView.getXCoordinate();
	Real dZ = getZCoordinate() - mRemoteView.getZCoordinate();
	Real squareDistance = dX * dX + dZ * dZ;
	if (squareDistance > TANK_SEND_THRESHOLD_POSITION * TANK_SEND_THRESHOLD_POSITION)
		return false;

	return true;
}
