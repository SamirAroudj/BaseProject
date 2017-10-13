/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _STATIC_MESH_H_
#define _STATIC_MESH_H_

#include "Platform/ResourceManagement/UserResource.h"
#include "VertexLayout.h"

namespace Graphics
{
	class Shader;

	class StaticMesh : public ResourceManagement::UserResource<StaticMesh>
	{
	friend class ResourceManagement::UserResource<StaticMesh>;

	public:
		static StaticMesh *createColoredCube(const std::string &shaderName, const std::string &name);
		static StaticMesh *createColoredFloor(const std::string &shaderName, const std::string &name);
		static StaticMesh *createColoredHeightMap(const std::string &shaderName, const std::string &name,
											   Real width, Real depth, uint32 numOfRowVertices, uint32 numOfColumnVertices,
											   Real (*heightFunction)(Real width, Real depth));
		static StaticMesh *createColoredPyramid(const std::string &shaderName, const std::string &name);
		static StaticMesh *createCrate(const std::string &shaderName, const std::string &name);
		static StaticMesh *createStaticMesh(const std::string &name, const std::string &vertexStructureName, const std::string &shaderName,
			void *vertices, uint32 numOfVertices, uint32 *indices, uint32 numOfIndices);
		// creates a rectangle with PosNorTex vertices
		static StaticMesh *createTestRectangle(const std::string &shaderName, const std::string &name);

		void render() const;
		static StaticMesh *request(const std::string &shaderName, const std::string &name);

	private:
		StaticMesh(const std::string &name, VertexLayout *vertexLayout, void *vertices, uint32 numOfVertices, uint32 *indices, uint32 numOfIndices);
		virtual ~StaticMesh();
		static StaticMesh *loadFromFile(const std::string &shaderName, const std::string &name);

		ID3D10Buffer *mIndexBuffer;
		ID3D10Buffer *mVertexBuffer;
		VertexLayout *mVertexLayout;
		uint32 mNumOfIndices;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copy constructor is forbidden. </summary>
		///
		/// <remarks>	Samir, 03.12.2011. </remarks>
		///
		/// <param name="copy">	Copy constructor is forbidden. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		StaticMesh(const StaticMesh &copy) : 
			ResourceManagement::UserResource<StaticMesh>("")
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
		StaticMesh &operator =(const StaticMesh &rhs) { assert(false); return *this; }
	};
}

#endif // _STATIC_MESH_H_