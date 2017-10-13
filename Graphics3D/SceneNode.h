/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _SCENE_NODE_H_
#define _SCENE_NODE_H_

#include <vector>
#include "Platform/DataTypes.h"
#include "Graphics/IRenderable.h"
#include "Math/Matrix4x4.h"
#include "Math/Quaternion.h"

namespace Graphics
{
	class SceneNode
	{
	public:
		enum TransformSpace { LOCAL, PARENT, WORLD };
		SceneNode();
		~SceneNode();

		void addChild(SceneNode *child);
		void addRenderable(IRenderable *renderable);
		void attachTo(SceneNode *parent);
		void detach();
		void detachChild(SceneNode *child);

		// only works with directly added renderables
		void removeRenderable(IRenderable *renderable);

		void render(const Math::Matrix4x4 &viewProjection, const Math::Matrix4x4 &parentTransform) const;

		SceneNode &getChild(uint32 number) { return *mChildren[number]; }
		const SceneNode &getChild(uint32 number) const { return *mChildren[number]; }
		uint32 getNumOfChildren() const { return mChildren.size(); }
		const Math::Quaternion &getOrientation() const { return mOrientation; }
		const Math::Vector3 &getPosition() const { return mPosition; }
		const Math::Vector3 &getScale() const { return mScale; }

		void scale(const Math::Vector3 &scale);
		void setOrientation(const Math::Quaternion &orientation);
		void setPosition(const Math::Vector3 &position);
		void setScale(const Math::Vector3 &scale);
		void translate(const Math::Vector3 &delta, TransformSpace relativeTo = LOCAL);
		void rotate(const Math::Quaternion &rotation, TransformSpace relativeTo = LOCAL);

	private:
		void computeCachedTransformation();

		std::vector<IRenderable *> mRenderables;
		std::vector<SceneNode *> mChildren;
		Math::Matrix4x4 mCachedTransformation;
		Math::Quaternion mOrientation;
		Math::Vector3 mPosition;
		Math::Vector3 mScale;
		SceneNode *mParent;
	};
}

#endif // _SCENE_NODE_H_
