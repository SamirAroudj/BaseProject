/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _GAME_STATE_SEARCHING_H_
#define _GAME_STATE_SEARCHING_H_

#include "GameState.h"
#include "LANServerFinder.h"
#include "Platform/TimePeriod.h"

namespace GamePlay
{
	class GameStateSearching : public GameState
	{
	public:
		GameStateSearching();
		virtual ~GameStateSearching();

		Network::ApplicationAddress getChosenServerApplication() const { return mLANServerFinder.getDiscoveredServers()[mChosenServerApplication]; }

		virtual State getType() const { return SEARCHING; }

		virtual void postRender();
		virtual void render();

		virtual GameState::State update(Real deltaTime);

	private:
		Network::LANServerFinder mLANServerFinder;
		uint32 mChosenServerApplication;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copy constructor is forbidden. </summary>
		///
		/// <remarks>	Samir, 29.11.2011. </remarks>
		///
		/// <param name="copy">	Copy constructor is forbidden. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GameStateSearching(const GameStateSearching &copy) : mLANServerFinder(0) { assert(false); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assignment operator is forbidden. </summary>
		///
		/// <remarks>	Samir, 29.11.2011. </remarks>
		///
		/// <param name="rhs">	Assignment operator is forbidden. </param>
		///
		/// <returns>	Assignment operator is forbidden. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const GameStateSearching &operator =(const GameStateSearching &rhs) { assert(false); return *this; }
	};
}

#endif // _GAME_STATE_SEARCHING_H_
