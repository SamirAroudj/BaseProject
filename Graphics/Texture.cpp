/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
	#include <Windows.h>
#endif // _WINDOWS
#include <GL/gl.h>

#include "Graphics/ImageManager.h"
#include "Graphics/MagicConstants.h"
#include "Graphics/Texture.h"
#include "Math/MathHelper.h"
#include "Platform/FailureHandling/FileAccessException.h"
#include "Platform/FailureHandling/GraphicsException.h"

using namespace FailureHandling;
using namespace Graphics;
using namespace Math;
using namespace ResourceManagement;
using namespace std;
using namespace Storage;
using namespace Utilities;

// static member definitions of base class Resource<Texture> of class Texture
template <>
Path Texture::Resource<Texture>::msResourcePath(PATH_TO_TEXTURES);

template <>
vector<Texture *> Texture::Resource<Texture>::msResources(0);

uint32 Texture::getOpenGLMagFilter(const MagFiltering magFilter)
{
	switch (magFilter)
	{
	case FILTERING_MAG_NEAREST:
		return GL_NEAREST;

	case FILTERING_MAG_LINEAR:
		return GL_LINEAR;

	default:
		assert(false);
		return GL_NEAREST;
	}
}

uint32 Texture::getOpenGLMinFilter(const MinFiltering minFilter)
{
	switch (minFilter)
	{
	case FILTERING_MIN_NEAREST:
		return GL_NEAREST;

	case FILTERING_MIN_LINEAR:
		return GL_LINEAR;

	case FILTERING_MIN_MIPMAPPING_NEAREST:
		return GL_NEAREST_MIPMAP_NEAREST;

	case FILTERING_MIN_MIPMAPPING_LINEAR_TEXELS_CLOSEST_MIPMAP_LEVEL:
		return GL_LINEAR_MIPMAP_NEAREST;

	case FILTERING_MIN_MIPMAPPING_NEAREST_TEXELS_LINEAR_MIPMAP_LEVELS:
		return GL_NEAREST_MIPMAP_LINEAR;

	case FILTERING_MIN_MIPMAPPING_LINEAR:
		return GL_LINEAR_MIPMAP_LINEAR;

	default:
		assert(false);
		return GL_NEAREST;
	}
}

uint32 Texture::getOpenGLWrappingMode(Wrapping wrappingMode)
{
	switch (wrappingMode)
	{
	case WRAPPING_CLAMP:
		return GL_CLAMP;

	case WRAPPING_REPEAT:
		return GL_REPEAT;

	default:
		assert(false);
		return GL_REPEAT;
	}
}

void Texture::getSurroundingPixelCoords(int32 &minX, int32 &minY, int32 &maxX, int32 &maxY, Real &factorX, Real &factorY,
	const Math::Vector2 &coords, const ImgSize &size, const Wrapping wrapping)
{
	// get pixel coords
	getSurroundingPixelCoords(minX, minY, maxX, maxY, coords);

	// do wrapping and get factors
	switch (wrapping)
	{
		case WRAPPING_CLAMP:
		{
			// clamp x coordinate & get factorX
			if (maxX <= 0)
			{
				minX = 0;
				maxX = 0;
				factorX = 0;
			}
			else if ((uint32) minX >= size[0] - 1)
			{
				minX = size[0] - 1;
				maxX = size[0] - 1;
				factorX = 0;
			}
			else
			{
				factorX = coords.x - minX - 0.5f;
			}
			
			// clamp y coordinate & get factorY
			if (maxY <= 0)
			{
				minY = 0;
				maxY = 0;
				factorY = 0;
			}
			else if ((uint32) minY >= size[1] - 1)
			{
				minY = size[1] - 1;
				maxY = size[1] - 1;
				factorY = 0;
			}
			else
			{
				factorY = coords.y - minY - 0.5f;
			}

			break;
		}
		
		case WRAPPING_REPEAT:
		{
			minX = minX % size[0];
			maxX = maxX % size[0];
			factorX = fmod(coords.x - minX - 0.5f, (Real) size[0]);

			minY = minY % size[1];
			maxY = maxY % size[1];
			factorY = fmod(coords.y - minY - 0.5f, (Real) size[1]);

			break;
		}

		case WRAPPING_NUM_OF:
		default:
		{
			assert(false);
			minX = maxX = minY = maxY = 0xffffffff;
			factorX = factorY = REAL_MAX;
			throw GraphicsException("Unsupported wrapping mode in Texture::getSurroundingPixelCoords.", wrapping);
			break;
		}
	}
}

void Texture::getSurroundingPixelCoords(int32 &minX, int32 &minY, int32 &maxX, int32 &maxY, const Math::Vector2 &coords)
{
	maxX = (int32) roundr(coords.x);
	minX = maxX - 1;
	
	maxY = (int32) roundr(coords.y);
	minY = maxY - 1;
}

Texture *Texture::request(const string &resourceName, const TextureHeader *header, const uint8 *pixelData)
{
	Texture *texture = UserResource<Texture>::request(resourceName);
	if (!texture)
		texture = new Texture(resourceName, header, pixelData);
	
	return texture;
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, mIdentifier);
}

Texture::Texture(const string &relativeName, const TextureHeader *header, const uint8 *pixelData) : 
	UserResource(relativeName), mTileSize(1.0f, 1.0f)
{
	// load texture data from file
	if (NULL == header)
	{
		Path absoluteName = Path::appendChild(msResourcePath, relativeName);	// load the texture by means of the image manager
		mIdentifier = ImageManager::getSingleton().loadTileTexture(mTileSize, absoluteName);
		if (0 == mIdentifier)
		{
			string message = "Could not load an image.\n";
			throw FileAccessException(message, absoluteName, 5);
		}
	}
	// initialize new texture with entered data
	else
	{
		assert(pixelData);

		mTileSize = header->mTileSize;
		mIdentifier = ImageManager::getSingleton().createTexture(*header, pixelData);

		if (0 == mIdentifier)
		{
			string message = "Could not create a texture. Returned graphics API texture identifier is invalid. (identifier = 0) \n";
			throw GraphicsException(message, 6);
		}
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &mIdentifier);
}
