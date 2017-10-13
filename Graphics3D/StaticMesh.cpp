/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Color.h"
#include "Platform/File.h"
#include "MagicGraphicsConstants.h"
#include "MagicConstants.h"
#include "Platform/ExceptionHandling/GraphicsException.h"
#include "Graphics/GraphicsManager.h"
#include "StaticMesh.h"
#include "Vertex.h"

using namespace ExceptionHandling;
using namespace Graphics;
using namespace Math;
using namespace ResourceManagement;
using namespace Platform;
using namespace std;

vector<StaticMesh *> *Resource<StaticMesh>::msResources = new vector<StaticMesh *>();
const char *Resource<StaticMesh>::msPathToResources = PATH_TO_MESHES;

StaticMesh *StaticMesh::request(const string &shaderName, const string &name)
{
	StaticMesh *mesh = UserResource<StaticMesh>::request(name);
	if (mesh)
		return mesh;

	if ("Crate" == name)	// some hard coded meshes for testing
		return createCrate(shaderName, name);
	if ("Cube" == name)
		return createColoredCube(shaderName, name);
	if ("Floor" == name)
		return createColoredFloor(shaderName, name);
	if ("Pyramid" == name)
		return createColoredPyramid(shaderName, name);
	if ("TestRectangle" == name)
		return createTestRectangle(shaderName, name);

	return loadFromFile(shaderName, name);
}

StaticMesh *StaticMesh::loadFromFile(const string &shaderName, const string &name)
{
	char buffer[FILE_READING_BUFFER_SIZE];
	string fullName(msPathToResources);
	string vertexStructureName;
	uint32 numOfVertices;
	uint32 numOfIndices;
	uint16 nameLength = 0;
	VertexLayout *vertexLayout = NULL;
	StaticMesh *mesh = NULL;
	char *vertices = NULL;
	char *indices = NULL;

	fullName += name;
	File file(fullName, File::OPEN_READING, true);

	fread(&nameLength, sizeof(uint16), 1, &file.getHandle());	// retrieve vertex structure name and vertex layout
	fread(buffer, nameLength, 1, &file.getHandle());
	buffer[nameLength] = '\0';
	vertexStructureName.assign(buffer, nameLength);
	vertexLayout = VertexLayout::request(vertexStructureName, shaderName);
	
	fread(&numOfVertices, sizeof(uint32), 1, &file.getHandle());	// create buffers to store vertices and indices
	fread(&numOfIndices, sizeof(uint32), 1, &file.getHandle());
	vertices = new char[numOfVertices * vertexLayout->getVertexSize()];
	indices = new char[numOfIndices * sizeof(uint32)];

	fread(vertices, numOfVertices * vertexLayout->getVertexSize(), 1, &file.getHandle());
	fread(indices, numOfIndices * sizeof(uint32), 1, &file.getHandle());

	mesh = new StaticMesh(name, vertexLayout, vertices, numOfVertices, reinterpret_cast<uint32 *>(indices), numOfIndices);

	delete [] vertices;
	delete [] indices;

	return mesh;
}

StaticMesh *StaticMesh::createColoredCube(const string &shaderName, const string &name)
{
	const unsigned int numOfVertices = 8;
	const unsigned int numOfIndices = 36;

	PositionColorVertex vertices[] =
	{
		{ Vector3(-0.5f, -0.5f, -0.5f), BLACK },	// bottom
		{ Vector3(-0.5f, -0.5f,  0.5f), WHITE },
		{ Vector3( 0.5f, -0.5f, -0.5f), RED },
		{ Vector3( 0.5f, -0.5f,  0.5f), GREEN },
		{ Vector3(-0.5f,  0.5f, -0.5f), MAGENTA },	// top
		{ Vector3(-0.5f,  0.5f,  0.5f), CYAN },
		{ Vector3( 0.5f,  0.5f, -0.5f), YELLOW },
		{ Vector3( 0.5f,  0.5f,  0.5f), BLUE }
	};

	uint32 indices[] = 
	{
		2, 1, 0,	1, 2, 3, // bottom faces
		0, 1, 5,	5, 4, 0, // left faces
		2, 6, 7,	7, 3, 2, // right faces
		0, 4, 6,	6, 2, 0, // back faces
		7, 5, 1,	1, 3, 7, // front faces
		5, 6, 4,	6, 5, 7, // top faces
	};

	return createStaticMesh(name, "POSITION_COLOR", shaderName, vertices, numOfVertices, indices, numOfIndices);
}

