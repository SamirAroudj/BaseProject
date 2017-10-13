/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#include "Light.h"

namespace Graphics
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(const Color &ambient, const Color &diffuse, const Color &specular, const Math::Vector3 &direction);
		virtual ~DirectionalLight() { }

		virtual LightData getData() const;
		void setDirection(const Math::Vector3 &direction);

		const Math::Vector3 &getDirection() const { return mDirection; }

	private:
		Math::Vector3 mDirection;

	};
}

#endif // _DIRECTIONAL_LIGHT_H_
