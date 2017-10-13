/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _COLOR_MATERIAL_H_
#define _COLOR_MATERIAL_H_

#include "Material.h"

namespace Graphics
{
	class ColorMaterial : public Material
	{
	friend class Material;

	public:
		virtual void prepareShaderForRendering(const Math::Matrix4x4 &worldViewProjection, const Math::Matrix4x4 &world);

	private:
		ColorMaterial(const std::string &name, const std::string &shaderName);
		virtual ~ColorMaterial();

		// forbidden
		ColorMaterial(const ColorMaterial &copy) : Material(copy) { assert(false); }
		// forbidden
		ColorMaterial &operator =(const ColorMaterial &rhs) { assert(false); return *this; }
	};
}

#endif // _COLOR_MATERIAL_H_