/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Graphics/TextureShader.h"

using namespace Graphics;
using namespace ResourceManagement;
using namespace std;

TextureShader *TextureShader::request(const string &name)
{
	Shader *shader = UserResource<Shader>::request(name);
	if (shader)
	{
		assert(dynamic_cast<TextureShader *>(shader));
		return (TextureShader *)shader;
	}

	TextureShader *textureShader = new TextureShader(name);
	return textureShader;
}

TextureShader::TextureShader(const string &name) : Shader(name), mDiffTex(NULL)
{
	mDiffTex = mEffect->GetVariableByName("gDiffTex")->AsShaderResource();
	assert(mDiffTex->IsValid());
}

TextureShader::~TextureShader()
{

}
