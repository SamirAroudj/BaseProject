/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "StaticObject.h"

using namespace Graphics;
using namespace std;

StaticObject::StaticObject(const string &materialName, const string &staticMeshName) :
	mMaterial(Material::request(materialName)),
	mStaticMesh(StaticMesh::request(mMaterial->getShader().getName(), staticMeshName))
{
}

	
StaticObject::StaticObject(const string &materialName, const string &staticMeshName,
							Real width, Real depth, uint32 numOfRowVertices, uint32 numOfColumnVertices,
							Real (*heightFunction)(Real width, Real depth)) :
	mMaterial(Material::request(materialName)),
	mStaticMesh(NULL)
{
	mStaticMesh = StaticMesh::createColoredHeightMap(mMaterial->getShader().getName(), staticMeshName,
		width, depth, numOfRowVertices, numOfColumnVertices, heightFunction);
}

StaticObject::StaticObject(const StaticObject &copy) :
	mMaterial(Material::request(copy.mMaterial->getName())),
	mStaticMesh(StaticMesh::request(mMaterial->getShader().getName(), copy.mStaticMesh->getName()))
{

}

StaticObject::~StaticObject()
{
	Material::release(mMaterial);
	StaticMesh::release(mStaticMesh);
}

StaticObject &StaticObject::operator =(const StaticObject &rhs)
{
	if (&rhs == this)
		return *this;

	Material::release(mMaterial);
	StaticMesh::release(mStaticMesh);

	mMaterial = Material::request(rhs.mMaterial->getName());
	mStaticMesh = StaticMesh::request(mMaterial->getShader().getName(), rhs.mStaticMesh->getName());

	return *this;
}

void StaticObject::render(const Math::Matrix4x4 &worldViewProjection, const Math::Matrix4x4 &world)
{
	mMaterial->prepareShaderForRendering(worldViewProjection, world);
	mMaterial->getShader().render(*mStaticMesh);
}