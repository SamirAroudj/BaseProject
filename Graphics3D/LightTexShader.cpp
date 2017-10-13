/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "LightTexShader.h"

using namespace Graphics;

using namespace Graphics;
using namespace std;

LightTexShader *LightTexShader::request(const string &name)
{
	Shader *shader = UserResource<Shader>::request(name);
	if (shader)
	{
		assert(dynamic_cast<LightTexShader *>(shader));
		return (LightTexShader *)shader;
	}

	LightTexShader *lightTexShader = new LightTexShader(name);
	return lightTexShader;
}

void LightTexShader::setLights(const std::vector<Light *> &lights)
{
	uint32 numOfLights;
	D3D10_EFFECT_TYPE_DESC typeDescription;
	ID3D10EffectType *lightsType = mLights->GetType();
	
	lightsType->GetDesc(&typeDescription);
	numOfLights = (lights.size() > typeDescription.Elements) ? typeDescription.Elements : lights.size();

	mNumOfLights->SetInt(numOfLights);
	for(uint32 i = 0; i < numOfLights; ++i)
		mLights->GetElement(i)->SetRawValue((void *)&lights[i]->getData(), 0, sizeof(LightData));
}

LightTexShader::LightTexShader(const string &name) : Shader(name), mDiffTex(NULL)
{
	mDiffTex = mEffect->GetVariableByName("gDiffTex")->AsShaderResource();
	assert(mDiffTex->IsValid());
	mSurfaceHardness = mEffect->GetVariableByName("gHardness")->AsScalar();
	assert(mSurfaceHardness->IsValid());

	mWorld = mEffect->GetVariableByName("gWorld")->AsMatrix();
	assert(mWorld->IsValid());
	mEyePosition = mEffect->GetVariableByName("gEyePos");
	assert(mEyePosition->IsValid());

	mLights = mEffect->GetVariableByName("gLights");
	assert(mLights->IsValid());
	mNumOfLights = mEffect->GetVariableByName("gNumLights")->AsScalar();
	assert(mNumOfLights->IsValid());
}

LightTexShader::~LightTexShader()
{

}
