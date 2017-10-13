/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _STATIC_GRAPHICS_OBJECT_H_
#define _STATIC_GRAPHICS_OBJECT_H_

#include "Graphics/IRenderable.h"
#include "Material.h"
#include "Shader.h"
#include "StaticMesh.h"

namespace Graphics
{
	class StaticObject : public IRenderable
	{
	public:
		StaticObject(const std::string &materialName, const std::string &staticMeshName);
		StaticObject(const std::string &materialName, const std::string &staticMeshName,
											   Real width, Real depth, uint32 numOfRowVertices, uint32 numOfColumnVertices,
											   Real (*heightFunction)(Real width, Real depth));
		StaticObject(const StaticObject &copy);
		virtual ~StaticObject();

		StaticObject &operator =(const StaticObject &rhs);

		virtual void render(const Math::Matrix4x4 &worldViewProjection, const Math::Matrix4x4 &world);

	private:
		Material *mMaterial;
		StaticMesh *mStaticMesh;
	};
}

#endif // _STATIC_GRAPHICS_OBJECT_H_
