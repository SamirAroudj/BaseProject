/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Math/MathHelper.h"
#include "SceneNode.h"

using namespace Graphics;
using namespace Math;

SceneNode::SceneNode() :
		mOrientation(0.0f, 0.0f, 0.0f, 1.0f),
		mPosition(0.0f, 0.0f, 0.0f),
		mScale(1.0f, 1.0f, 1.0f),
		mParent(NULL)
{

}

SceneNode::~SceneNode()
{
	detach();
	while(mChildren.size() > 0)
		delete mChildren[0];
}

void SceneNode::addChild(SceneNode *child)
{
	assert(child != NULL);
	mChildren.push_back(child);
	child->mParent = this;
}

void SceneNode::addRenderable(IRenderable *renderable)
{
	mRenderables.push_back(renderable);
}

void SceneNode::attachTo(SceneNode *parent)
{
	assert(!mParent);
	parent->addChild(this);
}

void SceneNode::detach()
{
	if (!mParent)
		return;

	mParent->detachChild(this);
	mParent = NULL;
}

void SceneNode::detachChild(SceneNode *child)
{
	uint32 numOfChildren = mChildren.size();
	for(uint32 i = 0; i < numOfChildren; ++i)
	{
		if (child == mChildren[i])
		{
			child->mParent = NULL;
			mChildren[i] = mChildren.back();
			mChildren.pop_back();
			return;
		}
	}
}

void SceneNode::removeRenderable(IRenderable *renderable)
{
	uint32 numOfRenderables = mRenderables.size();
	for(uint32 i = 0; i < numOfRenderables; ++i)
	{
		if (mRenderables[i] == renderable)
		{
			mRenderables[i] = mRenderables.back();
			mRenderables.pop_back();
			return;
		}
	}
	assert(false);
}

void SceneNode::render(const Math::Matrix4x4 &viewProjection, const Math::Matrix4x4 &parentTransform) const
{
	Matrix4x4 transformation(mCachedTransformation * parentTransform);

	uint32 numOfChildren = mChildren.size();
	for(uint32 i = 0; i < numOfChildren; ++i)
		mChildren[i]->render(viewProjection, transformation);

	
	uint32 numOfRenderables = mRenderables.size();
	if (0 == numOfRenderables)
		return;

	Matrix4x4 worldViewProjection(transformation * viewProjection);
	for(uint32 i = 0; i < numOfRenderables; ++i)
		mRenderables[i]->render(worldViewProjection, transformation);
}

void SceneNode::scale(const Math::Vector3 &scale)
{
	mScale += scale;
	computeCachedTransformation();
}

void SceneNode::setOrientation(const Math::Quaternion &orientation)
{
	mOrientation = orientation;
	computeCachedTransformation();
}

void SceneNode::setPosition(const Math::Vector3 &position)
{
	mPosition = position;
	computeCachedTransformation();
}

void SceneNode::setScale(const Math::Vector3 &scale)
{
	mScale = scale;
	computeCachedTransformation();
}

void SceneNode::translate(const Math::Vector3 &delta, TransformSpace relativeTo)
{
	switch(relativeTo)
	{
	case SceneNode::LOCAL:
		mPosition += delta * mCachedTransformation;
		break;

	case SceneNode::PARENT:
		mPosition += delta;
		break;

	case SceneNode::WORLD:
		assert(false);
		break;

	default:
		assert(false);
		break;
	}

	computeCachedTransformation();
}

void SceneNode::rotate(const Math::Quaternion &rotation, TransformSpace relativeTo)
{
	switch(relativeTo)
	{
	case SceneNode::LOCAL:
		mOrientation = rotation * mOrientation;	// intrinsic form
		break;

	case SceneNode::PARENT:
		mOrientation = mOrientation * rotation;	// extrinsic form
		rotation.rotateVector(mPosition, mPosition);
		break;

	case SceneNode::WORLD:
		assert(false);
		break;

	default:
		assert(false);
		break;
	}

	computeCachedTransformation();
}


void SceneNode::computeCachedTransformation()
{
	mCachedTransformation.setToIdentity();	// scale is applied first
	mCachedTransformation._11 = mScale.x;
	mCachedTransformation._22 = mScale.y;
	mCachedTransformation._33 = mScale.z;

	mOrientation.normalize();
	Matrix4x4 rotation = Math::createMatrix4x4FromQuaternion(mOrientation);	// secondly, orientation is applied
	Matrix4x4 temp = mCachedTransformation * rotation;

	mCachedTransformation._11 = mCachedTransformation._22 = mCachedTransformation._33 = 1.0f;	// translation is the last transformation applied
	mCachedTransformation._41 = mPosition.x;
	mCachedTransformation._42 = mPosition.y;
	mCachedTransformation._43 = mPosition.z;
	mCachedTransformation = temp * mCachedTransformation;
}