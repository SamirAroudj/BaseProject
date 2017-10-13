/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _LIGHT_TEX_MATERIAL_H_
#define _LIGHT_TEX_MATERIAL_H_

#include "LightTexShader.h"
#include "Material.h"
#include "Graphics/Texture.h"

namespace Graphics
{
	class LightTexMaterial : public Material
	{
	friend class Material;

	public:
		virtual void prepareShaderForRendering(const Math::Matrix4x4 &worldViewProjection, const Math::Matrix4x4 &world);

	private:
		LightTexMaterial(const std::string &name, const std::string &shaderName, const std::string &diffuseTextureName, Real hardness);
		virtual ~LightTexMaterial();

		Texture *mDiffTexture;
		Real mHardness;

		// forbidden
		LightTexMaterial(const LightTexMaterial &copy) : Material(copy) { assert(false); }
		LightTexMaterial &operator =(const LightTexMaterial &rhs) { assert(false); return *this; }
	};
}

#endif // _LIGHT_TEX_MATERIAL_H_
