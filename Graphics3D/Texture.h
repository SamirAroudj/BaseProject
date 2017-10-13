/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "d3dx10.h"
#include "Platform/ResourceManagement/UserResource.h"

namespace Graphics
{
	class Texture : public ResourceManagement::UserResource<Texture>
	{
	friend class ResourceManagement::UserResource<Texture>;

	public:
		ID3D10ShaderResourceView *getShaderResourceView() const { return mTextureView; }

		static Texture *request(const std::string &name);

	private:
		Texture(const std::string &name);
		virtual ~Texture();

		ID3D10ShaderResourceView *mTextureView;

		// forbidden
		Texture(const Texture &copy) : ResourceManagement::UserResource<Texture>("")
		{
			assert(false);
		}

		// forbidden
		Texture &operator =(const Texture &rhs)
		{
			assert(false);
			return *this;
		}
	};
}

#endif // _TEXTURE_H_
