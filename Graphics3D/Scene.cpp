/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Scene.h"

using namespace Graphics;
using namespace Math;

Scene::Scene() :
	mRootSceneNode()
{
}

Scene::~Scene()
{
}

void Scene::render(const Math::Matrix4x4 &viewProjection, const Math::Matrix4x4 &world) const
{
	mRootSceneNode.render(viewProjection, world);
}
