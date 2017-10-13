/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "MagicGameConstants.h"
#include "Math/MathHelper.h"
#include "Tank.h"

using namespace GamePlay;
using namespace Graphics;
using namespace Math;
using namespace std;

Tank::Tank(uint16 identifier, const Math::Vector2 &position, Real angle) :
	mPosition(position),
	mAcceleration(0.0f),
	mAngle(angle),
	mAngularAcceleration(0.0f),
	mAngularVelocity(0.0f),
	mVelocity(0.0f),
	mIdentifier(identifier),
	mBraking(false)
{
}

Tank::Tank(const Tank &copy) :
	mPosition(copy.mPosition),
	mAcceleration(copy.mAcceleration),
	mAngle(copy.mAngle),
	mAngularAcceleration(copy.mAngularAcceleration),
	mAngularVelocity(copy.mAngularVelocity),
	mVelocity(copy.mVelocity),
	mIdentifier(copy.mIdentifier),
	mBraking(copy.mBraking)
{

}

Tank &Tank::operator =(const Tank &rhs)
{
	if (this == &rhs)
		return *this;

	mPosition = rhs.mPosition;
	mAcceleration = rhs.mAcceleration;
	mAngle = rhs.mAngle;
	mAngularAcceleration = rhs.mAngularAcceleration;
	mAngularVelocity = rhs.mAngularVelocity;
	mVelocity = rhs.mVelocity;
	mIdentifier = rhs.mIdentifier;
	mBraking = rhs.mBraking;

	return *this;
}
		
void Tank::deserialize(Patterns::IDeserializer &deserializer)
{
	mPosition.x = deserializer.readFloat();
	mPosition.y = deserializer.readFloat();
	mAcceleration = deserializer.readFloat();
	mAngle = deserializer.readFloat();
	mAngularAcceleration = deserializer.readFloat();
	mAngularVelocity = deserializer.readFloat();
	mVelocity = deserializer.readFloat();
	mIdentifier = deserializer.readUInt16();
	mBraking = static_cast<bool>(deserializer.readChar());
}

uint32 Tank::getRequiredSerializationMemory() const
{
	return sizeof(mPosition) + sizeof(mAcceleration) + sizeof(mAngle) + sizeof(mAngularAcceleration) +
		sizeof(mAngularVelocity) + sizeof(mVelocity) + sizeof(mIdentifier) + sizeof(mBraking);
}

void Tank::serialize(Patterns::ISerializer &serializer) const
{
	serializer.write(mPosition.x);
	serializer.write(mPosition.y);
	serializer.write(mAcceleration);
	serializer.write(mAngle);
	serializer.write(mAngularAcceleration);
	serializer.write(mAngularVelocity);
	serializer.write(mVelocity);
	serializer.write(mIdentifier);
	serializer.write(static_cast<char>(mBraking));
}

void Tank::startAcceleratingBackwards()
{
	mAcceleration = -TANK_ACCELERATION;
}

void Tank::startAcceleratingForwards()
{
	mAcceleration = TANK_ACCELERATION;
}

void Tank::startBraking()
{
	mAcceleration = TANK_BRAKE_ACCELERATION;
	mBraking = true;
}

void Tank::startTurningLeft()
{
	mAngularAcceleration = -TANK_ANGULAR_ACCELERATION;
}

void Tank::startTurningRight()
{
	mAngularAcceleration = TANK_ANGULAR_ACCELERATION;
}

void Tank::stopAcceleratingBackwards()
{
	if (-TANK_ACCELERATION == mAcceleration)
		mAcceleration = 0.0f;
}

void Tank::stopAcceleratingForwards()
{
	if (TANK_ACCELERATION == mAcceleration)
		mAcceleration = 0.0f;
}

void Tank::stopBraking()
{
	if (mAcceleration == TANK_BRAKE_ACCELERATION)
		mAcceleration = 0.0f;
	mBraking = false;
}

void Tank::stopTurningLeft()
{
	if (mAngularAcceleration == -TANK_ANGULAR_ACCELERATION)
		mAngularAcceleration = 0.0f;
}

void Tank::stopTurningRight()
{
	if (mAngularAcceleration == TANK_ANGULAR_ACCELERATION)
		mAngularAcceleration = 0.0f;
}

void Tank::update(Real deltaTime)
{
	Vector2 direction(0.0f, 1.0f);
	direction.rotate(-mAngle);

	mPosition += direction * (mVelocity * deltaTime);
	mAngle = Math::normalizeAngle(mAngle + mAngularVelocity * deltaTime);
	
	if (mBraking)
	{
		Real velocityChange = deltaTime * mAcceleration;
		if (velocityChange > fabsr(mVelocity))
			mVelocity = 0.0f;
		else
			if (mVelocity > 0.0f)
				mVelocity -= velocityChange;
			else
				mVelocity += velocityChange;
	}
	else
	{
		mVelocity += deltaTime * mAcceleration;
	}

	if (fabsr(mAngularAcceleration) < Math::EPSILON)
		mAngularVelocity *= deltaTime * TANK_ANGULAR_FRICTION;
	else
		mAngularVelocity += deltaTime * mAngularAcceleration;

	mAngularVelocity = Math::clamp(mAngularVelocity, TANK_MAX_ANGULAR_VELOCITY, -TANK_MAX_ANGULAR_VELOCITY);
	mVelocity = Math::clamp(mVelocity, TANK_MAX_VELOCITY, -TANK_MAX_VELOCITY);
}
	
std::ostream &GamePlay::operator <<(std::ostream &os, const Tank &tank)
{
	os << "Tank:" <<
		"\nidentifier: " << tank.getIdentifier() <<
		"\nposition: " << tank.getXCoordinate() << "/" << tank.getZCoordinate() <<
		"\nangle: " << tank.getAngle() <<
		"\nangular velocity: " << tank.getAngularVelocity() <<
		"\nangular accleration: " << tank.getAngularAcceleration() <<
		"\nvelocity: " << tank.getVelocity() <<
		"\nacceleration: " << tank.getAcceleration() <<
		"\nTank is " << (tank.isBraking() ?  "braking." : "not braking.") << endl;

	return os;
}