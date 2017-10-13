/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _GAME_H_
#define _GAME_H_

#include "Platform/Application.h"
#include "FrameStatistics.h"
#include "GameState.h"

/// Contains all the functionality that does not belong to the technical engine part.
namespace GamePlay
{
	class Game : public Platform::Application
	{
	public:
		Game(HINSTANCE applicationHandle, const std::string &graphicsConfigFile);
		virtual ~Game();

		virtual void onActivation();
		virtual void onDeactivation();

	protected:
		virtual void postRender();
		virtual void render();
		virtual void update(Real deltaTime);

		Graphics::FrameStatistics mFrameStatistics;
		GameState *mState;
	};
}

#endif _GAME_H_