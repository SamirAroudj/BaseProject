/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <vector>
#include "Color.h"
#include "Math/Vector3.h"

namespace Graphics
{
	struct LightData
	{
		enum Type { DIRECTIONAL, POINT, SPOT, NUM_OF, PADDING = 0xFFFFFFFF };

		LightData(LightData::Type type, const Color &ambient, const Color &diffuse, const Color &specular,
			const Math::Vector3 &position = Math::Vector3(), const Math::Vector3 &direction = Math::Vector3(),
			const Math::Vector3 &attenuationFactors = Math::Vector3(),
			Real range = 0.0f, Real spotLightFactor = 0.0f);

		Color mAmbient;
		Color mDiffuse;
		Color mSpecular;
		Math::Vector3 mPosition;
		Real mRange;
		Math::Vector3 mDirection;
		Real mSpotLightFactor;
		Math::Vector3 mAttenuationFactors;
		Type mType;
	};

	class Light
	{
	public:
		Light(const Color &ambient, const Color &diffuse, const Color &specular);
		virtual ~Light();


		static void freeMemory() { msLights.swap(std::vector<Light *>()); }
		const Color &getAmbient() const { return mAmbient; }
		virtual LightData getData() const = 0;
		const Color &getDiffuse() const { return mDiffuse; }
		static const std::vector<Light *> &getLights() { return msLights; }
		const Color &getSpecular() const { return mSpecular; }

		
		void setAmbient(const Color &ambient) { mAmbient = ambient; }
		void setDiffuse(const Color &diffuse) { mDiffuse = diffuse; }
		void setSpecular(const Color &specular) { mSpecular = specular; }

	protected:
		static std::vector<Light *> msLights;
		Color mAmbient;
		Color mDiffuse;
		Color mSpecular;
	};
}

#endif // _LIGHT_H_
