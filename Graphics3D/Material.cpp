/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cstring>
#include "ColorMaterial.h"
#include "Platform/ExceptionHandling/CorruptedFileException.h"
#include "DiffTexMaterial.h"
#include "Platform/File.h"
#include "LightTexMaterial.h"
#include "MagicGraphicsConstants.h"
#include "MagicConstants.h"
#include "Material.h"

using namespace ExceptionHandling;
using namespace Graphics;
using namespace ResourceManagement;
using namespace Platform;
using namespace std;

vector<Material *> *Resource<Material>::msResources = new vector<Material *>();
const char *Resource<Material>::msPathToResources = PATH_TO_MATERIALS;

Material *Material::request(const string &name)
{
	Material *material = UserResource<Material>::request(name);
	if (material)
		return material;

	material = loadMaterial(name);
	return material;
}

Material *Material::loadMaterial(const std::string &name)
{
	Material *material = NULL;
	char buffer[FILE_READING_BUFFER_SIZE];
	char shaderName[FILE_READING_BUFFER_SIZE];
	string fullName = msPathToResources + name;
	File file(fullName, File::OPEN_READING, false);

	if (feof(&file.getHandle()))	// material type + shader name
		throw CorruptedFileException("Material file does not contain a line to identify the material type.", fullName);
	fgets(buffer, FILE_READING_BUFFER_SIZE, &file.getHandle());
	if (feof(&file.getHandle()))
		throw CorruptedFileException("Material file does not contain a line with a shader name.", fullName);
	fscanf_s(&file.getHandle(), "%s\n", shaderName, FILE_READING_BUFFER_SIZE);

	if (0 == strncmp(buffer, "Color", 5))	// create a ColorMaterial object
	{
		material = new ColorMaterial(name, shaderName);
	}
	else if (0 == strncmp(buffer, "Graphics/Texture", 7))	// create a DiffTexMaterial object
	{
		if (feof(&file.getHandle()))
			throw CorruptedFileException("Graphics/Texture material file does not contain a name of a diffuse texture.", fullName);
		fscanf_s(&file.getHandle(), "%s\n", buffer, FILE_READING_BUFFER_SIZE);
		material = new DiffTexMaterial(name, shaderName, buffer);
	}
	else if (0 == strncmp(buffer, "LightTex", 8))
	{
		Real hardness;
		if (feof(&file.getHandle()))
			throw CorruptedFileException("LightTex material file does not contain a name of a diffuse texture.", fullName);
		fscanf_s(&file.getHandle(), "%s\n", buffer, FILE_READING_BUFFER_SIZE);
		fscanf_s(&file.getHandle(), "%f\n", &hardness);
		material = new LightTexMaterial(name, shaderName, buffer, hardness);
	}

	assert(material);
	return material;
}

Material::Material(const string &name, Shader *shader) :
	UserResource<Material>(name),
	mShader(shader)
{ 
	assert(mShader);
}

Material::~Material()
{
	if (mShader)
		Shader::release(mShader);
}
