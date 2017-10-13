/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _SCENE_H_
#define _SCENE_H_

#include "SceneNode.h"

namespace Graphics
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		SceneNode &getRootSceneNode() { return mRootSceneNode; }
		const SceneNode &getRootSceneNode() const { return mRootSceneNode; }

		void render(const Math::Matrix4x4 &viewProjection, const Math::Matrix4x4 &world = Math::Matrix4x4()) const;
	private:
		SceneNode mRootSceneNode;
	};
}

#endif // _SCENE_H_
