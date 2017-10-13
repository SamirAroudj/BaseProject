/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Platform/ResourceManagement/UserResource.h"
#include "Shader.h"

namespace Graphics
{
	class Material : public ResourceManagement::UserResource<Material>
	{
	friend class ResourceManagement::UserResource<Material>;

	public:
		Shader &getShader() { return *mShader; }
		static Material *request(const std::string &name);
		virtual void prepareShaderForRendering(const Math::Matrix4x4 &worldViewProjection, const Math::Matrix4x4 &world) = 0;

	protected:
		Material(const std::string &name, Shader *shader);
		virtual ~Material();
		static Material *loadMaterial(const std::string &name);

		Shader *mShader;

		// forbidden
		Material(const Material &copy) : UserResource<Material>("")
		{
			assert(false);
		}

		// forbidden
		Material &operator =(const Material &rhs)
		{
			assert(false);
			return *this;
		}
	};
}

#endif // _MATERIAL_H_