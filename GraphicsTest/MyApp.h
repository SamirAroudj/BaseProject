/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _MY_APP_H_
#define _MY_APP_H_

#include "Graphics/Camera2D.h"
#include "Graphics/Sprite.h"
#include "Platform/Application.h"

class MyApp : public Platform::Application
{
public:
#ifdef _WINDOWS
	MyApp(HINSTANCE instanceHandle);
#elif _LINUX
	MyApp();
#endif // _WINDOWS
	virtual ~MyApp();

	virtual void onActivation() { Platform::Application::onActivation(); }
	virtual void onDeactivation() { Platform::Application::onDeactivation(); }

protected:
	virtual void postRender() { }
	virtual void render();
	virtual void update();

private:
	Graphics::Camera2D	*mCamera;
	Graphics::Sprite	**mSprites;
	uint32				mNumOfSprites;
};

#endif // _MY_APP_H_