StaticMesh *StaticMesh::createColoredFloor(const string &shaderName, const string &name)
{
	const unsigned int numOfVertices = 4;
	const unsigned int numOfIndices = 6;
	PositionColorVertex vertices[] =
	{
		{ Vector3( 100.0f, 0.0f,  100.0f),	BLUE },
		{ Vector3(-100.0f, 0.0f,  100.0f),	RED },
		{ Vector3(-100.0f, 0.0f, -100.0f),	Color(0.3f, 0.7f, 0.2f, 1.0f) },
		{ Vector3( 100.0f, 0.0f, -100.0f),	GREEN }
	};
	uint32 indices[] = { 0, 2, 1, 2, 0, 3 };

	return createStaticMesh(name, "POSITION_COLOR", shaderName, vertices, numOfVertices, indices, numOfIndices);
}

StaticMesh *StaticMesh::createColoredHeightMap(const string &shaderName, const string &name,
											   Real width, Real depth, uint32 numOfRowVertices, uint32 numOfColumnVertices,
											   Real (*heightFunction)(Real width, Real depth))
{
	StaticMesh *mesh = UserResource<StaticMesh>::request(name);
	if (mesh)
		return mesh;

	assert(numOfRowVertices > 1 && numOfColumnVertices > 1);
	const uint32 numOfVertices = numOfRowVertices * numOfColumnVertices;
	const uint32 numOfIndices = 6 * (numOfRowVertices - 1) * (numOfColumnVertices - 1);
	PositionColorVertex *vertices = reinterpret_cast<PositionColorVertex *>(new char[sizeof(PositionColorVertex) * numOfVertices]);
	uint32 *indices = new uint32[numOfIndices];


	Real dx = width / (numOfRowVertices - 1);	// strides
	Real dz = depth / (numOfColumnVertices - 1);

	for(uint32 i = 0; i < numOfRowVertices; ++i)
	{
		Real x = -0.5f * width + i * dx;
		for(uint32 j = 0; j < numOfColumnVertices; ++j)
		{
			Real height = 0.0f;
			Real z = 0.5f * depth - j * dz;	// set vertex values
			uint32 currentVertex = i * numOfColumnVertices + j;
			vertices[currentVertex].mPosition.x = x;
			vertices[currentVertex].mPosition.y = height = heightFunction(x, z);
			vertices[currentVertex].mPosition.z = z;

			if (height < -8.0f)
				vertices[currentVertex].mColor = GREY_BEACH;
			else if (height < 1.0f)
				vertices[currentVertex].mColor = DRY_GRASS;
			else if (height < 14.0f)
				vertices[currentVertex].mColor = DARK_GREEN;
			else if (height < 35.0f)
				vertices[currentVertex].mColor = ROCK;
			else
				vertices[currentVertex].mColor = WHITE;
		}
	}

	uint32 k = 0;
	for(uint32 i = 0; i < numOfRowVertices - 1; ++i)	// compute indices to define triangles
	{
		for(uint32 j = 0; j < numOfColumnVertices - 1; ++j)
		{
			indices[k] = i * numOfColumnVertices + j;	// first quad triangle
			indices[k + 1] = (i + 1) * numOfColumnVertices + j;
			indices[k + 2] = i * numOfColumnVertices + j + 1;

			
			indices[k + 3] = i * numOfColumnVertices + j + 1;	// second quad triangle
			indices[k + 4] = (i + 1) * numOfColumnVertices + j;
			indices[k + 5] = (i + 1) * numOfColumnVertices + j + 1;

			k += 6;
		}
	}

	mesh = createStaticMesh(name, "POSITION_COLOR", shaderName, vertices, numOfVertices, indices, numOfIndices);

	delete [] vertices;
	delete [] indices;
	return mesh;
}

