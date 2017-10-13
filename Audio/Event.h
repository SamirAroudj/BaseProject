/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _EVENT_H_
#define _EVENT_H_

#include <string>
#include "Audio.h"
#include "fmod_event.hpp"
#include "Math/Vector3.h"

namespace Audio
{
	class Event
	{
	public:
		Event(const std::string &name);

		bool isPlaying() const;

		void setMute(bool mute);
		void setPosition(const Math::Vector3 &position) { errorCheck(mEvent->set3DAttributes((FMOD_VECTOR *)&position, NULL)); }
		void setPositionVelocity(const Math::Vector3 &pos, const Math::Vector3 &vel)
		{ 
			errorCheck(mEvent->set3DAttributes((FMOD_VECTOR *)&pos, (FMOD_VECTOR *)&vel)); 
		}
		void setVelocity(const Math::Vector3 &velocity) { errorCheck(mEvent->set3DAttributes(NULL, (FMOD_VECTOR *)&velocity)); }
		void setVolume(Real volume);
		void start();

	private:
		FMOD::Event *mEvent;
	};
}

#endif // _EVENT_H_
