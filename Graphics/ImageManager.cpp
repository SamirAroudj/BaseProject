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

#include <cassert>
#include <cstring>
#include <GL/gl.h>
#include "Math/MathHelper.h"
#include "Graphics/ImageManager.h"
#include "Graphics/MagicConstants.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureHeader.h"
#include "Platform/FailureHandling/FileAccessException.h"
#include "Platform/FailureHandling/FileCorruptionException.h"
#include "Platform/FailureHandling/FileException.h"
#include "png.h"

using namespace FailureHandling;
using namespace Graphics;
using namespace Platform;
using namespace std;
using namespace Storage;
using namespace Utilities;

ImageManager::~ImageManager()
{

}

/*void ImageManager::convertToTileTexture(Size2<Real> &tileSize, const string &sourceImageName, const string &tileTextureName)
{
	assert(tileSize[0] > 0.0f);
	assert(tileSize[1] > 0.0f);

	BITMAPINFO info;
	TextureHeader header;
	uint32 format = 0;

	if (!mIlutInitialized) 	// init image library to be able to load common image types, such as PNG, BMP, JPG
	{
		mIlutInitialized = true;
		ilInit();
		ilutRenderer(ILUT_OPENGL);
		ilutEnable(ILUT_OPENGL_CONV);
	}
	
	// load the source image and get its properties
	uint32 identifier = ilutGLLoadImage(const_cast<char *>(sourceImageName.c_str()));
	glBindTexture(GL_TEXTURE_2D, identifier);
	ilutGetBmpInfo(&info);
	header.mTileSize[0] = tileSize[0];
	header.mTileSize[1] = tileSize[1];
	header.mWidthHeight = info.bmiHeader.biWidth;
	switch(info.bmiHeader.biBitCount)
	{
		case 24:	// 3 channels: RGB
			format = GL_RGB;
			header.mFormat = Texture::FORMAT_RGB;
			break;

		case 32: // 4 channels: RGBA
			format = GL_RGBA;
			header.mFormat = Texture::FORMAT_RGBA;
			break;	

		default: 
			assert(false);
	}

	// get actual pixel data
	uint8 *pixels = new uint8[info.bmiHeader.biSizeImage];
	glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, pixels);

	// create the converted file
	File file(tileTextureName, File::CREATE_WRITING, true);
	fprintf(&file.getHandle(), TILE_TEXTURE_START_CHARS);
	fwrite(&header, sizeof(TextureHeader), 1, &file.getHandle());
	fwrite(pixels, info.bmiHeader.biSizeImage, 1, &file.getHandle());

	// free resources
	delete [] pixels;
	pixels = NULL;
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &identifier);
}*/

