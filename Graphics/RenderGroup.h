/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _RENDER_GROUP_H_
#define _RENDER_GROUP_H_

#include <cassert>
#include <vector>
#include "Platform/DataTypes.h"
#include "Graphics/IRenderable.h"

namespace Graphics
{
	/// An RenderGroup instance is a collection of IRenderable objects which are rendered together.
	/** Several RenderGroup objects containing IRenderable instances can be used to define a rendering order.
		This is a means to solve rendering dependency problems between IRenderable objects. */
	class RenderGroup
	{
	public:
		/** Creates an emtpy group. */
		RenderGroup();

		/** Destroys the group. However, it does not destroy its members! */
		~RenderGroup();

		/** Adds an IRenderable object to the group to be rendered with it.
		@param renderable The IRenderable object is rendered together with all other members when calling render() of the group. */
		void add(const IRenderable &renderable);

		/** Removes an IRenderable object from the group to stop rendering it.
		@param renderable The IRenderable object won't be rendered anymore when calling render() of the group. */
		void remove(const IRenderable &renderable);

		/** Render all members. There is no order guarantee between group members. */
		void render() const;

	protected:
		std::vector<const IRenderable *> mMembers; /// group members which are updated when the group update funciton is called

	private:
		/** Copy constructor is forbidden. */
		RenderGroup(const RenderGroup &copy) { assert(false); }

		/** Assign operator is forbidden. */
		RenderGroup &operator =(const RenderGroup &rhs) { assert(false); return *this; }
	};
}

#endif // _RENDER_GROUP_H_
