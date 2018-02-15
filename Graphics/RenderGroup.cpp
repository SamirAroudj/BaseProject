/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#include "Graphics/RenderGroup.h"
#include "Platform/Utilities/Array.h"

using namespace Graphics;
using namespace std;
using namespace Utilities;

RenderGroup::RenderGroup()
{
}

RenderGroup::~RenderGroup()
{
	assert(mMembers.empty());
}

void RenderGroup::add(const IRenderable &renderable)
{
	mMembers.push_back(&renderable);
}

void RenderGroup::remove(const IRenderable &renderable)
{
	const size_t idx = Array<const IRenderable *>::deleteFirstBySwapWithBack(mMembers, &renderable);
	assert(idx != ((size_t) -1));
}

void RenderGroup::render() const
{
	size_t numOfMembers = mMembers.size();
	for (size_t i = 0; i < numOfMembers; ++i)
		mMembers[i]->render();
}