uint8 *ImageManager::loadPNG(ImgSize &size, Texture::Format &format, const Path &fileName) const
{
	// initialize output parameters
	size.set(0, 0);
	format = Texture::FORMAT_NUM_OF;

	// access data via lib png functions and structs
	uint8 *pixels = NULL;
	png_structp pngData = NULL;
	png_infop pngInfo = NULL;
	png_bytep *rowPointers = NULL;
	uint32 bitDepth = 0;
	uint32 channelCount = 0;

	try
	{
		File file(fileName, File::OPEN_READING, true);
		
		// get header data
		{
			// each png file starts with a specific 8 byte sequence -> check it to be sure that the file is a png
			const uint8 SIGNATURE_BYTE_COUNT = 8;
			png_byte signature[SIGNATURE_BYTE_COUNT];
			uint8 readSequenceCount = file.read(signature, SIGNATURE_BYTE_COUNT, SIGNATURE_BYTE_COUNT, 1);
			if (file.errorOccured() || 0 == readSequenceCount || !file.hasLeftData())
				throw FileCorruptionException("Could not read the mandatory PNG file signature.", fileName);

			if (0 != png_sig_cmp(signature, 0, SIGNATURE_BYTE_COUNT))
				throw FileCorruptionException("The \"supposed\" PNG file does not start with the mandatory PNG signature but with something else.", fileName);

			// initialize png data and info structs
			pngData = png_create_read_struct(PNG_LIBPNG_VER_STRING,	NULL, NULL, NULL);
			if (!pngData)
				throw Exception("Could not create png data structure since the call of png_create_read_struct failed.");

			pngInfo = png_create_info_struct(pngData);
			if (!pngInfo)
				throw Exception("Could not create png info structure since the call of png_create_info_struct failed.");

			// prepare png data struct for file reading operations
			png_init_io(pngData, &file.getHandle());
			png_set_sig_bytes(pngData, SIGNATURE_BYTE_COUNT); // signature was already read

			// get header / meta data
			png_read_info(pngData, pngInfo);
			
			// get meta data
			size[0] = png_get_image_width(pngData, pngInfo);
			size[1] = png_get_image_height(pngData, pngInfo);

			bitDepth = png_get_bit_depth(pngData, pngInfo);
			if (bitDepth > 8)
				throw FileException("Unsupported png file bit depth: Up to 8 bit per channel are supported.", fileName);
			
			channelCount = png_get_channels(pngData, pngInfo);
			if (channelCount != 3 && channelCount != 4) // todo
				throw FileException("Unsupported png file format: RGB and RGBA images are supported (3 or 4 channels).", fileName);
			format = (channelCount == 3 ? Texture::FORMAT_RGB : Texture::FORMAT_RGBA);
		}

		// load color data
		{
			// convert data to get an 8bit per channel data array
			{
				const uint32 colorType = png_get_color_type(pngData, pngInfo);

				// no palette data but 8 bit per channel color values
				if (PNG_COLOR_TYPE_PALETTE == colorType)
					png_set_palette_to_rgb(pngData);

				// not fewer than 8 bit per channel as format for the color data array
				if (PNG_COLOR_TYPE_GRAY == colorType && bitDepth < 8)
					png_set_expand_gray_1_2_4_to_8(pngData);

				// not transparency in a tRNS chunk but within the color data array
				if (png_get_valid(pngData, pngInfo, PNG_INFO_tRNS))
					png_set_tRNS_to_alpha(pngData);

				// update meta data about data conversion
				png_read_update_info(pngData, pngInfo);
			}

			// allocate pixel data and set it to black
			const uint32 pixelCount = size.getElementCount();
			pixels = new uint8[pixelCount * channelCount];
			memset(pixels, 0, sizeof(uint8) * pixelCount * channelCount);
			
			// finally, read the color data row by row
			rowPointers = new png_bytep[size[1]];
			for (uint32 rowIdx = 0; rowIdx < size[1]; ++rowIdx)
				rowPointers[rowIdx] = pixels + size[0] * channelCount * rowIdx;
			png_read_image(pngData, rowPointers);

			// clean up
			delete [] rowPointers;
			png_destroy_read_struct(&pngData, &pngInfo, NULL);
		}
	}
	catch (Exception &exception)
	{
		exception;
		// todo log exception
		//e.getMessage();
		//e.getSource();

		// clean up
		delete [] pixels;
		delete [] rowPointers;
		png_destroy_read_struct(&pngData, &pngInfo, NULL);
		return NULL;
	}

	return pixels;
}

uint32 ImageManager::loadTileTexture(Size2<Real> &tileSize, const Path &name) const
{
	TextureHeader header;
	uint32 identifier	= 0;
	File file(name, File::OPEN_READING, true);

	// check file type and get texture header
	{
		char buffer[TILE_TEXTURE_START_CHAR_COUNT];
		const uint32 count = file.read(buffer, TILE_TEXTURE_START_CHAR_COUNT, 1, TILE_TEXTURE_START_CHAR_COUNT);

		if (TILE_TEXTURE_START_CHAR_COUNT != count ||
			0 != strncmp(buffer, TILE_TEXTURE_START_CHARS, TILE_TEXTURE_START_CHAR_COUNT))
		{
			throw FileCorruptionException("Tile texture file doesn't begin with with standard start text.", name);
		}
	}

	// read header
	if (1 != file.read(&header, sizeof(TextureHeader), sizeof(TextureHeader), 1))
		throw FileCorruptionException("Could not read tile texture header.", name);
	if (header.mFormat != Texture::FORMAT_RGB && header.mFormat != Texture::FORMAT_RGBA)
		throw FileCorruptionException("Could not load tile texture data due to unsupported pixel format.", name);

	// get width & height of tiles
	tileSize = header.mTileSize;

	// #bytes required for pixels
	const uint32 pixelCount = header.mWidthHeight * header.mWidthHeight;
	const uint32 pixelSize = (Texture::FORMAT_RGB == header.mFormat ? 3 : 4);
	const uint32 bufferSize = pixelCount * pixelSize;

	// todo memory allocation can this be done better?
	// read pixel data
	uint8 *pixels = new uint8[bufferSize];
	if (pixelCount != file.read(pixels, bufferSize, pixelSize, pixelCount))
		throw FileCorruptionException("Tile texture file does not contain as many pixels as described in its header.", name);

	identifier = createOpenGLTexture(header, pixels);

	// free resources
	delete [] pixels;
	pixels = NULL;

	return identifier;
}

uint32 ImageManager::createTexture(const TextureHeader &header, const uint8 *pixels,
	const Texture::Wrapping wrapping1stDir, const Texture::Wrapping wrapping2ndDir,
	const Texture::MagFiltering magFilter, const Texture::MinFiltering minFilter) const
{
	return createOpenGLTexture(header, pixels, wrapping1stDir, wrapping2ndDir, magFilter, minFilter);
}

