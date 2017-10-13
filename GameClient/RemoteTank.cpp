/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "ClientTimeManager.h"
#include "RemoteTank.h"

using namespace GamePlay;
using namespace Math;
using namespace Network;
using namespace Platform;
using namespace std;

RemoteTank::RemoteTank(const uint16 identifier, const Vector2 &position, Real angle) :
	Tank(identifier, position, angle), 
	mGraphics("Tank.mat", "Tank.mesh"),
	mRemoteUpdate(NULL),
	mConversionFactor(1.0f)
{
}

RemoteTank::RemoteTank(const RemoteTank &copy) :
	Tank(copy),
	mGraphics(copy.mGraphics),
	mRemoteUpdate(NULL),
	mConversionFactor(copy.mConversionFactor)
{
	if (copy.mRemoteUpdate)
		mRemoteUpdate = new Tank(*copy.mRemoteUpdate);
}

RemoteTank::~RemoteTank()
{
	delete mRemoteUpdate;
}

RemoteTank &RemoteTank::operator =(const RemoteTank &rhs)
{
	if (&rhs == this)
		return *this;

	Tank::operator=(rhs);
	mGraphics = rhs.mGraphics;
	mConversionFactor = rhs.mConversionFactor;

	if (rhs.mRemoteUpdate)
	{
		if (mRemoteUpdate)
			*mRemoteUpdate = *rhs.mRemoteUpdate;
		else
			mRemoteUpdate = new Tank(*rhs.mRemoteUpdate);
	}
	else
	{
		delete mRemoteUpdate;
		mRemoteUpdate = NULL;
	}

	return *this;
}

void RemoteTank::processRemoteUpdate(const Tank &remoteUpdate, bool smooth)
{
	assert(remoteUpdate.getIdentifier() == mIdentifier);
	if (!smooth)
	{
		Tank::operator=(remoteUpdate);
		delete mRemoteUpdate;
		mRemoteUpdate = NULL;
		return;
	}

	if (mRemoteUpdate)
		*mRemoteUpdate = remoteUpdate;
	else
		mRemoteUpdate = new Tank(remoteUpdate);
	mConversionFactor = 1.0f;
	mBraking = remoteUpdate.isBraking();
}

void RemoteTank::render(const Matrix4x4 &viewProjection)
{
	Matrix4x4 scale;
	scale._11 = scale._22 = scale._33 = 0.005f;
	Matrix4x4 world = scale * Matrix4x4::createRotationY(mAngle);
	world.addTranslation(mPosition.x, 0.0f, mPosition.y);
	mGraphics.render(world * viewProjection, world);
}

void RemoteTank::update(Real deltaTime)
{
	Tank::update(deltaTime);
	if (mRemoteUpdate)
	{
		mRemoteUpdate->update(deltaTime);
		convertToRemoteUpdate(deltaTime);
	}
}

void RemoteTank::convertToRemoteUpdate(Real deltaTime)
{
	assert(mRemoteUpdate);

	Real factor1 = deltaTime * (isBraking() ? 10.0f : 5.0f);
	Real factor2 = 1.0f - factor1;
	mPosition.x =			factor1 * mRemoteUpdate->getXCoordinate()			+ factor2 * mPosition.x;
	mPosition.y =			factor1 * mRemoteUpdate->getZCoordinate()			+ factor2 * mPosition.y;
	mAcceleration =			factor1 * mRemoteUpdate->getAcceleration()			+ factor2 * mAcceleration;
	mAngularAcceleration =	factor1 * mRemoteUpdate->getAngularAcceleration()	+ factor2 * mAngularAcceleration;
	mAngularVelocity =		factor1 * mRemoteUpdate->getAngularVelocity()		+ factor2 * mAngularVelocity;
	mVelocity =				factor1 * mRemoteUpdate->getVelocity()				+ factor2 * mVelocity;

	Real diff = mAngle - mRemoteUpdate->getAngle();	// take the shorter way, don't turn more than PI to align the simulations
	if (diff > PI)
		mAngle =			factor1 * mRemoteUpdate->getAngle()					+ factor2 * (mAngle - TWO_PI);
	else if (diff < -PI)
		mAngle =			factor1 * mRemoteUpdate->getAngle()					+ factor2 * (mAngle + TWO_PI);
	else
		mAngle =			factor1 * mRemoteUpdate->getAngle()					+ factor2 * mAngle;

	
	mConversionFactor *= factor2;
	if (mConversionFactor < 0.01f)
	{
		Tank::operator=(*mRemoteUpdate);
		delete mRemoteUpdate;
		mRemoteUpdate = NULL;
		return;
	}
}