StaticMesh *StaticMesh::createColoredPyramid(const string &shaderName, const string &name)
{
	const unsigned int numOfVertices = 5;
	const unsigned int numOfIndices = 18;

	PositionColorVertex vertices[] =
	{
		{ Vector3(-0.5f, -0.5f, -0.5f), YELLOW },	// bottom
		{ Vector3(-0.5f, -0.5f,  0.5f), YELLOW },
		{ Vector3( 0.5f, -0.5f,  0.5f), YELLOW },
		{ Vector3( 0.5f, -0.5f, -0.5f), YELLOW },
		{ Vector3( 0.0f,  0.5f,  0.0f), RED }	// top
	};

	uint32 indices[] = 
	{
		2, 1, 0,	0, 3, 2, // bottom faces
		0, 1, 4,	// left face
		1, 2, 4,	// right face
		2, 3, 4,	// back face
		3, 0, 4,	// front face
	};

	return createStaticMesh(name, "POSITION_COLOR", shaderName, vertices, numOfVertices, indices, numOfIndices);
}

StaticMesh *StaticMesh::createCrate(const std::string &shaderName, const std::string &name)
{
	const unsigned int numOfVertices = 12;
	const unsigned int numOfIndices = 36;

	PositionTexCoordVertex vertices[] =
	{
		{ Vector3(-0.5f, -0.5f, -0.5f), Vector2(0.0f, 0.0f) },	// bottom
		{ Vector3(-0.5f, -0.5f,  0.5f), Vector2(0.0f, 1.0f) },
		{ Vector3( 0.5f, -0.5f, -0.5f), Vector2(1.0f, 0.0f) },
		{ Vector3( 0.5f, -0.5f,  0.5f), Vector2(1.0f, 1.0f) },
		{ Vector3(-0.5f,  0.5f, -0.5f), Vector2(0.0f, 1.0f) },	// top
		{ Vector3(-0.5f,  0.5f,  0.5f), Vector2(0.0f, 0.0f) },
		{ Vector3( 0.5f,  0.5f, -0.5f), Vector2(1.0f, 1.0f) },
		{ Vector3( 0.5f,  0.5f,  0.5f), Vector2(1.0f, 0.0f) },
		{ Vector3(-0.5f, -0.5f,  0.5f), Vector2(1.0f, 0.0f) },	// 4 extra due to texture mapping
		{ Vector3(-0.5f,  0.5f,  0.5f), Vector2(1.0f, 1.0f) },
		{ Vector3( 0.5f,  0.5f,  0.5f), Vector2(0.0f, 1.0f) },
		{ Vector3( 0.5f,  0.5f, -0.5f), Vector2(0.0f, 0.0f) }
	};

	uint32 indices[] = 
	{
		2, 1, 0,	1, 2, 3, // bottom faces
		9, 4, 0,	0, 8, 9, // left faces
		2, 11, 10,	10, 3, 2, // right faces
		0, 4, 6,	6, 2, 0, // back faces
		7, 5, 1,	1, 3, 7, // front faces
		5, 6, 4,	6, 5, 7, // top faces
	};

	return createStaticMesh(name, "POSITION_TEXCOORD", shaderName, vertices, numOfVertices, indices, numOfIndices);
}

