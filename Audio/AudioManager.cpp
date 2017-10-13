/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "AudioManager.h"
#include "fmod.hpp"
#include "MagicAudioConstants.h"

using namespace Audio;
using namespace Math;
using namespace std;

AudioManager::AudioManager(const string &audioMediaPath) :
	mBackgroundMusic(NULL), mApplicationVolume(1.0), mMuted(false)
{
	FMOD::System *fmodSystem;	
	FMOD_SPEAKERMODE speakerMode;

	errorCheck(FMOD::EventSystem_Create(&mEventSystem));

	errorCheck(mEventSystem->getSystemObject(&fmodSystem));
    errorCheck(fmodSystem->getDriverCaps(0,0,0,&speakerMode));// set user's speaker mode to levarage 3D sound effects
    errorCheck(fmodSystem->setSpeakerMode(speakerMode));

	errorCheck(mEventSystem->init(CHANNELS_MAX_NUMBER, FMOD_INIT_NORMAL, 0, FMOD_EVENT_INIT_NORMAL));
	errorCheck(mEventSystem->setMediaPath(audioMediaPath.c_str()));
	errorCheck(mEventSystem->getMusicSystem(&mMusicSystem));
}

AudioManager::~AudioManager()
{
	errorCheck(mEventSystem->unload());
	errorCheck(mEventSystem->release());
}


void AudioManager::loadEventDefinitions(const string &fileName)
{
	// load from file - data is not already stored in memory
	errorCheck(mEventSystem->load(fileName.c_str(), NULL, NULL));
}

void AudioManager::setApplicationVolume(Real newAppVolume)
{
	assert(newAppVolume >= 0.0f && newAppVolume <= 1.0f);

	FMOD::EventCategory *category;
	int32 numOfCategories;
	Real categoryVolume;
	Real absoluteToRelativeFactor = 1.0f / mApplicationVolume;

	if (0.0f == newAppVolume)
	{
		setMuted(true);
		return;
	}
	else if (mMuted)
	{
		setMuted(false);
	}

	errorCheck(mEventSystem->getNumCategories(&numOfCategories));
	for(int32 i = 0; i < numOfCategories; ++i)
	{
		errorCheck(mEventSystem->getCategoryByIndex(i, &category));
		errorCheck(category->getVolume(&categoryVolume));
		errorCheck(category->setVolume(categoryVolume * absoluteToRelativeFactor * newAppVolume));
	}

	mApplicationVolume = newAppVolume;
}

void AudioManager::setBackgroundMusic(FMOD_MUSIC_CUE_ID cueID)
{
	if (mBackgroundMusic)
		errorCheck(mBackgroundMusic->release());
	errorCheck(mMusicSystem->prepareCue(cueID, &mBackgroundMusic));
}

void AudioManager::setListenerAttributes(int32 listener, const Vector3 &position, const Vector3 &velocity, const Vector3 &forward, const Vector3 &up)
{
	assert(EPSILON > forward.getLengthSquared() - 1.0f);
	assert(EPSILON > up.getLengthSquared() - 1.0f);
#ifdef _DEBUG
	int32 numOfListeners;
	mEventSystem->get3DNumListeners(&numOfListeners);
	assert(listener < numOfListeners);
#endif // _DEBUG
	errorCheck(mEventSystem->set3DListenerAttributes(listener, (FMOD_VECTOR *)&position, (FMOD_VECTOR *)&velocity,
		(FMOD_VECTOR *)&forward, (FMOD_VECTOR *)&up));
}

void AudioManager::setMuted(bool muted)
{
	FMOD::EventCategory *category;
	int32 numOfCategories;

	mMuted = muted;
	errorCheck(mEventSystem->getNumCategories(&numOfCategories));
	for(int32 i = 0; i < numOfCategories; ++i)
	{
		errorCheck(mEventSystem->getCategoryByIndex(i, &category));
		errorCheck(category->setMute(mMuted));
	}
}

void AudioManager::unloadData()
{
	errorCheck(mEventSystem->unload());
}

void AudioManager::update()
{
	errorCheck(mEventSystem->update());
}