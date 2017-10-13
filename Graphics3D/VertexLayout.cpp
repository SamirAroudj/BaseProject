/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Platform/ExceptionHandling/GraphicsException.h"
#include "Graphics/GraphicsManager.h"
#include "Shader.h"
#include "Vertex.h"
#include "VertexLayout.h"

using namespace ExceptionHandling;
using namespace Graphics;
using namespace ResourceManagement;
using namespace std;

vector<VertexLayout *> *Resource<VertexLayout>::msResources = new vector<VertexLayout *>();

VertexLayout *VertexLayout::request(const std::string &vertexStructureName, const string &shaderName)
{
	VertexLayout *layout = UserResource<VertexLayout>::request(vertexStructureName);
	if (layout)
		return layout;

	layout = new VertexLayout(vertexStructureName, shaderName);
	return layout;
}

VertexLayout::VertexLayout(const std::string &vertexStructureName, const string &shaderName) :
	UserResource<VertexLayout>(vertexStructureName),
	mD3DVertexLayout(NULL),
	mVertexSize(0)
{
	D3D10_PASS_DESC passDescription;
	Shader *shader = Shader::request(shaderName);
	bool found = true;
	shader->getCurrentTechnique()->GetPassByIndex(0)->GetDesc(&passDescription);

	if ("POSITION_COLOR" == vertexStructureName)
		createPositionColorLayout(passDescription);
	else if ("POS_NOR_TEX" == vertexStructureName)
		createPosNorTexLayout(passDescription);
	else if ("POSITION_TEXCOORD" == vertexStructureName)
		createPositionTextureCoordinatesLayout(passDescription);
	else
		found = false;

	Shader::release(shader);
	assert(found);
}

void VertexLayout::createPositionColorLayout(const D3D10_PASS_DESC &passDescription)
{
	D3D10_INPUT_ELEMENT_DESC vertexDescription[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT result = GraphicsManager::getSingleton().getDevice()->CreateInputLayout(vertexDescription,
		2, passDescription.pIAInputSignature, passDescription.IAInputSignatureSize, &mD3DVertexLayout);

	if (S_OK != result)
		throw GraphicsException("Could not create a vertex layout: POSITION_COLOR", result);
	mVertexSize = sizeof(PositionColorVertex);
}

void VertexLayout::createPosNorTexLayout(const D3D10_PASS_DESC &passDescription)
{
	D3D10_INPUT_ELEMENT_DESC vertexDescription[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT result = GraphicsManager::getSingleton().getDevice()->CreateInputLayout(vertexDescription,
		3, passDescription.pIAInputSignature, passDescription.IAInputSignatureSize, &mD3DVertexLayout);
	if (S_OK != result)
		throw GraphicsException("Could not create a vertex layout: POS_NOR_TEX", result);
	mVertexSize = sizeof(PosNorTexVertex);
}

void VertexLayout::createPositionTextureCoordinatesLayout(const D3D10_PASS_DESC &passDescription)
{
	D3D10_INPUT_ELEMENT_DESC vertexDescription[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT result = GraphicsManager::getSingleton().getDevice()->CreateInputLayout(vertexDescription,
		2, passDescription.pIAInputSignature, passDescription.IAInputSignatureSize, &mD3DVertexLayout);

	if (S_OK != result)
		throw GraphicsException("Could not create a vertex layout: POSITION_TEXCOORD", result);
	mVertexSize = sizeof(PositionTexCoordVertex);
}

VertexLayout::~VertexLayout()
{
	mD3DVertexLayout->Release();
}



