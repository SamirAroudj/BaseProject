/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include <string>
#include "Audio.h"
#include "Platform/DataTypes.h"
#include "fmod_event.hpp"
#include "Patterns/Singleton.h"
#include "Math/Vector3.h"

/// Contains all functionality that is used to manage and play audio files in a specific way.
namespace Audio
{
	/// Unique and globally accessible object that manages the whole audio system.
	/** Manages the event system, the music system and the application volume. */
	class AudioManager : public Patterns::Singleton<AudioManager>
	{
	public:
		AudioManager(const std::string &audioMediaPath);
		virtual ~AudioManager();

		FMOD::EventSystem &getEventSystem() const { return *mEventSystem; }

		Real getApplicationVolume() const { return mMuted ? 0.0f : mApplicationVolume; }
		bool isMuted() const { return mMuted; }

		void loadEventDefinitions(const std::string &fileName);

		// volume must be in the interval [0.0f, 1.0f]
		// a value of 0.0f mutes all event categories
		void setApplicationVolume(Real volume);
		void setBackgroundMusic(FMOD_MUSIC_CUE_ID cueID);
		// forward and up must be unit vectors
		void setListenerAttributes(int32 listener, const Math::Vector3 &position, const Math::Vector3 &velocity,
			const Math::Vector3 &forward, const Math::Vector3 &up);
		void setMuted(bool mute);
		void setNumOfListeners(int32 numOfListeners) { errorCheck(mEventSystem->set3DNumListeners(numOfListeners)); }
		void startBackgroundMusic() { mBackgroundMusic->begin(); }
		void stopBackgroundMusic() { mBackgroundMusic->end(); }

		void unloadData();

		/** Updates the whole audio system, e.g. event system is updated. */
		void update();

	private:
		FMOD::EventSystem	*mEventSystem;
		FMOD::MusicSystem	*mMusicSystem;
		FMOD::MusicPrompt	*mBackgroundMusic;
		Real				mApplicationVolume;
		bool				mMuted;
	};
}

#endif // _AUDIO_MANAGER_H_