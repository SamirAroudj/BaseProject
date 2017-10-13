/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <sstream>
#include "FontManager.h"
#include "Game.h"
#include "GameStateSearching.h"
#include "Platform/Input/InputManager.h"
#include "MagicGameConstants.h"
#include "MagicNetworkConstants.h"

using namespace GamePlay;
using namespace GUI;
using namespace Input;
using namespace Network;
using namespace Platform;
using namespace std;

GameStateSearching::GameStateSearching() : 
	mLANServerFinder(LAN_ADVERTISER_PORT_NUMBER),
	mChosenServerApplication(0)
{

}

GameStateSearching::~GameStateSearching()
{

}

void GameStateSearching::postRender()
{

}

void GameStateSearching::render()
{
	wchar_t buffer[MAX_IP_ADDRESS_LENGTH];
	wostringstream os(wostringstream::out);
	uint32 numOfServerApplications = mLANServerFinder.getDiscoveredServers().size();

	os << L"Searching LAN servers...\n";
	if (numOfServerApplications > 0)
		os << L"LAN server to connect to: " << mChosenServerApplication;

	for(uint32 i = 0; i < numOfServerApplications; ++i)	// display found LAN servers
	{
		string IPAddress = mLANServerFinder.getDiscoveredServers()[i].getIPAddressAsString();
		mbstowcs_s(NULL, buffer, MAX_IP_ADDRESS_LENGTH, IPAddress.c_str(), MAX_IP_ADDRESS_LENGTH);
		os << L"\nServer " << i << L": ";
		os << L" IP address: ";
		os << buffer;
		os << L" port number: ";
		os << mLANServerFinder.getDiscoveredServers()[i].getPortNumber();
	}
	FontManager::getSingleton().getFont(0).renderText(os.str().c_str(), 20, 100, TEXT_COLOR);
}

GameState::State GameStateSearching::update(Real deltaTime)
{
	mLANServerFinder.update();
	if (InputManager::getSingleton().getKeyboard().isKeyPressed(KEY_RETURN) &&!mLANServerFinder.getDiscoveredServers().empty())
		return GameState::CONNECTING;

	if (InputManager::getSingleton().getKeyboard().isKeyPressed(KEY_R))	// update LAN server search
	{
		mLANServerFinder.clearDiscoveredServers();
		mLANServerFinder.sendDiscoveryMessage();
	}
	
	if (mLANServerFinder.getDiscoveredServers().size() > 1)	// choose one of the found LAN servers
	{
		if (InputManager::getSingleton().getKeyboard().isKeyPressed(KEY_DOWN))
		{
			if (0 == mChosenServerApplication)
				mChosenServerApplication = mLANServerFinder.getDiscoveredServers().size() - 1;
			else
				--mChosenServerApplication;
		}
		if (InputManager::getSingleton().getKeyboard().isKeyPressed(KEY_UP))
			mChosenServerApplication = (1 + mChosenServerApplication) % mLANServerFinder.getDiscoveredServers().size();
	}

	if (InputManager::getSingleton().getKeyboard().isKeyPressed(KEY_ESCAPE))	// exit?
		Game::getSingleton().exit();

	return GameState::NO_STATE_CHANGE;
}
