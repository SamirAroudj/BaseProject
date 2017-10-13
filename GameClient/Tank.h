/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _TANK_H_
#define _TANK_H_

#include <iostream>
#include "ISerializable.h"
#include "Math/Vector2.h"

namespace GamePlay
{
	class Tank : public Patterns::ISerializable
	{
	public:
		Tank(uint16 identifier, const Math::Vector2 &position = Math::Vector2(), Real angle = 0.0f);
		Tank(const Tank &copy);
		virtual ~Tank() { }

		Tank &operator =(const Tank &rhs);


		virtual void deserialize(Patterns::IDeserializer &deserializer);

		Real getAcceleration() const { return mAcceleration; }
		Real getAngle() const { return mAngle; }
		Real getAngularAcceleration() const { return mAngularAcceleration; }
		const uint16 getIdentifier() const { return mIdentifier; }
		Real getAngularVelocity() const { return mAngularVelocity; }
		virtual uint32 getRequiredSerializationMemory() const;
		Real getVelocity() const { return mVelocity; }
		Real getXCoordinate() const { return mPosition.x; }
		Real getZCoordinate() const { return mPosition.y; }
		bool isBraking() const { return mBraking; }

		virtual void serialize(Patterns::ISerializer &serializer) const;

		void startAcceleratingBackwards();
		void startAcceleratingForwards();
		void startBraking();
		void startTurningLeft();
		void startTurningRight();
		void stopAcceleratingBackwards();
		void stopAcceleratingForwards();
		void stopBraking();
		void stopTurningLeft();
		void stopTurningRight();

		virtual void update(Real deltaTime);

	protected:
		Math::Vector2 mPosition;
		Real mAcceleration;
		Real mAngle;
		Real mAngularAcceleration;
		Real mAngularVelocity;
		Real mVelocity;
		uint16 mIdentifier;
		bool mBraking;
	};

	std::ostream &operator <<(std::ostream &os, const Tank &tank);
}

#endif // _TANK_H_
