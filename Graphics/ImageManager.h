/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _IMAGE_MANAGER_H_
#define _IMAGE_MANAGER_H_

#include <string>
#include "Graphics/TextureHeader.h"
#include "Patterns/Singleton.h"
#include "Platform/Storage/File.h"
#include "Platform/Utilities/Size2.h"

namespace Graphics
{
	/// This class is responsible for texture conversion and loading.
	class ImageManager : public Patterns::Singleton<ImageManager>
	{
	public:
		/** Creates a unique instance of the ImageManager. There can only be one ImageManager. */
		ImageManager() { }

		/** Frees resources. */
		virtual ~ImageManager();

		/** Loads an image (e.g. png, bmp) converts it to the tile texture format and saves it to disc.
		@param sourceImageName This is the file name (with path) of the image to be converted.
		@param tileTextureName The converted data will be saved to tileTextureName.
							  tileTextureName must also contain the path to the new file.
		@param tileWidth Specifies the width of each tile the texture contains. tileWidth \in [0, 1]
		@param tileHeight Specifies the height of each tile the texture contains tileHeight \in [0, 1] */
		/*void convertToTileTexture(const std::string &sourceImageName,
			const std::string &tileTextureName, Real tileWidth, Real tileHeight);*/

		/** Loads a png image from file and returns a pointer to the color array.
		@param size Is set to the png image size (width and height in pixels).
		@param format Is set according to the PNG files format, e.g. RGB for an image without transparency.
		@param fileName Set this to the complete path to the file including its name.
		@return Returns a pointer providing access to the raw image data formatted according to header. */
		uint8 *loadPNG(Utilities::ImgSize &size, Texture::Format &format, const Storage::Path &fileName) const;

		/** Loads and creates an OpenGL texture from a tile texture file.
		@param textureFileName	This is the name of the texture without path and without extension.
								TextureFileName must also contain the path to the new file.
		@param tileSize Contains the width [0] and height [1] of each tile the texture contains. tileSize \in (0, 1]^2
		@return The function returns the OpenGL identifier of the new texture.*/
		uint32 loadTileTexture(Utilities::Size2<Real> &tileSize, const Storage::Path &textureFileName) const;
	
		/** Creates a texture and fills it with the entered pixel data.
		@param header Contains meta data of the texture to be created. See Graphics::TextureHeader for more information.
		@param pixels Pointer to an array containing the actual pixel data. Size should be width * height * number of color channels * color channel size.
		@param wrapping1stDir Set this to texture repetition or clamping. Refers to the first (also called s or u) texture coordinate axis.
		@param wrapping1stDir Set this to texture repetition or clamping. Referst to the second (also called t or v) texture coordinate axis.
		@param magFilter Defines what filtering is used when a texture is larger in screen space than in texture space. (Texels must be magnified.)
		@param minFilter Defines what filtering is used when a texture is smaller in screen space than in texture space. (Texels must be shrinked.)
		@return The function returns the OpenGL identifier of the new texture. */
		uint32 createTexture(const TextureHeader &header, const uint8 *pixels,
			const Texture::Wrapping wrapping1stDir = Texture::WRAPPING_REPEAT, const Texture::Wrapping wrapping2ndDir =Texture:: WRAPPING_REPEAT, 
			const Texture::MagFiltering magFilter = Texture::FILTERING_MAG_NEAREST, const Texture::MinFiltering minFilter = Texture::FILTERING_MIN_NEAREST) const;

		/** Saves pixel data in a file in the PNG format. 
		@param file Set this to the file in which you want to store the data. libPNG will write to it the pixel data.
		@param pixels The pixels are assumed to be in the range [0, 1] for which reason they are internally converted to values within {0, 1, ..., 255}.
			This requires a temporary uint8 buffer which is as big as size.getElementCount().
		@param channelCount Set this to a value \in {1, 2, 3, 4} which correspond to gray image, gray image with alpha, RGB image, RGBA image.
		@param size This defines the image size. Set this to the width and height of the image in pixels.
		@param compressionLevel Set this to the wanted compression level. See libPNG for more information. */
		void savePNG(Storage::File &file, const Real *pixels, const uint32 channelCount, const Utilities::ImgSize &size, int32 compressionLevel = -1);
		
		/** Saves pixel data in a file in the PNG format. 
		@param file Set this to the file in which you want to store the data. libPNG will write to it the pixel data.
		@param pixels The pixels are assumed to be \in {0, 1, 2, ..., 255 }.
		@param channelCount Set this to a value \in {1, 2, 3, 4} which correspond to gray image, gray image with alpha, RGB image, RGBA image.
		@param size This defines the image size. Set this to the width and height of the image in pixels.
		@param compressionLevel Set this to the wanted compression level. See libPNG for more information. */
		void savePNG(Storage::File &file, const uint8 *pixels, const uint32 channelCount, const Utilities::ImgSize &size, int32 compressionLevel = -1);

	private:
		/** Creates an OpenGL texture and fills it with the entered pixel data.
		@param header Contains meta data of the texture to be created. See Graphics::TextureHeader for more information.qual and a power of two: width = height = 2^x.
		@param pixels Pointer to an array containing the actual pixel data. Size should be width * height * number of color channels * color channel size.
		@param wrapping1stDir Set this to texture repetition or clamping. Refers to the first (also called s or u) texture coordinate axis.
		@param wrapping1stDir Set this to texture repetition or clamping. Referst to the second (also called t or v) texture coordinate axis.
		@param magFilter Defines what filtering is used when a texture is larger in screen space than in texture space. (Texels must be magnified.)
		@param minFilter Defines what filtering is used when a texture is smaller in screen space than in texture space. (Texels must be shrinked.)
		@return The function returns the OpenGL identifier of the new texture. */
		uint32 createOpenGLTexture(const TextureHeader &header, const uint8 *pixels,
			const Texture::Wrapping wrapping1stDir = Texture::WRAPPING_REPEAT, const Texture::Wrapping wrapping2ndDir = Texture::WRAPPING_REPEAT, 
			const Texture::MagFiltering magFilter = Texture::FILTERING_MAG_NEAREST, const Texture::MinFiltering minFilter = Texture::FILTERING_MIN_NEAREST) const;

		/** Copy constructor is forbidden for safe programming & singleton reasons..
		@param copy Copy constructor is forbidden for safe programming & singleton reasons. */
		ImageManager(const ImageManager &copy) { assert(false); }

		/** Assign operator is forbidden for safe programming & singleton reasons.
		@param rhs Assign operator is forbidden for safe programming & singleton reasons.
		@return Assign operator is forbidden for safe programming & singleton reasons.  */
		ImageManager &operator =(const ImageManager &rhs) { assert(false); return *this; }
	};
}

#endif // _IMAGE_MANAGER_H_
