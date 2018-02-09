/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Math/Vector2.h"
#include "Platform/ResourceManagement/UserResource.h"
#include "Utilities/Size2.h"

namespace Graphics
{
	struct TextureHeader;

	/// Textures contain one or more pictures referred to as tiles
	class Texture : public ResourceManagement::UserResource<Texture>
	{
	friend class ResourceManagement::UserResource<Texture>;
	public:
		/** Defines possible wrapping modes for texture coordinates outside the interval [0, 1] \in R. 
		@see See https://www.opengl.org/sdk/docs/man/html/glTexParameter.xhtml */
		enum Wrapping
		{
			WRAPPING_CLAMP,
			WRAPPING_REPEAT,
			WRAPPING_NUM_OF
		};
		
		/** Filtering which is used in case of texture magnification. */
		enum MagFiltering
		{
			FILTERING_MAG_NEAREST,	/// nearest neighbor texel lookup
			FILTERING_MAG_LINEAR	/// linear interpolation between 4 corresponding texels
		};

		/** Filtering which is used in case of texture minification. */
		enum MinFiltering
		{
			FILTERING_MIN_NEAREST,											/// no mipmapping, nearest neighbor texel lookup
			FILTERING_MIN_LINEAR,											/// no mipmapping, linear interpolation between 4 closest texels
			FILTERING_MIN_MIPMAPPING_NEAREST,								/// nearest neighbor filtering for texels and closest mipmap level
			FILTERING_MIN_MIPMAPPING_LINEAR_TEXELS_CLOSEST_MIPMAP_LEVEL,	/// linear interpolation of texels within closest mipmap level
			FILTERING_MIN_MIPMAPPING_NEAREST_TEXELS_LINEAR_MIPMAP_LEVELS,	/// nearest neighbor filtering for texels and linear interpolation between chosen texels for the two closest mipmap levels
			FILTERING_MIN_MIPMAPPING_LINEAR,								/// linear filtering in textures of two cloesest mipmap levels and linear interpolation of results (for 2D textures: trilinear filtering)
		};

		enum Format	/// pixel structure
		{
			FORMAT_RGB,			/// red, green, blue - 24bit
			FORMAT_RGBA,		/// red, green, blue, alpha - 32bit
			FORMAT_NUM_OF		/// number of possible Format enumeration values
		};

	public:

		/** Converts a platform independent texture magnification technique identifier to its corresponding OpenGL value.
		@param magFilter Set this to the platform indipendent magnification filter technique value you want to get the corresponding OpenGL value for.
		@return Returns the OpenGL value which corresponds to magFilter. */
		static uint32 getOpenGLMagFilter(const Texture::MagFiltering magFilter);
		
		/** Converts a platform independent texture minification technique identifier to its corresponding OpenGL value.
		@param minFilter Set this to the platform indipendent minification filter technique value you want to get the corresponding OpenGL value for.
		@return Returns the OpenGL value which corresponds to minFilter. */
		static uint32 getOpenGLMinFilter(const Texture::MinFiltering minFilter);

		/** Converts the platform independent value wrappingMode to its corresponding OpenGL value.
		@param wrappingMode This is a platform independent version of possible wrapping modes.
		@return Returns the OpenGL value which corresponds to the entered platform independent value wrappingMode.*/
		static uint32 getOpenGLWrappingMode(Texture::Wrapping wrappingMode);

		/** Computes the integer coordinates of the four closest pixels around the real coordinates coords.
			The output pixel coordinates are not necessary within some region and they can be negative.
			E.g. coords = (-0.6, 1.7) -> minX = -2, minY = 1, maxX = -1, maxY = 2
		@param minX Is set to the minimum x-coordinates of the bounding box of the four closest pixels around coords.
		@param minY Is set to the minimum y-coordinates of the bounding box of the four closest pixels around coords.
		@param maxX Is set to the maiximum x-coordinates of the bounding box of the four closest pixels around coords.
		@param maxY Is set to the maiximum y-coordinates of the bounding box of the four closest pixels around coords. 
		@param coords Set this to the real coordinates you want to get the colosest for pixels for.*/
		static void getSurroundingPixelCoords(int32 &minX, int32 &minY, int32 &maxX, int32 &maxY,
			const Math::Vector2 &coords);
		
