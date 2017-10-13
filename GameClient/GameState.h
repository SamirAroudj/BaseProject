/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include <cassert>

namespace GamePlay
{
	class GameState
	{
	public:
		enum State { NO_STATE_CHANGE, SEARCHING, CONNECTING, COMMUNICATING, DISCONNECTING, DISCONNECTED, NUM_OF };

		GameState() { }
		virtual ~GameState() { }

		virtual State getType() const = 0;

		virtual void postRender() = 0;
		virtual void render() = 0;

		virtual State update(Real deltaTime) = 0;
	private:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copy constructor is forbidden. </summary>
		///
		/// <remarks>	Samir, 29.11.2011. </remarks>
		///
		/// <param name="copy">	Copy constructor is forbidden. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GameState(const GameState &copy) { assert(false); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assignment operator is forbidden. </summary>
		///
		/// <remarks>	Samir, 29.11.2011. </remarks>
		///
		/// <param name="rhs">	Assignment operator is forbidden. </param>
		///
		/// <returns>	Assignment operator is forbidden. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const GameState &operator =(const GameState &rhs) { assert(false); return *this; }
	};
}
#endif // _GAME_STATE_H_