/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Graphics/AnimatedSprite.h"
#include "Graphics/GraphicsManager.h"
#include "Graphics/ImageManager.h"
#include "Platform/Input/InputManager.h"
#include "Platform/Timing/ApplicationTimer.h"
#include "Math/MathCore.h"
#include "Math/MathHelper.h"
#include "MyApp.h"

using namespace Graphics;
using namespace Input;
using namespace Math;
using namespace Platform;
using namespace std;
using namespace Timing;
using namespace Utilities;

#ifdef _WINDOWS
	MyApp::MyApp(HINSTANCE instanceHandle) : Platform::Application(instanceHandle),
#elif _LINUX
	MyApp::MyApp() : Platform::Application(),
#endif // _WINDOWS
	mCamera(NULL), mSprites(NULL), mNumOfSprites(0)
{
	// only create rendering stuff if there is a window
	if (!Platform::Window::exists())
	{
		mRunning = false;
		return;
	}

	// create managers
	uint32 colorResolution = 32;
	GraphicsManager *manager = new GraphicsManager(colorResolution);
	manager->enableAlphaBlending();

	ImageManager *imageManager = new ImageManager();

	mCamera = new Camera2D();

	mSprites = NULL;
	mNumOfSprites = 0;

	// opzen a png image
	const string fileName = "Data/undist-L3.png";
	ImgSize size;
	Texture::Format format;
	uint8 *pixels = imageManager->loadPNG(size, format, fileName, false);

	// clean up pixels
	delete [] pixels;
	pixels = NULL;

	//	// convert images
	//	imageManager->convertToTileTexture(DATA_DIRECTORY "Textures/fire.png", DATA_DIRECTORY "Textures/fire", 1.0f, 1.0f);
	//	imageManager->convertToTileTexture(DATA_DIRECTORY "Textures/bricks.png", DATA_DIRECTORY "Textures/bricks", 1.0f, 1.0f);
	//	imageManager->convertToTileTexture(DATA_DIRECTORY "Textures/symbol256.png", DATA_DIRECTORY "Textures/symbol256", 1.0f, 1.0f);
	//	imageManager->convertToTileTexture(DATA_DIRECTORY "Textures/symbol256.png", DATA_DIRECTORY "Textures/testAnim", 0.1f, 0.1f);

		// create some test sprites
	//	mNumOfSprites = 4;
	//	mSprites = new Sprite*[mNumOfSprites];
	//	mSprites[0] = new Sprite("symbol256");
	//	GraphicsManager::getSingleton().getRenderGroup(0).add(*(mSprites[0]));
	//	mSprites[1] = new Sprite("bricks");
	//	GraphicsManager::getSingleton().getRenderGroup(0).add(*(mSprites[1]));
	//	mSprites[2] = new AnimatedSprite("testAnim", 100);
	//	GraphicsManager::getSingleton().getRenderGroup(0).add(*(mSprites[2]));
	//	mSprites[3] = new Sprite("fire");
	//	GraphicsManager::getSingleton().getRenderGroup(0).add(*(mSprites[3]));

	//	// transform the created sprites
	//	mSprites[0]->setScaling(Vector2(2.0f, 0.5f));
	//	mSprites[0]->setPivot(Vector2(0.0f, -0.5f));
	//	mSprites[0]->setPosition(Vector2(1.0f, 1.0f));
	//	mSprites[0]->setDepth(0.3f);
	//	mSprites[0]->setAngle(-Math::QUARTER_PI);

	//	mSprites[1]->setDepth(0.5f);

	//	mSprites[1]->setScaling(Vector2(2.0f * 8.0f / 3.0f, 4.0f));

	//	mSprites[2]->setPosition(Vector2(-1.0f, 0.0f));
	//	mSprites[3]->setPosition(Vector2(-1.0f, 1.0f));
}

MyApp::~MyApp()
{
	delete mCamera;

	// free resources
	Texture::freeMemory();

	// free sprites
	for(uint32 i = 0; i < mNumOfSprites; ++i)
	{
		GraphicsManager::getSingleton().getRenderGroup(0).remove(*(mSprites[i]));
		delete mSprites[i];
	}
	delete [] mSprites;

	// free managers
	if (ImageManager::exists())
		delete ImageManager::getSingletonPointer();
	if (GraphicsManager::exists())
		delete GraphicsManager::getSingletonPointer();
}

void MyApp::render()
{
	GraphicsManager::getSingleton().clearBackAndDepthStencilBuffer();

	 // apply view matrix
	Matrix4x4 view = mCamera->computeViewMatrix();
	glPushMatrix();
	glMultMatrixr(view.getData());

	// render stuff
	for(uint32 i = 0; i < mNumOfSprites; ++i)
		mSprites[i]->render();

	// remove view matrix
	glPopMatrix();

	GraphicsManager::getSingleton().presentBackBuffer();
}

void MyApp::update()
{
	// variable definitions																
	Vector2	relativeCameraMovement;
	Real deltaTime = ApplicationTimer::getSingleton().getDeltaTime();
	Real cameraRotation	= 0.0f;	
	const Mouse &mouse = InputManager::getSingleton().getMouse();
	const Keyboard &keyboard = InputManager::getSingleton().getKeyboard();

	// quit application?
	if (keyboard.isKeyDown(KEY_ESCAPE))																
	{
		mRunning = false;
		return;
	}

	// move camera
	if (keyboard.isKeyDown(KEY_S))																	
		relativeCameraMovement.y -= 1.0f;
	if (keyboard.isKeyDown(KEY_W))
		relativeCameraMovement.y += 1.0f;
	if (keyboard.isKeyDown(KEY_A))
		relativeCameraMovement.x -= 1.0f;
	if (keyboard.isKeyDown(KEY_D))
		relativeCameraMovement.x += 1.0f;

	relativeCameraMovement.scale(deltaTime);
	mCamera->moveSidewards(relativeCameraMovement.x);
	mCamera->moveUpwards(relativeCameraMovement.y);
	
	// rotate camera
	if (keyboard.isKeyDown(KEY_Q))																	
		cameraRotation += Math::HALF_PI;
	if (keyboard.isKeyDown(KEY_E))
		cameraRotation -= Math::HALF_PI;
	cameraRotation *= deltaTime;
	mCamera->rotate(cameraRotation);

	if (0 == mNumOfSprites)
		return;

	// update animated sprites
	for(uint32 i = 0; i < mNumOfSprites; ++i)														
		if (Sprite::ANIMATED_SPRITE == mSprites[i]->GetType())
			((AnimatedSprite *) mSprites[i])->update();

	// compute other updates
	mSprites[2]->rotate(deltaTime * (-Math::PI / 18));												
	mSprites[3]->rotate(deltaTime * Math::PI / 18);
	mSprites[3]->setPosition(Vector2(5.0f * mouse.getAbsoluteX(), 5.0f * mouse.getAbsoluteY()));
	if (keyboard.isKeyDown(KEY_UP))
		mSprites[3]->setDepth(mSprites[3]->getDepth() + deltaTime);
	if (keyboard.isKeyDown(KEY_DOWN))
	{
		const Real temp = mSprites[3]->getDepth() - deltaTime;
		mSprites[3]->setDepth(Math::clamp((float) temp, 2.0f, 0.0f));
	}
}
