/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Audio.h"
#include "AudioManager.h"
#include "Event.h"

using namespace Audio;
using namespace std;

Event::Event(const string &name)
{
	FMOD::EventSystem &system = AudioManager::getSingleton().getEventSystem();
	errorCheck(system.getEvent(name.c_str(), FMOD_EVENT_DEFAULT, &mEvent));
}

bool Event::isPlaying() const
{
	FMOD_EVENT_STATE state;
	errorCheck(mEvent->getState(&state));
	return FMOD_EVENT_STATE_PLAYING == (state & FMOD_EVENT_STATE_PLAYING);
}

void Event::setMute(bool mute)
{
	errorCheck(mEvent->setMute(mute));
}

void Event::setVolume(Real volume)
{
	assert(volume >= 0.0f && volume <= 1.0f);
	errorCheck(mEvent->setVolume(volume));
}

void Event::start()
{
	errorCheck(mEvent->start());
}