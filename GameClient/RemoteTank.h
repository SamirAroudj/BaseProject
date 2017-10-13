/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _REMOTE_TANK_H_
#define _REMOTE_TANK_H_

#include "Platform/ApplicationTimer.h"
#include "MagicGameConstants.h"
#include "StaticObject.h"
#include "Tank.h"

namespace GamePlay
{
	class RemoteTank : public Tank
	{
	public:
		RemoteTank(const uint16 identifier, const Math::Vector2 &position, Real angle = 0.0f);
		RemoteTank(const RemoteTank &copy);
		virtual ~RemoteTank();

		RemoteTank &operator =(const RemoteTank &rhs);

		void processRemoteUpdate(const Tank &remoteUpdate, bool smooth = true);

		void render(const Math::Matrix4x4 &viewProjection);

		virtual void update(Real deltaTime);

	private:
		void convertToRemoteUpdate(Real deltaTime);

		Graphics::StaticObject mGraphics;
		Tank *mRemoteUpdate;
		Real mConversionFactor;
	};
}

#endif // _REMOTE_TANK_H_
