/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _TEXTURE_SHADER_H_
#define _TEXTURE_SHADER_H_

#include "d3dx10.h"
#include "Shader.h"
#include "Graphics/Texture.h"

namespace Graphics
{
	class TextureShader : public Shader
	{
	public:
		static void release(TextureShader *&resource) { Shader::release((Shader *&) resource); }
		static TextureShader *request(const std::string &name);

		void setDiffuseTexture(Texture &texture) { mDiffTex->SetResource(texture.getShaderResourceView()); }

	private:
		TextureShader(const std::string &name);
		virtual ~TextureShader();

		ID3D10EffectShaderResourceVariable *mDiffTex;
	};
}

#endif // _TEXTURE_SHADER_H_