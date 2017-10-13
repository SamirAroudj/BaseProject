/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _VERTEX_LAYOUT_H_
#define _VERTEX_LAYOUT_H_

#include <vector>
#include "d3dx10.h"
#include "Platform/ResourceManagement/UserResource.h"

namespace Graphics
{
	class Shader;

	class VertexLayout : public ResourceManagement::UserResource<VertexLayout>
	{
	friend class ResourceManagement::UserResource<VertexLayout>;

	public:
		ID3D10InputLayout *getD3DVertexLayout() const { return mD3DVertexLayout; }
		uint32 getVertexSize() const { return mVertexSize; }

		static VertexLayout *request(const std::string &vertexStructureName, const std::string &shaderName);

	private:
		VertexLayout(const std::string &vertexStructureName, const std::string &shaderName);
		virtual ~VertexLayout();
		void createPositionColorLayout(const D3D10_PASS_DESC &passDescription);
		void createPosNorTexLayout(const D3D10_PASS_DESC &passDescription);
		void createPositionTextureCoordinatesLayout(const D3D10_PASS_DESC &passDescription);

		ID3D10InputLayout *mD3DVertexLayout; 
		uint32 mVertexSize;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copy constructor is forbidden. </summary>
		///
		/// <remarks>	Samir, 03.12.2011. </remarks>
		///
		/// <param name="copy">	Copy constructor is forbidden. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VertexLayout(const VertexLayout &copy) : 
			ResourceManagement::UserResource<VertexLayout>("")
		{
			assert(false);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assignment operator is forbidden. </summary>
		///
		/// <remarks>	Samir, 03.12.2011. </remarks>
		///
		/// <param name="rhs">	Assignment operator is forbidden. </param>
		///
		/// <returns>	Assignment operator is forbidden. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VertexLayout &operator =(const VertexLayout &rhs) { assert(false); return *this; }
	};
}

#endif // _VERTEX_LAYOUT_H_
