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
#include "GameStateCommunicating.h"
#include "GameStateConnecting.h"
#include "GameStateDisconnecting.h"
#include "MagicGameConstants.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"

using namespace ExceptionHandling;
using namespace GamePlay;
using namespace GUI;
using namespace Network;
using namespace std;

GameStateDisconnecting::GameStateDisconnecting(GameState *&previousGameState) :
	mDisconnectingReason(L"")
{
	if (CONNECTING == previousGameState->getType())
	{
		GameStateConnecting *connectingState = (GameStateConnecting *) previousGameState;
		wostringstream os(wostringstream::out);

		switch(connectingState->getException().getType())
		{
		case ConnectionCreationException::SESSION_IS_FULL:
			os  << L"Cannot connect since game session is full.\n";
			break;

		case ConnectionCreationException::WRONG_PASSWORD:
			os << L"Cannot connect since password is wrong.\n";
			break;

		default:
			assert(false);
		}
		mDisconnectingReason = os.str().c_str();
	}

	if (COMMUNICATING == previousGameState->getType())
		mDisconnectingReason = ((GameStateCommunicating *) previousGameState)->getDisconnectReason();

	delete previousGameState;
	previousGameState = NULL;
}

GameStateDisconnecting::~GameStateDisconnecting()
{
	delete Client::getSingletonPointer();
}

void GameStateDisconnecting::postRender()
{

}

void GameStateDisconnecting::render()
{
	FontManager::getSingleton().getFont(0).renderText(L"Disconnecting from server.", 50, 20, TEXT_COLOR);
	if (mDisconnectingReason.length() > 0)
		FontManager::getSingleton().getFont(0).renderText(mDisconnectingReason, 20, 100, TEXT_COLOR);
}

GameState::State GameStateDisconnecting::update(Real deltaTime)
{
	try
	{
		Client::getSingleton().update();
	}
	catch(NetworkException &exception)
	{
		// todo log this
	}

	if (ClientState::DISCONNECTED == Client::getSingleton().getState())
		return GameState::DISCONNECTED;

	return GameState::NO_STATE_CHANGE;
}
