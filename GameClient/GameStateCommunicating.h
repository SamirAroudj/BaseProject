/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _GAME_STATE_COMMUNICATING_H_
#define _GAME_STATE_COMMUNICATING_H_

#include <string>
#include <vector>
#include "Camera.h"
#include "DirectionalLight.h"
#include "GameState.h"
#include "ISessionListener.h"
#include "LocalTank.h"
#include "RemoteTank.h"

namespace GamePlay
{
	class GameStateCommunicating : public GameState, public Network::ISessionListener
	{
	public:
		GameStateCommunicating(GameState *&mPreviousState);
		virtual ~GameStateCommunicating();

		const std::wstring &getDisconnectReason() const { return mDisconnectReason; }
		virtual State getType() const { return COMMUNICATING; }

		void notifyNewMember(const Network::SessionMember &newMember);
		void notifyRemovedMember(uint16 identifier);

		virtual void postRender();
		virtual void render();

		virtual GameState::State update(Real deltaTime);

	private:
		void processReceivedPackets();
		void updateRemoteTank(Real latency);

		Graphics::Camera mCamera;
		Graphics::DirectionalLight mDirectionalLight;
		std::vector<RemoteTank *> mRemoteTanks;
		std::wstring mDisconnectReason;
		Graphics::StaticObject *mFloor;
		Graphics::StaticObject *mBlock;
		LocalTank *mTank;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copy constructor is forbidden. </summary>
		///
		/// <remarks>	Samir, 29.11.2011. </remarks>
		///
		/// <param name="copy">	Copy constructor is forbidden. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GameStateCommunicating(const GameStateCommunicating &copy) :
			mDirectionalLight(Graphics::WHITE, Graphics::WHITE, Graphics::WHITE, Math::Vector3()) 
		{ 
			assert(false); 
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copy constructor is forbidden. </summary>
		///
		/// <remarks>	Samir, 29.11.2011. </remarks>
		///
		/// <param name="rhs">	Copy constructor is forbidden. </param>
		///
		/// <returns>	Copy constructor is forbidden. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const GameStateCommunicating &operator =(const GameStateCommunicating &rhs) { assert(false); return *this; }
	};
}

#endif // _GAME_STATE_COMMUNICATING_H_
