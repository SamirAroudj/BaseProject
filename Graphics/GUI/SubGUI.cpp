/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
	#include <Windows.h>
#endif // _WINDOWS
#include <gl/gl.h>

#include "../MagicConstants.h"
#include "SubGUI.h"
#include "../Viewport.h"

using namespace Graphics;
using namespace Graphics::GUI;

SubGUI::SubGUI(int32 identifier, uint32 numOfRenderGroups) : mIdentifier(identifier)
{
	for(uint32 i = 0; i < numOfRenderGroups; ++i)
		mRenderGroups.push_back(new RenderGroup());
}


SubGUI::~SubGUI()
{
	uint32 numOfRenderGroups = mRenderGroups.size();
	for(uint32 i = 0; i < numOfRenderGroups; ++i)
		delete mRenderGroups[i];
}

void SubGUI::add(const IRenderable &renderable, uint32 renderGroup)
{
	assert(renderGroup < mRenderGroups.size());
	mRenderGroups[renderGroup]->add(renderable);
}
			
void SubGUI::remove(const IRenderable &renderable, uint32 renderGroup)
{
	assert(renderGroup < mRenderGroups.size());
	mRenderGroups[renderGroup]->remove(renderable);
}

void SubGUI::render() const
{
	glMatrixMode(GL_MODELVIEW);																		// set model view and projection matrix and disable depth tests
	glLoadIdentity(); 

	Real aspectRatio = Viewport::getActiveViewport().getAspectRatio();
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(ORTHO_PROJECTION_LEFT * aspectRatio, ORTHO_PROJECTION_RIGHT * aspectRatio,
			ORTHO_PROJECTION_BOTTOM,			 ORTHO_PROJECTION_TOP,
			-ORTHO_PROJECTION_Z_NEAR, -ORTHO_PROJECTION_Z_FAR);

	glDisable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);
	
	uint32 numOfRenderGroups = mRenderGroups.size();												// render each render group whereas 0 is the lowest layer
	for(uint32 i = 0; i < numOfRenderGroups; ++i)
		mRenderGroups[i]->render();
}