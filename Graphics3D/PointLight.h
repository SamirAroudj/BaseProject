/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "Light.h"

namespace Graphics
{
	class PointLight : public Light
	{
	public:
		PointLight(const Color &ambient, const Color &diffuse, const Color &specular,
			const Math::Vector3 &position, const Math::Vector3 &attenuationFactors, Real range);
		virtual ~PointLight();

		const Math::Vector3 &getAttenuationFactors() const { return mAttenuationFactors; }
		virtual LightData getData() const;
		const Math::Vector3 &getPosition() const { return mPosition; }
		Real getRange() const { return mRange; }

		void setAttenuationFactors(const Math::Vector3 &attenuationFactors);
		void setPosition(const Math::Vector3 &position) { mPosition = position; }
		void setRange(Real range) { assert(range > 0.0f); mRange = range; }

	private:
		Math::Vector3 mAttenuationFactors;
		Math::Vector3 mPosition;
		Real mRange;
	};
}

#endif _POINT_LIGHT_H_
