/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _LOCAL_TANK_H_
#define _LOCAL_TANK_H_

#include "IPacketLossListener.h"
#include "StaticObject.h"
#include "Tank.h"

namespace GamePlay
{
	class LocalTank : public Tank, public Network::IPacketLossListener
	{
	public:
		LocalTank(uint16 identifier, const Math::Vector2 &position = Math::Vector2(0.0f, 0.0f), Real angle = 0.0f);
		LocalTank(const LocalTank &copy);
		virtual ~LocalTank();

		LocalTank &operator =(const LocalTank &rhs);

		void notifyAboutPacketLoss() { mDiscrepancy = true; }
		void render(const Math::Matrix4x4 &viewProjection);
		void requestUpdate();

		virtual void update(Real deltaTime);

	private:
		bool isSimilarToRemoteView();

		Graphics::StaticObject mGraphics;
		Tank mRemoteView;
		bool mDiscrepancy;
	};
}

#endif // _LOCAL_TANK_H_
