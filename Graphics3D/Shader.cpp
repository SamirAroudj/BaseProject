/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Platform/ExceptionHandling/CorruptedFileException.h"
#include "Platform/ExceptionHandling/GraphicsException.h"
#include "Graphics/GraphicsManager.h"
#include "MagicGraphicsConstants.h"
#include "Shader.h"

using namespace ExceptionHandling;
using namespace Graphics;
using namespace Math;
using namespace ResourceManagement;
using namespace std;

vector<Shader *> *Resource<Shader>::msResources = new vector<Shader *>();
const char *Resource<Shader>::msPathToResources = PATH_TO_SHADERS;

void Shader::render(const StaticMesh &staticMesh) const
{
    D3D10_TECHNIQUE_DESC techniqueDescription;
    mCurrentTechnique->GetDesc(&techniqueDescription);
	uint32 numOfPasses = techniqueDescription.Passes;
    for(uint32 passIndex = 0; passIndex < numOfPasses; ++passIndex)
    {
		mCurrentTechnique->GetPassByIndex(passIndex)->Apply(0);
        staticMesh.render();
    }
}

Shader *Shader::request(const string &shaderFile)
{
	Shader *shader = UserResource<Shader>::request(shaderFile);	// exists?
	if (shader)
		return shader;

	shader = new Shader(shaderFile);
	return shader;
}

void Shader::setWorldViewProjectionMatrix(const Matrix4x4 &matrix)
{
	mWorldViewProjection->SetMatrix(const_cast<Real *>(matrix.getData()));
}

Shader::Shader(const string &shaderFile) :
	UserResource(shaderFile)
{
	string fullName = msPathToResources + shaderFile;
	uint32 flags = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG
	flags |= D3D10_SHADER_DEBUG;
	flags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif // _DEBUG
	ID3D10Blob *compilationErrors = NULL;

	HRESULT result = D3DX10CreateEffectFromFileA(fullName.c_str(), NULL, NULL, "fx_4_0", flags, 0,
		GraphicsManager::getSingleton().getDevice(), NULL, NULL, &mEffect, &compilationErrors, 0);
	if (S_OK != result)	// exception handling
	{
		if (compilationErrors)
		{
			CorruptedFileException *exception = new CorruptedFileException(reinterpret_cast<char *>(compilationErrors->GetBufferPointer()), shaderFile);
			compilationErrors->Release();
			throw exception;
		}
		else
		{
			string message = "Could not create a shader. Name: ";
			message += shaderFile;
			message += " Path: ";
			message += msPathToResources;
			throw GraphicsException(message, result);
		}
	}

	mCurrentTechnique = mEffect->GetTechniqueByIndex(0);	// get access to basic shader elements
	assert(mCurrentTechnique->IsValid());
	mWorldViewProjection = mEffect->GetVariableByName("gWVP")->AsMatrix();
	assert(mWorldViewProjection->IsValid());
}

Shader::~Shader()
{
	mEffect->Release();
}
