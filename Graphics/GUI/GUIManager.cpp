/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "GUIManager.h"

using namespace Graphics::GUI;
using namespace Math;

GUIManager::GUIManager()
{
	// TODO: the depth of the main SubGUI object shouldn't be such a constant
	mSubGUIs.push_back(new SubGUI(-1, 1));																		// create the main SubGUI object


	mTestSprite = new Sprite("map", 0);																			// add a test sprite so that some GUI is always there
	mTestSprite->setScaling(Vector2(0.3333f, 0.3333f));
	mTestSprite->setPivot(Vector2(-0.5f, 0.5f));

	Real aspectRatio = mMainSubGUIViewport.getAspectRatio();
	mTestSprite->setPosition(Vector2(aspectRatio * ORTHO_PROJECTION_LEFT, ORTHO_PROJECTION_TOP));

	mSubGUIs[0]->add(*mTestSprite, 0);
}

GUIManager::~GUIManager()
{
	mSubGUIs[0]->remove(*mTestSprite, 0);																		// free test sprite
	delete mTestSprite;

	assert(1 == mSubGUIs.size());																				// free the main SubGUI object
	delete mSubGUIs[0];																							// which should be the only one that remains
}

SubGUI &GUIManager::createSubGUI(int32 identifier, uint32 numOfLayers)
{
	#ifdef _DEBUG																								// check that ID is new
		uint32 number = mSubGUIs.size();
		for(uint32 i = 0; i < number; ++i)
			assert(mSubGUIs[i]->getID() != identifier);
	#endif // _DEBUG

	SubGUI *newSubGUI = new SubGUI(identifier, numOfLayers);													// create and return sub GUI
	mSubGUIs.push_back(newSubGUI);
	return *newSubGUI;
}

void GUIManager::deleteSubGUI(int32 identifier)
{
	uint32 number = mSubGUIs.size();
	for(uint32 i = 1; i < number; ++i)																			// go through all sub GUIs
	{																											//  except the general sub GUI which
		if (mSubGUIs[i]->getID() == identifier)																	//  cannot be manually created or destroyed
		{
			delete mSubGUIs[i];
			mSubGUIs[i] = mSubGUIs.back();
			mSubGUIs.pop_back();
			return;
		}
	}

	assert(false);
}

SubGUI &GUIManager::getSubGUI(int32 identifier)
{
	uint32 number = mSubGUIs.size();																			// find the requested sub GUI and return it
	for(uint32 i = 1; i < number; ++i)
		if (mSubGUIs[i]->getID() == identifier)
			return *mSubGUIs[i];

	assert(false);
	return *mSubGUIs[0];
}
			
void GUIManager::renderMainSubGUI()
{
	mMainSubGUIViewport.becomeActiveViewport();
	mSubGUIs[0]->render();
}

void GUIManager::update()
{
}
