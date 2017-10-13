/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cstdio>
#include "Platform/ApplicationTimer.h"
#include "Client.h"
#include "ClientTimeManager.h"
#include "FontManager.h"
#include "GameStateCommunicating.h"
#include "GameStateConnecting.h"
#include "Platform/Input/InputManager.h"
#include "MagicGameConstants.h"
#include "MagicGraphicsConstants.h"
#include "Platform/ExceptionHandling/NetworkExceptionFactory.h"
#include "Session.h"
#include "Graphics/Texture.h"

using namespace ExceptionHandling;
using namespace GamePlay;
using namespace Graphics;
using namespace GUI;
using namespace Input;
using namespace Math;
using namespace ResourceManagement;
using namespace Network;
using namespace std;

GameStateCommunicating::GameStateCommunicating(GameState *&mPreviousState) :
	mCamera(FOVY, Z_NEAR, Z_FAR), mDisconnectReason(L""),
	mDirectionalLight(Color(0.8f, 0.8f, 0.8f, 1.0f), Color(0.8f, 0.8f, 0.8f, 1.0f), Color(0.3f, 0.3f, 0.8f, 1.0f), Vector3(0.0f, -1.0f, 0.0f))
{
	delete mPreviousState;
	mPreviousState = NULL;

	mTank = new LocalTank(Session::getSingleton().getOwnIdentifier());
	mFloor = new StaticObject("WoodCrate.mat", "Crate");
	mBlock = new StaticObject("Color.mat", "Floor");

	Session::getSingleton().addListener(this);
	mCamera.setAsActiveCamera();
}

GameStateCommunicating::~GameStateCommunicating()
{
	uint32 numOfRemoteTanks = mRemoteTanks.size();
	for(uint32 i = 0; i < numOfRemoteTanks; ++i)
		delete mRemoteTanks[i];
	delete mBlock;
	delete mFloor;
	delete mTank;

	assert(UserResource<StaticMesh>::getNumOfResources() == 0);	// test whether everything was freed
	assert(UserResource<VertexLayout>::getNumOfResources() == 0);
	assert(UserResource<Shader>::getNumOfResources() == 0);
	assert(UserResource<Texture>::getNumOfResources() == 0);
	assert(UserResource<Material>::getNumOfResources() == 0);
}

void GameStateCommunicating::notifyNewMember(const SessionMember &newMember)
{
	if (newMember.getIdentifier() == Session::getSingleton().getOwnIdentifier() || newMember.getIdentifier() == 0)
		return;

	RemoteTank *remoteTank = new RemoteTank(newMember.getIdentifier(), Vector2(0.0f, 0.0f));
	mRemoteTanks.push_back(remoteTank);
	mTank->requestUpdate();
}

void GameStateCommunicating::notifyRemovedMember(uint16 identifier)
{
	uint32 numOfRemoteTanks = mRemoteTanks.size();
	for(uint32 i = 0; i < numOfRemoteTanks; ++i)
	{
		if (identifier == mRemoteTanks[i]->getIdentifier())
		{
			delete mRemoteTanks[i];
			mRemoteTanks[i] = mRemoteTanks.back();
			mRemoteTanks.pop_back();
			return;
		}
	}
}

void GameStateCommunicating::postRender()
{

}

void GameStateCommunicating::render()
{
	wchar_t buffer[100];
	const Matrix4x4 viewProjection(mCamera.getViewMatrix() * mCamera.getProjectionMatrix());

	Matrix4x4 translation;

	mFloor->render(translation * viewProjection, translation);

	translation.addTranslation(2.0f, 0.0f, 2.0f);
	mBlock->render(translation * viewProjection, translation);
	translation.addTranslation(-3.0f, 0.0f, -3.0f);
	mBlock->render(translation * viewProjection, translation);

	translation.addTranslation(1.0f, -0.5f, 1.0f);
	mTank->render(viewProjection);

	uint32 numOfRemoteTanks = mRemoteTanks.size();
	for(uint32 i = 0; i < numOfRemoteTanks; ++i)
		mRemoteTanks[i]->render(viewProjection);

	swprintf(buffer, 100, L"%u", static_cast<uint32>(0.5f + 1000.0f * ClientTimeManager::getSingleton().getNetworkTime()));
	FontManager::getSingleton().getFont(0).renderText(L"Connection to the chosen server was established.", 50, 50, TEXT_COLOR);
	FontManager::getSingleton().getFont(0).renderText(buffer, 50, 150, TEXT_COLOR);
}

GameState::State GameStateCommunicating::update(Real deltaTime)
{
	Vector4 cameraPosition;
	Vector4 tankPosition;
	try
	{
		Client::getSingleton().update();
		if (ClientState::READY_TO_USE == Client::getSingleton().getState())
		{
			processReceivedPackets();

			mTank->update(deltaTime);
			tankPosition.set(mTank->getXCoordinate(), 0.0f, mTank->getZCoordinate(), 1.0f);
			cameraPosition.set(tankPosition.x + 2.0f, tankPosition.y + 20.0f, tankPosition.z + 2.0f, 1.0f);
			mCamera.lookAtLH(cameraPosition, tankPosition, Vector3(0.0f, 1.0f, 0.0f));

			
			uint32 numOfRemoteTanks = mRemoteTanks.size();
			for(uint32 i = 0; i < numOfRemoteTanks; ++i)
				mRemoteTanks[i]->update(deltaTime);

			if (InputManager::getSingleton().getKeyboard().isKeyPressed(KEY_ESCAPE))
			{
				mDisconnectReason = L"User requested disconnect.";
				Client::getSingleton().disconnect();
			}
			else
				Client::getSingleton().send();
		}
	}
	catch(ExceptionHandling::ClosedConnectionException &exception)
	{
		mDisconnectReason = L"Server shut down.";
	}

	switch(Client::getSingleton().getState())
	{
	case ClientState::DISCONNECTING:
		return GameState::DISCONNECTING;

	case ClientState::DISCONNECTED:
		return GameState::DISCONNECTED;

	case ClientState::READY_TO_USE:
		break;

	default:
		assert(false);
	}

	return GameState::NO_STATE_CHANGE;
}

void GameStateCommunicating::processReceivedPackets()
{
	uint16 senderIdentifier = 0;
	MessageType message = Client::getSingleton().getCurrentUDPMessageIdentifier(senderIdentifier);

	switch(message)
	{
	case UDPPacket::NO_MESSAGE:
		return;

	case TANK_UPDATE_MESSAGE:
	{
		Real latency = ClientTimeManager::getSingleton().getNetworkTime() - Client::getSingleton().getUDPMessageNetworkTime();
		updateRemoteTank(latency);
		return;
	}

	default:
		assert(false);
	}
}
		
void GameStateCommunicating::updateRemoteTank(Real latency)
{
	Tank remoteUpdate(0);
	Client::getSingleton().getUDPMessage(&remoteUpdate, 1);
	if (remoteUpdate.getIdentifier() == Session::getSingleton().getOwnIdentifier())
		return;
	remoteUpdate.update(latency);

	uint32 numOfRemoteTanks = mRemoteTanks.size();
	for(uint32 i = 0; i < numOfRemoteTanks; ++i)
	{
		if (remoteUpdate.getIdentifier() == mRemoteTanks[i]->getIdentifier())
		{
			mRemoteTanks[i]->processRemoteUpdate(remoteUpdate);
			return;
		}
	}
	assert(false);
}
