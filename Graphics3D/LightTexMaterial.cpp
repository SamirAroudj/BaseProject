/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Camera.h"
#include "LightTexMaterial.h"

using namespace Graphics;
using namespace Math;
using namespace std;

void LightTexMaterial::prepareShaderForRendering(const Math::Matrix4x4 &worldViewProjection, const Math::Matrix4x4 &world)
{
	assert(dynamic_cast<LightTexShader *>(mShader));
	LightTexShader *shader = (LightTexShader *)mShader;
	shader->setWorldMatrix(world);
	shader->setWorldViewProjectionMatrix(worldViewProjection);

	shader->setDiffuseTexture(*mDiffTexture);
	shader->setSurfaceHardness(mHardness);
	shader->setCameraPosition(Camera::getActiveCamera().getPosition());
	shader->setLights(Light::getLights());
}

LightTexMaterial::LightTexMaterial(const string &name, const std::string &shaderName, const string &diffuseTextureName, Real hardness) :
	Material(name, LightTexShader::request(shaderName)),
	mDiffTexture(Texture::request(diffuseTextureName)),
	mHardness(hardness)
{
	assert(mHardness >= 1.0f);
}

LightTexMaterial::~LightTexMaterial()
{
	Texture::release(mDiffTexture);
}