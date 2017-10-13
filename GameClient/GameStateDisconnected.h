/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _CLIENT_STATE_DISCONNECTED_H_
#define _CLIENT_STATE_DISCONNECTED_H_

#include <string>
#include "GameState.h"

namespace GamePlay
{
	class GameStateDisconnected : public GameState
	{
	public:
		GameStateDisconnected(GameState *&previousState);
		virtual ~GameStateDisconnected();

		virtual State getType() const { return DISCONNECTED; }

		virtual void postRender();
		virtual void render();

		virtual State update(Real deltaTime);

	private:
		std::wstring mDisconnectReason;
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copy constructor is forbidden. </summary>
		///
		/// <remarks>	Samir, 29.11.2011. </remarks>
		///
		/// <param name="copy">	Copy constructor is forbidden. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GameStateDisconnected(const GameStateDisconnected &copy) { assert(false); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assignment operator is forbidden. </summary>
		///
		/// <remarks>	Samir, 29.11.2011. </remarks>
		///
		/// <param name="rhs">	Assignment operator is forbidden. </param>
		///
		/// <returns>	Assignment operator is forbidden. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const GameStateDisconnected &operator =(const GameStateDisconnected &rhs) { assert(false); return *this; }
	};
}

#endif //_CLIENT_STATE_DISCONNECTED_H_
