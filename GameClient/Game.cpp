/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "FontManager.h"
#include "Game.h"
#include "GameStateCommunicating.h"
#include "GameStateConnecting.h"
#include "GameStateDisconnected.h"
#include "GameStateDisconnecting.h"
#include "GameStateSearching.h"
#include "GraphicsConfigurationLoader.h"
#include "Graphics/GraphicsManager.h"
#include "MagicGameConstants.h"
#include "Graphics/Texture.h"
#include "Platform/Window.h"

using namespace GamePlay;
using namespace Graphics;
using namespace GUI;
using namespace ResourceManagement;
using namespace Network;

Game::Game(HINSTANCE applicationHandle, const std::string &graphicsConfigFile) : 
	Application(applicationHandle, graphicsConfigFile)
{
	uint32 refreshRate = 60, multisamplingCount = 1, multisamplingQuality = 0;
	bool enableVSync = true;
	GraphicsConfigurationLoader::loadGraphicsConfiguration(refreshRate, multisamplingCount, multisamplingQuality, enableVSync, graphicsConfigFile);
	GraphicsManager *graphicsManager = new GraphicsManager(refreshRate, multisamplingCount, multisamplingQuality, enableVSync);
	FontManager *fontManager = new FontManager();

	Network::initializeNetwork();
	mState = new GameStateSearching();
}

Game::~Game()
{
	assert(UserResource<StaticMesh>::getNumOfResources() == 0);	// test whether everything was freed
	UserResource<StaticMesh>::freeMemory();
	assert(UserResource<VertexLayout>::getNumOfResources() == 0);
	UserResource<VertexLayout>::freeMemory();
	assert(UserResource<Shader>::getNumOfResources() == 0);
	UserResource<Shader>::freeMemory();
	assert(UserResource<Texture>::getNumOfResources() == 0);
	UserResource<Texture>::freeMemory();
	assert(UserResource<Material>::getNumOfResources() == 0);
	UserResource<Material>::freeMemory();

	delete mState;
	Network::shutDownNetwork();
	delete FontManager::getSingletonPointer();
	delete GraphicsManager::getSingletonPointer();
}

void Game::onActivation()
{
	Application::onActivation();
}

void Game::onDeactivation()
{
	Application::onDeactivation();
}

void Game::postRender()
{
	mState->postRender();
}

void Game::render()
{
	GraphicsManager::getSingleton().clearBackAndDepthStencilBuffer();
	GraphicsManager::getSingleton().clearOutputMergerStates();
	mState->render();
	FontManager::getSingleton().getFont(0).renderText(mFrameStatistics.getStatistics(), 20, 5, TEXT_COLOR);
	GraphicsManager::getSingleton().presentBackBuffer();
}

void Game::update(Real deltaTime)
{
	switch(mState->update(deltaTime))
	{
	case GameState::NO_STATE_CHANGE:
		break;

	case GameState::CONNECTING:
		mState = new GameStateConnecting(mState); // no memory leak since GameStateConnecting frees memory
		break;

	case GameState::COMMUNICATING:
		mState = new GameStateCommunicating(mState); // no memory leak since GameSateCommunicating frees memory
		break;

	case GameState::DISCONNECTING:
		mState = new GameStateDisconnecting(mState); // no memory leak
		break;

	case GameState::DISCONNECTED:
		mState = new GameStateDisconnected(mState); // no memory leak
		break;

	case GameState::SEARCHING:
		delete mState;
		mState = new GameStateSearching();
		break;

	default:
		assert(false);
	}

	if (!Platform::Window::getSingleton().isActive())
		return;
	mFrameStatistics.update();
}