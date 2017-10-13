/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _TEXTURE_MATERIAL_H_
#define _TEXTURE_MATERIAL_H_

#include "Material.h"
#include "Graphics/Texture.h"

namespace Graphics
{
	class DiffTexMaterial : public Material
	{
	friend class Material;

	public:
		virtual void prepareShaderForRendering(const Math::Matrix4x4 &worldViewProjection, const Math::Matrix4x4 &world);

	private:
		DiffTexMaterial(const std::string &name, const std::string &shaderName, const std::string &diffuseTextureName);
		virtual ~DiffTexMaterial();

		Texture *mTexture;

		// forbidden
		DiffTexMaterial(const DiffTexMaterial &copy) : Material(copy) { assert(false); }
		DiffTexMaterial &operator =(const DiffTexMaterial &rhs) { assert(false); return *this; }
	};
}

#endif // _TEXTURE_MATERIAL_H_
