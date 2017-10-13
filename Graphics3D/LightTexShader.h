/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _LIGHT_TEX_SHADER_H_
#define _LIGHT_TEX_SHADER_H_

#include "Light.h"
#include "Shader.h"
#include "Graphics/Texture.h"
#include "Math/Vector4.h"

namespace Graphics
{
	class LightTexShader : public Shader
	{
	public:
		static void release(LightTexShader *&resource) { Shader::release((Shader *&) resource); }
		static LightTexShader *request(const std::string &name);

		void setCameraPosition(const Math::Vector4 &position) { mEyePosition->SetRawValue((void *)&position, 0, 3 * sizeof(Real)); }
		void setDiffuseTexture(Texture &texture) { mDiffTex->SetResource(texture.getShaderResourceView()); }
		void setLights(const std::vector<Light *> &lights);
		void setSurfaceHardness(Real hardness) { assert(hardness >= 1.0f); mSurfaceHardness->SetFloat(hardness); }
		void setWorldMatrix(const Math::Matrix4x4 &world) { mWorld->SetMatrix(const_cast<Real *>(world.getData())); }

	private:
		LightTexShader(const std::string &name);
		virtual ~LightTexShader();

		ID3D10EffectShaderResourceVariable *mDiffTex;
		ID3D10EffectScalarVariable *mSurfaceHardness;

		ID3D10EffectMatrixVariable *mWorld;
		ID3D10EffectVariable *mEyePosition;
		ID3D10EffectVariable *mLights;
		ID3D10EffectScalarVariable *mNumOfLights;
	};
}

#endif // _LIGHT_TEX_SHADER_H_