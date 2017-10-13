/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "DiffTexMaterial.h"
#include "Graphics/TextureShader.h"

using namespace Graphics;
using namespace std;

void DiffTexMaterial::prepareShaderForRendering(const Math::Matrix4x4 &worldViewProjection, const Math::Matrix4x4 &world)
{
	assert(dynamic_cast<TextureShader *>(mShader));
	((TextureShader *)mShader)->setDiffuseTexture(*mTexture);
	mShader->setWorldViewProjectionMatrix(worldViewProjection);
}

DiffTexMaterial::DiffTexMaterial(const string &name, const std::string &shaderName, const string &diffuseTextureName) :
	Material(name, TextureShader::request(shaderName)),
	mTexture(Texture::request(diffuseTextureName))
{

}

DiffTexMaterial::~DiffTexMaterial()
{
	Texture::release(mTexture);
}