uint32 ImageManager::createOpenGLTexture(const TextureHeader &header, const uint8 *pixels,
	const Texture::Wrapping wrapping1stDir, const Texture::Wrapping wrapping2ndDir,
	const Texture::MagFiltering magFilter, const Texture::MinFiltering minFilter) const
{
	uint32 identifier	= 0;
	GLenum formatOpenGL	= -1;

	#ifdef _DEBUG
		// check that texture dimension is a power of two	
		bool		 powerOfTwo		= (1 == header.mWidthHeight);

		for (uint32 exponent = 1, size = 2;
			 exponent <= TEXTURE_MAX_EXPONENT && !powerOfTwo;
			 ++exponent, size *= 2)
				powerOfTwo |= (size == header.mWidthHeight);

		assert(powerOfTwo);
	#endif // _DEBUG

	// translate format
	if (Texture::FORMAT_RGB == header.mFormat)
		formatOpenGL = GL_RGB;
	else if (Texture::FORMAT_RGBA == header.mFormat)
		formatOpenGL = GL_RGBA;
	else
		assert(false);

	// create and bind texture
	glGenTextures(1, &identifier);
	glBindTexture(GL_TEXTURE_2D, identifier);

	// set up texture filters and wraping mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Texture::getOpenGLWrappingMode(wrapping1stDir));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Texture::getOpenGLWrappingMode(wrapping2ndDir));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Texture::getOpenGLMinFilter(minFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Texture::getOpenGLMagFilter(magFilter));

	// copy data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, header.mWidthHeight, header.mWidthHeight, 0, formatOpenGL, GL_UNSIGNED_BYTE, pixels);

	return identifier;
}

void ImageManager::savePNG(File &file, const Real *pixels, const uint32 channelCount, const ImgSize &size, int32 compressionLevel)
{
	uint8 *integerPixels = NULL;

	try
	{
		// reserve memory
		const uint32 pixelCount = size.getElementCount();
		integerPixels = new uint8[pixelCount];

		// convert pixels
		for (uint32 pixelIdx = 0; pixelIdx < pixelCount; ++pixelIdx)
			integerPixels[pixelIdx] = static_cast<uint8>(pixels[pixelIdx] * 255);

		// save pixel data to file
		savePNG(file, integerPixels, channelCount, size, compressionLevel);
		delete [] integerPixels;
		integerPixels = NULL;
	}
	catch (Exception &exception)
	{
		delete [] integerPixels;
		throw exception;
	}
}

void ImageManager::savePNG(File &file, const uint8 *pixels, const uint32 channelCount, const ImgSize &size, int32 compressionLevel)
{
	assert(pixels);
	if (!pixels || size[0] <= 0 || size[1] <= 0)
		throw Exception("Invalid data for png file output!");

	// bit depth & color type / format
	const png_byte bitDepth = 8;
	png_byte colorType;

	switch (channelCount)
	{
		case 0: colorType = PNG_COLOR_TYPE_PALETTE; break;
		case 1: colorType = PNG_COLOR_TYPE_GRAY; break;
		case 2: colorType = PNG_COLOR_TYPE_GRAY_ALPHA; break;
		case 3: colorType = PNG_COLOR_TYPE_RGB; break;
		case 4: colorType = PNG_COLOR_TYPE_RGB_ALPHA; break;
		default:
			throw Exception("Unsupported color type for png file creation.");
	}

	// png body, header & data source
	png_struct *png = NULL;
	png_info *pngHeader = NULL;
	png_byte **rowPointers = new png_byte *[size[1]];

	// set pixels as row-wise png data source
	const uint8 *rowPointer = pixels;
	const uint32 rowOffset = size[0] * channelCount;
	for (uint32 rowIdx = 0; rowIdx < size[1]; ++rowIdx, rowPointer += rowOffset)
		rowPointers[rowIdx] = (png_byte *) rowPointer;

	try
	{
		// create body and header & prepare writing
		png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); // create struct for writing without user error or error or warning function pointers
		if (!png)
			throw Exception("Could not create png_struct for png file creation.");

		pngHeader = png_create_info_struct(png);
		if (!pngHeader)
			throw Exception("Could not create png_info for png file creation.");

		// set png header / meta data
		png_set_IHDR(png, pngHeader, size[0], size[1], bitDepth, colorType, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		//png_write_info(png, pngHeader);
		//png_set_compression_level(png, compressionLevel);

		// prepare IO
		png_init_io(png, &file.getHandle());
		png_set_rows(png, pngHeader, rowPointers);

		// actually write the data to the file
		png_write_png(png, pngHeader, PNG_TRANSFORM_IDENTITY, NULL);

		//png_write_image(png, rowPointers);
		//png_write_end(png, pngHeader);

		// free memory
		delete [] rowPointers;
		png_destroy_write_struct(&png, &pngHeader);

		rowPointers = NULL;
		pngHeader = NULL;
		png = NULL;
	}
	catch (Exception &exception)
	{
		// free memory
        png_destroy_write_struct(&png, &pngHeader);
		delete [] rowPointers;

		throw exception;
	}
}