StaticMesh *StaticMesh::createTestRectangle(const std::string &shaderName, const std::string &name)
{
	const unsigned int numOfVertices = 4;
	const unsigned int numOfIndices = 6;

	/*	{ Vector3( 100.0f, 0.0f,  100.0f),	BLUE },
		{ Vector3(-100.0f, 0.0f,  100.0f),	RED },
		{ Vector3(-100.0f, 0.0f, -100.0f),	Color(0.3f, 0.7f, 0.2f, 1.0f) },
		{ Vector3( 100.0f, 0.0f, -100.0f),	GREEN }*/

	PosNorTexVertex vertices[] =
	{
		{ Vector3( 1.0f, 0.0f,  0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f) },
		{ Vector3(-1.0f, 0.0f,  0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f) },
		{ Vector3(-1.0f, 0.0f, -0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f) },
		{ Vector3( 1.0f, 0.0f, -0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f) }
	};
	uint32 indices[] = { 0, 2, 1, 2, 0, 3 };

	return createStaticMesh(name, "POS_NOR_TEX", shaderName, vertices, numOfVertices, indices, numOfIndices);
}

StaticMesh *StaticMesh::createStaticMesh(const string &name, const string &vertexStructureName, const string &shaderName,
										 void *vertices, uint32 numOfVertices, uint32 *indices, uint32 numOfIndices)
{
	StaticMesh *mesh = UserResource<StaticMesh>::request(name);
	if (mesh)
		return mesh;

	mesh = new StaticMesh(name, VertexLayout::request(vertexStructureName, shaderName), vertices, numOfVertices, indices, numOfIndices);
	return mesh;
}

void StaticMesh::render() const
{
	uint32 vertexBufferOffset = 0;
	uint32 indexBufferOffset = 0;
	uint32 stride = mVertexLayout->getVertexSize();

	GraphicsManager::getSingleton().setVertexLayout(mVertexLayout);
	GraphicsManager::getSingleton().getDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    GraphicsManager::getSingleton().getDevice()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &vertexBufferOffset);
	GraphicsManager::getSingleton().getDevice()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, indexBufferOffset);
	GraphicsManager::getSingleton().getDevice()->DrawIndexed(mNumOfIndices, 0, 0);
}

StaticMesh::StaticMesh(const string &name, VertexLayout *vertexLayout, void *vertices, uint32 numOfVertices, uint32 *indices, uint32 numOfIndices) :
	UserResource<StaticMesh>(name),
	mIndexBuffer(NULL),
	mVertexBuffer(NULL),
	mVertexLayout(vertexLayout),
	mNumOfIndices(numOfIndices)
{
	D3D10_BUFFER_DESC vertexBufferDescription;	// immutable = static mesh with numOfVertices vertices
	vertexBufferDescription.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDescription.ByteWidth = numOfVertices * vertexLayout->getVertexSize();
	vertexBufferDescription.CPUAccessFlags = 0;
	vertexBufferDescription.MiscFlags = 0;
	vertexBufferDescription.Usage = D3D10_USAGE_IMMUTABLE;
	D3D10_SUBRESOURCE_DATA initialVertices;
	initialVertices.pSysMem = vertices;

	HRESULT result = GraphicsManager::getSingleton().getDevice()->CreateBuffer(&vertexBufferDescription, &initialVertices, &mVertexBuffer);
    if (S_OK != result)
		throw GraphicsException("Could not create a vertex buffer for a custom static mesh.", result);

	D3D10_BUFFER_DESC indexBufferDescription;	// immutable = static mesh with numOfIndices inidces
	indexBufferDescription.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDescription.ByteWidth = sizeof(uint32) * numOfIndices;
	indexBufferDescription.CPUAccessFlags = 0;
	indexBufferDescription.MiscFlags = 0;
	indexBufferDescription.Usage = D3D10_USAGE_IMMUTABLE;
	D3D10_SUBRESOURCE_DATA initialIndices;
	initialIndices.pSysMem = indices;

	result = GraphicsManager::getSingleton().getDevice()->CreateBuffer(&indexBufferDescription, &initialIndices, &mIndexBuffer);
    if (S_OK != result)
		throw GraphicsException("Could not create a index buffer for a custom static mesh.", result);
}

StaticMesh::~StaticMesh()
{
	mIndexBuffer->Release();
	mVertexBuffer->Release();
	VertexLayout::release(mVertexLayout);
}
