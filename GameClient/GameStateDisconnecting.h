/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _GAME_STATE_DISCONNECTING_H_
#define _GAME_STATE_DISCONNECTING_H_

#include "GameState.h"

namespace GamePlay
{
	class GameStateDisconnecting : public GameState
	{
	public:
		GameStateDisconnecting(GameState *&previousGameState);
		virtual ~GameStateDisconnecting();

		const std::wstring &getDisconnectReason() const { return mDisconnectingReason; }

		virtual State getType() const { return DISCONNECTING; }

		virtual void postRender();
		virtual void render();

		virtual GameState::State update(Real deltaTime);

	private:
		std::wstring mDisconnectingReason;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copy constructor is forbidden. </summary>
		///
		/// <remarks>	Samir, 29.11.2011. </remarks>
		///
		/// <param name="copy">	Copy constructor is forbidden. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GameStateDisconnecting(const GameStateDisconnecting &copy) { assert(false); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assignment operator is forbidden. </summary>
		///
		/// <remarks>	Samir, 29.11.2011. </remarks>
		///
		/// <param name="rhs">	Assignment operator is forbidden. </param>
		///
		/// <returns>	Assignment operator is forbidden. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const GameStateDisconnecting &operator =(const GameStateDisconnecting &rhs) { assert(false); return *this; }
	};
}

#endif // _GAME_STATE_DISCONNECTING_H_