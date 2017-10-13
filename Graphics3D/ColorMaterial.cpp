/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "ColorMaterial.h"

using namespace Graphics;

void ColorMaterial::prepareShaderForRendering(const Math::Matrix4x4 &worldViewProjection, const Math::Matrix4x4 &world)
{
	mShader->setWorldViewProjectionMatrix(worldViewProjection);
}

ColorMaterial::ColorMaterial(const std::string &name, const std::string &shaderName) :
	Material(name, Shader::request(shaderName))
{

}

ColorMaterial::~ColorMaterial()
{

}