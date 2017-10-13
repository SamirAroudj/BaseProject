/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _SPOT_LIGHT_H_
#define _SPOT_LIGHT_H_

#include "Light.h"

namespace Graphics
{
	class SpotLight : public Light
	{
	public:
		// spotLightFactor is used to narrow the light cone with increasing value
		SpotLight(const Color &ambient, const Color &diffuse, const Color &specular,
			const Math::Vector3 &position, const Math::Vector3 &direction,
			const Math::Vector3 &attenuationFactors, Real range, Real lightConeFactor);
		virtual ~SpotLight() { }

		const Math::Vector3 &getAttenuationFactors() const { return mAttenuationFactors; }
		virtual LightData getData() const;
		const Math::Vector3 &getDirection() const { return mDirection; }
		Real getLightConeFactor() const { return mLightConeFactor; }
		const Math::Vector3 &getPosition() const { return mPosition; }
		Real getRange() const { return mRange; }

		void setAttenuationFactors(const Math::Vector3 &attenuationFactors);
		void setDirection(const Math::Vector3 &direction);
		void setLightConeFactor(Real lightConeFactor) { assert(lightConeFactor >= 1.0f); mLightConeFactor = lightConeFactor; }
		void setPosition(const Math::Vector3 &position) { mPosition = position; }
		void setRange(Real range) { assert(range > 0.0f); mRange = range; }

	private:
		Math::Vector3 mAttenuationFactors;
		Math::Vector3 mDirection;
		Math::Vector3 mPosition;
		Real mLightConeFactor;
		Real mRange;
	};
}

#endif // _SPOT_LIGHT_H_