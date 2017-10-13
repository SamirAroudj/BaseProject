/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _SHADER_H_
#define _SHADER_H_

#include <vector>
#include "d3dx10.h"
#include "StaticMesh.h"
#include "Math/Matrix4x4.h"
#include "Platform/ResourceManagement/UserResource.h"

namespace Graphics
{
	class Shader : public ResourceManagement::UserResource<Shader>
	{
	friend class ResourceManagement::UserResource<Shader>;

	public:
		ID3D10EffectTechnique *getCurrentTechnique() const { return mCurrentTechnique; }

		void render(const StaticMesh &staticMesh) const;

		static Shader *request(const std::string &shaderFile);
		void setWorldViewProjectionMatrix(const Math::Matrix4x4 &matrix);

	protected:
		Shader(const std::string &shaderFile);
		virtual ~Shader();

		ID3D10Effect *mEffect;
		ID3D10EffectTechnique *mCurrentTechnique;
		ID3D10EffectMatrixVariable *mWorldViewProjection;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copy constructor is forbidden. </summary>
		///
		/// <remarks>	Samir, 03.12.2011. </remarks>
		///
		/// <param name="copy">	Copy constructor is forbidden. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Shader(const Shader &copy) :
			ResourceManagement::UserResource<Shader>("")
		{
			assert(false);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assignment operator is forbidden. </summary>
		///
		/// <remarks>	Samir, 03.12.2011. </remarks>
		///
		/// <param name="rhs">	Assignment operator is forbidden. </param>
		///
		/// <returns>	Assignment operator is forbidden. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Shader &operator =(const Shader &rhs) { assert(false); return *this; }
	};
}

#endif // _SHADER_H_