		/** Computes the integer coordinates of the four closest pixels around the real coordinates coords.
			The output pixel coordinates are always within the space [0, width - 1] x [0, height -1] thanks to some kind of wrapping.
		@param minX Is set to the minimum x-coordinates of the bounding box of the four closest pixels around coords.
		@param minY Is set to the minimum y-coordinates of the bounding box of the four closest pixels around coords.
		@param maxX Is set to the maiximum x-coordinates of the bounding box of the four closest pixels around coords.
		@param maxY Is set to the maiximum y-coordinates of the bounding box of the four closest pixels around coords. 
		@param factorX Is set to a value of the interval [0, 1] whereas 0 means that the coords are at the left column (0.5 - directly in the middle, 1.0 at the right column) of the 4x4 pixel bounding box.
		@param factorY Is set to a value of the interval [0, 1] whereas 0 means that the coords are at the lower row (0.5 - directly in the middle, 1.0 at the top row) of the 4x4 pixel bounding box.
		@param coords Set this to the real coordinates you want to get the colosest for pixels for.
		@param imgSize Set this to the range of pixel coordinates you want to allow. minX (minY) and maxX (maxY) are always within the interval [0, width - 1] ([0, height - 1).
		@param wrapping Defines how to handle pixel coordinates outside the allowed domain [0, width - 1] x [0, height -1].
			E.g. do you want the pixel coordinates to be clamped or to repeat and start again?
			See Texture::Wrapping for more information. */
		static void getSurroundingPixelCoords(int32 &minX, int32 &minY, int32 &maxX, int32 &maxY, Real &factorX, Real &factorY,
			const Math::Vector2 &coords, const Utilities::ImgSize &imgSize, const Wrapping wrapping);

		/** Requests a texture and loads data from file or requests a texture and creates it according to entered header and pixel data.
		@param textureName The name should not contain a path since the path should be managed by the base class.
		@param header Set header to NULL to load texture data from file.
					  Set this to a header data structure if you don't want to load data from file but initialize the texture to be created with some pixel data.
					  pixelData must not be NULL in this case.
		@param pixelData Set this to the data to initialize the texture with if you don't want to load data from file. Header must not be NULL in this case. */
		static Texture *request(const std::string &textureName, const TextureHeader *header = NULL, const uint8 *pixelData = NULL);

	public:
		/** Binds a texture to use. */
		void bind() const;

		/** Returns a unique identifier which is necessary for OpenGL functions.
		@return The returned identifier is unique. */
		inline const uint32 getIdentifier() const;

		/** Textures contain tiles (grid order), they've got same width and height, each tile contains exactly one thing, e.g. a plant, a person...
		@return The returned object represents the size of each tile in the texture. tile size \in (0, 1]^2. */
		inline const Utilities::Size2<Real> & getTileSize() const;

	private:
		/** Constructor is private in order to avoid duplicated textures.
			To get access to a texture call the request function.
		@param relativeName Contains the relative name of the texture whereas the name should be relative to the texture resources folder. See Texture::msResourcePath.
		@param header Set header to NULL to load texture data from file.
					  Set this to a header data structure if you don't want to load data from file but initialize the texture to be created with some pixel data.
					  pixelData must not be NULL in this case.
		@param pixelData Set this to the data to initialize the texture with if you don't want to load data from file. Header must not be NULL in this case. */
		Texture(const std::string &relativeName, const TextureHeader *header, const uint8 *pixelData);

		/** Destroys the texture. That is, requested resources are freed. */
		virtual ~Texture();

		Utilities::Size2<Real> mTileSize;	/// textures contain tiles (in grid order), they've got same width and height, each tile contains exactly one thing
		uint32	mIdentifier;				///	identifier which is also used by the underlying graphics API to distinguish between the created textures

		/** Copy constructor is forbidden. */
		inline Texture(const Texture &copy);

		/** Assign operator is forbidden.  */
		inline Texture &operator =(const Texture &rhs);
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	inline Texture::Texture(const Texture &copy) : UserResource(""), mIdentifier(-1)
	{
		assert(false);
	}

	inline Texture &Texture::operator =(const Texture &rhs)
	{
		assert(false);
		return *this;
	}

	inline const uint32 Texture::getIdentifier() const
	{
		return mIdentifier;
	}

	inline const Utilities::Size2<Real> &Texture::getTileSize() const
	{
		return mTileSize;
	}
}

#endif // _TEXTURE_H_
