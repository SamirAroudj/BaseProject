/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _GAME_STATE_CONNECTING_H_
#define _GAME_STATE_CONNECTING_H_

#include "GameState.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"

namespace GamePlay
{
	class GameStateConnecting : public GameState
	{
	public:
		GameStateConnecting(GameState *&previousState);
		virtual ~GameStateConnecting();

		ExceptionHandling::ConnectionCreationException &getException() const { return *mException; }

		virtual State getType() const { return CONNECTING; }

		virtual void postRender();
		virtual void render();

		virtual GameState::State update(Real deltaTime);

	private:
		ExceptionHandling::ConnectionCreationException *mException;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copy constructor is forbidden. </summary>
		///
		/// <remarks>	Samir, 29.11.2011. </remarks>
		///
		/// <param name="copy">	Copy constructor is forbidden. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GameStateConnecting(const GameStateConnecting &copy) { assert(false); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assignment operator is forbidden. </summary>
		///
		/// <remarks>	Samir, 29.11.2011. </remarks>
		///
		/// <param name="rhs">	Assignment operator is forbidden. </param>
		///
		/// <returns>	Assignment operator is forbidden. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const GameStateConnecting &operator =(const GameStateConnecting &rhs) { assert(false); return *this; }
	};
}

#endif // _GAME_STATE_CONNECTING_H_
