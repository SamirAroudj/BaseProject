/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <sstream>
#include "Client.h"
#include "FontManager.h"
#include "Game.h"
#include "GameStateCommunicating.h"
#include "GameStateConnecting.h"
#include "GameStateDisconnected.h"
#include "GameStateDisconnecting.h"
#include "Platform/Input/InputManager.h"
#include "MagicGameConstants.h"
#include "Platform/ExceptionHandling/NetworkException.h"

using namespace ExceptionHandling;
using namespace GamePlay;
using namespace GUI;
using namespace Input;
using namespace Network;
using namespace std;

GameStateDisconnected::GameStateDisconnected(GameState *&previousState) :
	mDisconnectReason(L"")
{
	if (DISCONNECTING == previousState->getType())
		mDisconnectReason = ((GameStateDisconnecting *) previousState)->getDisconnectReason();

	if (COMMUNICATING == previousState->getType())
		mDisconnectReason = ((GameStateCommunicating *) previousState)->getDisconnectReason();

	
	if (CONNECTING == previousState->getType())
	{
		GameStateConnecting *connecting = (GameStateConnecting *) previousState;

		wostringstream os(ostringstream::out);
		switch(connecting->getException().getType())
		{
			case ConnectionCreationException::SOCKET_PROBLEM:
				os << L"Cannot connect since there is a problem with a network socket of the underlying network API.\n";
				break;

			case ConnectionCreationException::TIME_OUT:
				os << L"Cannot connect since the server does not answer.\n";
				break;

			default:
				assert(false);
		}
		mDisconnectReason = os.str().c_str();
	}

	delete previousState;
	previousState = NULL;
	if (Client::exists())
		delete Client::getSingletonPointer();
}

GameStateDisconnected::~GameStateDisconnected()
{

}

void GameStateDisconnected::postRender()
{

}

void GameStateDisconnected::render()
{
	FontManager::getSingleton().getFont(0).renderText(L"Disconnected from server.", 20, 50, TEXT_COLOR);
	if (mDisconnectReason.length() > 0)
		FontManager::getSingleton().getFont(0).renderText(mDisconnectReason, 20, 100, TEXT_COLOR);
}

GameState::State GameStateDisconnected::update(Real deltaTime)
{
	if (InputManager::getSingleton().getKeyboard().isKeyPressed(KEY_S))
		return GameState::SEARCHING;

	if (InputManager::getSingleton().getKeyboard().isKeyPressed(KEY_ESCAPE))
		Game::getSingleton().exit();

	return GameState::NO_STATE_CHANGE;
}
