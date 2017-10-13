/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Platform/ExceptionHandling/GraphicsException.h"
#include "Graphics/GraphicsManager.h"
#include "MagicGraphicsConstants.h"
#include "Graphics/Texture.h"

using namespace ExceptionHandling;
using namespace Graphics;
using namespace ResourceManagement;
using namespace std;

vector<Texture *> *Resource<Texture>::msResources = new vector<Texture *>();
const char *Resource<Texture>::msPathToResources = PATH_TO_TEXTURES;

Texture *Texture::request(const string &name)
{
	Texture *texture = UserResource<Texture>::request(name);
	if (NULL != texture)
		return texture;

	texture = new Texture(name);
	return texture;
}

Texture::Texture(const string &name) : 
	UserResource<Texture>(name)
{
	string fullName = msPathToResources + name;
	HRESULT result = D3DX10CreateShaderResourceViewFromFileA(GraphicsManager::getSingleton().getDevice(), fullName.c_str(), NULL, NULL, &mTextureView, NULL);
	if (S_OK != result)
	{
		string message = "Could not load a shader resource view (texture) from file. Name: ";
		message += name;
		message += " Path: ";
		message += msPathToResources;
		if (D3D10_ERROR_FILE_NOT_FOUND == result)
			message += " File was not found.";
		throw GraphicsException(message, result);
	}
}

Texture::~Texture()
{
	mTextureView->Release();
}
