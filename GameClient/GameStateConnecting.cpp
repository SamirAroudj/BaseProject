/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Client.h"
#include "FontManager.h"
#include "GameStateConnecting.h"
#include "GameStateSearching.h"
#include "MagicGameConstants.h"

using namespace ExceptionHandling;
using namespace GamePlay;
using namespace GUI;
using namespace Network;

GameStateConnecting::GameStateConnecting(GameState *&previousState) :
	mException(NULL)
{
	assert(SEARCHING == previousState->getType());
	GameStateSearching *searchingState = (GameStateSearching *) previousState;

	Client *client = new Client();
	client->connect(searchingState->getChosenServerApplication(), PASSWORD);

	delete previousState;
	previousState = NULL;
}

GameStateConnecting::~GameStateConnecting()
{
	delete mException;
}

void GameStateConnecting::postRender()
{

}

void GameStateConnecting::render()
{
	if (ClientState::CONNECTING == Client::getSingleton().getState())
		FontManager::getSingleton().getFont(0).renderText(L"Connecting to the chosen server.", 50, 50, TEXT_COLOR);
	else if (ClientState::CONNECTED == Client::getSingleton().getState())
		FontManager::getSingleton().getFont(0).renderText(L"Negotiating game session acceptence.", 50, 50, TEXT_COLOR);
}

GameState::State GameStateConnecting::update(Real deltaTime)
{
	try
	{
		Client::getSingleton().update();
	}
	catch(ConnectionCreationException &exception)
	{
		mException = exception;
	}

	switch(Client::getSingleton().getState())
	{
	case ClientState::DISCONNECTED:
		return GameState::DISCONNECTED;

	case ClientState::DISCONNECTING:
		return GameState::DISCONNECTING;

	case ClientState::CONNECTING:	// do nothing
	case ClientState::CONNECTED:
		break;

	case ClientState::READY_TO_USE:
		return GameState::COMMUNICATING;

	default:
		assert(false);
	}

	return GameState::NO_STATE_CHANGE;
}
