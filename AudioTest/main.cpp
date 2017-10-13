/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <conio.h>
#include <windows.h>
#include "AudioManager.h"
#include "Platform/DataTypes.h"
#include "Event.h"
#include "MagicAudioConstants.h"
#include "Platform/ResourceManagement/ResourceManagement.h"

using namespace Audio;
using namespace Math;
using namespace ResourceManagement;

// Memory management
const uint32 ResourceManagement::DEFAULT_POOL_BUCKET_NUMBER = 5;
const uint16 ResourceManagement::DEFAULT_POOL_BUCKET_CAPACITIES[DEFAULT_POOL_BUCKET_NUMBER] = { 1024, 1024, 1024, 1024, 1024 };
const uint16 ResourceManagement::DEFAULT_POOL_BUCKET_GRANULARITIES[DEFAULT_POOL_BUCKET_NUMBER] = { 16, 32, 64, 128, 256 };

// minimum frame time in milliseconds
const int32 FRAME_TIME_MINIMUM = 15;
const Real VOLUME_CHANGE = 2.0f / (1000.0f / FRAME_TIME_MINIMUM);

// test 3D events
const Real EVENT_DISTANCE = 10.0f;
Real eventAngle = 0.0f;
Real eventAngularVelocity = Math::TWO_PI / 1000.0f;
Real eventVelocity = (Math::PI * 2 * EVENT_DISTANCE) * (1000.0f * eventAngularVelocity) / Math::TWO_PI;

int updateVolume(const int key);

int main(int argc, char **argv)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG
	Vector3 position, velocity, forward(0.0f, 0.0f, 1.0f), up(0.0f, 1.0f, 0.0f);
	AudioManager *manager = new AudioManager("Audio\\");
	manager->loadEventDefinitions("Test.fev");
	manager->setBackgroundMusic(MUSICCUE_TEST_DANCEOFTHESUGARPLUMFAIRY);
	manager->startBackgroundMusic();

	manager->setListenerAttributes(0, position, velocity, forward, up);
	Event airBlow("Test/TestGroup/AirBlow");
	airBlow.setVolume(0.1f);
	airBlow.start();

	int key = 0;
	while(true)
	{
		// udpate volume
		if (_kbhit())
		{
			key = updateVolume(_getch());
		}

		// update event
		eventAngle += FRAME_TIME_MINIMUM * eventAngularVelocity;
		if (eventAngle > Math::TWO_PI)
			eventAngle -= Math::TWO_PI;
		Vector3 position(EVENT_DISTANCE * cosr(eventAngle), 0.0f, EVENT_DISTANCE * sinr(eventAngle));
		Vector3 velocity(eventVelocity * -sinr(eventAngle), 0.0f, eventVelocity * cosr(eventAngle));
		airBlow.setPositionVelocity(position, velocity);
		//airBlow.setPosition(position);

		manager->update();
		if ('e' == key)
			break;

		Sleep(FRAME_TIME_MINIMUM);
	}

	delete manager;
	MemoryManager::shutDown();
	return 0;
}

int updateVolume(const int key)
{
	if ('u' != key && 'd' != key)
		return key;

	Real volume = AudioManager::getSingleton().getApplicationVolume();

	if ('u' == key)
	{
		volume += VOLUME_CHANGE;
		if (volume > 1.0f)
			volume = 1.0f;
	}

	if ('d' == key)
	{
		volume -= VOLUME_CHANGE;
		if (volume <= 0.0f)
			volume = 0.0f;
	}

	AudioManager::getSingleton().setApplicationVolume(volume);
	return key;
}