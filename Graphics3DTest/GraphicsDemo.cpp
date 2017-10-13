/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <cstdlib>
#include "AudioManager.h"
#include "FontManager.h"
#include "GraphicsConfigurationLoader.h"
#include "GraphicsDemo.h"
#include "Graphics/GraphicsManager.h"
#include "Platform/Input/InputManager.h"
#include "MagicGraphicsConstants.h"
#include "Math/MathHelper.h"
#include "Platform/Window.h"

using namespace Audio;
using namespace Graphics;
using namespace GUI;
using namespace Input;
using namespace Math;
using namespace ResourceManagement;
using namespace Platform;

const uint32 MUSICCUE_TEST_DANCEOFTHESUGARPLUMFAIRY = 1;

Real heightFunction(Real x, Real z)
{
	return 0.3f * ( z * sinr(0.1f * x) + x * cosr(0.1f * z));
}

GraphicsDemo::GraphicsDemo(HINSTANCE applicationHandle, const std::string &graphicsConfigFile) : 
	Application(applicationHandle, graphicsConfigFile),
	mDirectionalLight(Color(0.1f, 0.1f, 0.1f, 1.0f), Color(0.1f, 0.1f, 0.5f, 1.0f), Color(0.1f, 0.2f, 0.5f, 1.0f), Vector3(0.0f, -1.0f, 0.0f)),
	mSpotLight(Color(0.1f, 0.1f, 0.1f, 1.0f), Color(0.8f, 0.8f, 0.8f, 1.0f), Color(0.2f, 0.8f, 0.2f, 1.0f),
			Vector3(-3.0f, 5.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f),
			Vector3(0.0f, 0.0f, 0.2f), 100.0f, 10.0f),
	mPointLight(Color(0.3f, 0.3f, 0.3f, 1.0f), Color(0.8f, 0.8f, 0.8f, 1.0f), Color(0.8f, 0.2f, 0.2f, 1.0f),
		Vector3(-2.0f, 5.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), 50.0f),
	mSmallSceneYTimer(1.0f),
	mDistance(0.0f),
	up(false)
{
	uint32 refreshRate = 60, multisamplingCount = 1, multisamplingQuality = 0;
	bool enableVSync = true;
	GraphicsConfigurationLoader::loadGraphicsConfiguration(refreshRate, multisamplingCount, multisamplingQuality, enableVSync, graphicsConfigFile);
	GraphicsManager *graphicsManager = new GraphicsManager(refreshRate, multisamplingCount, multisamplingQuality, enableVSync);
	FontManager *fontManager = new FontManager();

	// init audio
	AudioManager *manager = new AudioManager("Audio\\");
	manager->loadEventDefinitions("Test.fev");
	manager->setNumOfListeners(1);
	manager->setBackgroundMusic(MUSICCUE_TEST_DANCEOFTHESUGARPLUMFAIRY);
	manager->startBackgroundMusic();

	// init sound effects
	mAirBlow = new Event("Test/TestGroup/AirBlow");
	mAirBlow->start();
	mAirBlow->setPosition(Vector3());

	// create static objects
	mCrate = new StaticObject("WoodCrate.mat", "Crate");
	mFloor = new StaticObject("Color.mat", "Floor");
	mFloorSmall = new StaticObject("Floor.mat", "TestRectangle");
	mHeightMap = new StaticObject("Color.mat", "HeightMap", 250.0f, 250.0f, 1000, 1000, heightFunction);
	mPyramid = new StaticObject("Color.mat", "Pyramid");
	mStaticObject = new StaticObject("Color.mat", "Cube");
	mTank = new StaticObject("Tank.mat", "Tank.mesh");	// something with regard to the tank normals went wrong
	mTestRectangle = new StaticObject("LightWoodCrate.mat", "TestRectangle");
	mWomen = new StaticObject("Women.mat", "Women.mesh");

	mScene.getRootSceneNode().addRenderable(mHeightMap);
	createThingToTestScene();

	// a simple, but flying ;), crate
	SceneNode *crateNode = new SceneNode();
	crateNode->translate(Vector3(3.0f, 5.0f, 0.0f));
	crateNode->addRenderable(mCrate);
	mScene.getRootSceneNode().addChild(crateNode);

	// floor which looks a little bit like toxic water which reminds me of The Toxic Avenger YouTube video
	SceneNode *childNode = new SceneNode();
	childNode->translate(Vector3(0.0f, -20.0f, 0.0f), SceneNode::PARENT);
	childNode->setScale(Vector3(2.0f, 2.0f, 2.0f));
	childNode->addRenderable(mFloor);
	mScene.getRootSceneNode().addChild(childNode);

	createSmallScene();
}

void GraphicsDemo::createThingToTestScene()
{
	// create this colored and rotating thing in the middle of the scene
	Quaternion rotation(Vector3(0.0f, 0.0f, 1.0f), Math::PI);
	SceneNode *node = new SceneNode();
	node->setScale(Vector3(0.75f, 0.75f, 0.75f));
	node->rotate(rotation);
	node->translate(Vector3(0.0f, 5.0f, 10.0f), SceneNode::PARENT);
	mScene.getRootSceneNode().addChild(node);
	node->addRenderable(mStaticObject);

	Quaternion rotationZAxis(Vector3(0.0f, 0.0f, 1.0f), Math::HALF_PI);
	SceneNode *childNode = new SceneNode();
	childNode->addRenderable(mPyramid);
	childNode->translate(Vector3(0.0f, 1.0f, 1.0f), SceneNode::PARENT);
	childNode->rotate(rotation, SceneNode::PARENT);
	childNode->rotate(rotationZAxis, SceneNode::PARENT);
	node->addChild(childNode);
	mTest = childNode;

	childNode = new SceneNode();
	childNode->addRenderable(mStaticObject);
	childNode->translate(Vector3(0.0f, 1.0f, -1.0f));
	node->addChild(childNode);
}

void GraphicsDemo::createSmallScene()
{
	// prepare a small scene
	Vector3 lightDirection(-1.0f, -1.0f, -1.0f);
	lightDirection.normalize();
	mDirectionalLight.setDirection(lightDirection);
	mSmallScene = new SceneNode();
	mSmallScene->translate(Vector3(-3.0f, 2.0f, 1.0f));
	mScene.getRootSceneNode().addChild(mSmallScene);

	// flying rectangle to test LightTex.fx etc
	SceneNode *childNode = new SceneNode();
	childNode->translate(Vector3(0.0f, 1.0f, 0.0f), SceneNode::PARENT);
	childNode->addRenderable(mTestRectangle);
	childNode->setScale(Vector3(2.0f, 2.0f, 2.0f));
	mSmallScene->addChild(childNode);

	// show our loaded women - actually, she looks a little bit like being loaded^^
	childNode = new SceneNode();
	childNode->translate(Vector3(-3.0f, 2.0f, 0.0f), SceneNode::PARENT);
	childNode->addRenderable(mWomen);
	childNode->setScale(Vector3(0.01f, 0.01f, 0.01f));
	childNode->rotate(Quaternion(Vector3(1.0f, 0.0f, 0.0f), -Math::HALF_PI) * Quaternion(Vector3(0.0f, 1.0f, 0.0f), Math::HALF_PI));
	mSmallScene->addChild(childNode);

	// a second women - man, I've never thought that it could be that easy to get women^^
	childNode = new SceneNode();
	childNode->translate(Vector3(3.0f, 2.0f, 0.0f), SceneNode::PARENT);
	childNode->addRenderable(mWomen);
	childNode->setScale(Vector3(0.01f, 0.01f, 0.01f));
	childNode->rotate(Quaternion(Vector3(1.0f, 0.0f, 0.0f), -Math::HALF_PI) * Quaternion(Vector3(0.0f, 1.0f, 0.0f), -Math::HALF_PI));
	mSmallScene->addChild(childNode);

	// what about some tanks??
	childNode = new SceneNode();
	childNode->translate(Vector3(0.0f, -0.0f, 6.0f));
	childNode->addRenderable(mTank);
	childNode->setScale(Vector3(0.01f, 0.01f, 0.01f));
	mSmallScene->addChild(childNode);

	// guys love tanks, don't they?
	childNode = new SceneNode();
	childNode->translate(Vector3(0.0f, -0.0f, -6.0f));
	childNode->addRenderable(mTank);
	childNode->rotate(Quaternion(Vector3(0.0f, 1.0f, 0.0f), Math::PI));
	childNode->setScale(Vector3(0.01f, 0.01f, 0.01f));
	mSmallScene->addChild(childNode);

	// create a ground 
	childNode = new SceneNode();
	childNode->setScale(Vector3(4.0f, 8.0f, 8.0f));
	childNode->translate(Vector3(0.0f, -0.6f, 0.0f), SceneNode::PARENT);
	childNode->addRenderable(mFloorSmall);
	mSmallScene->addChild(childNode);

	mSmallSceneYRotation[0] = Quaternion(Vector3(0.0f, 1.0f, 0.0f), 0.0f);
	mSmallSceneYRotation[1] = Quaternion(Vector3(0.0f, 1.0f, 0.0f), Math::PI);
}

GraphicsDemo::~GraphicsDemo()
{
	delete mCrate;	// free requested resources
	delete mFloor;
	delete mFloorSmall;
	delete mHeightMap;
	delete mPyramid;
	delete mStaticObject;
	delete mTestRectangle;
	delete mTank;
	delete mWomen;

	delete mAirBlow;

	delete AudioManager::getSingletonPointer();	// free singletons
	delete FontManager::getSingletonPointer();
	delete GraphicsManager::getSingletonPointer();

	assert(UserResource<StaticMesh>::getNumOfResources() == 0);	// test whether everything was freed
	UserResource<StaticMesh>::freeMemory();
	assert(UserResource<VertexLayout>::getNumOfResources() == 0);
	UserResource<VertexLayout>::freeMemory();
	assert(UserResource<Shader>::getNumOfResources() == 0);
	UserResource<Shader>::freeMemory();
	assert(UserResource<Texture>::getNumOfResources() == 0);
	UserResource<Texture>::freeMemory();
	assert(UserResource<Material>::getNumOfResources() == 0);
	UserResource<Material>::freeMemory();
}

void GraphicsDemo::onActivation()
{
	Application::onActivation();
}

void GraphicsDemo::onDeactivation()
{
	Application::onDeactivation();
}

void GraphicsDemo::postRender()
{

}

void GraphicsDemo::render()
{
	GraphicsManager &graphicsManager = GraphicsManager::getSingleton();
	FontManager &fontManager = FontManager::getSingleton();

	graphicsManager.clearBackAndDepthStencilBuffer();
	graphicsManager.clearOutputMergerStates();

	uint32 width = Window::getSingleton().getWidth(), height = Window::getSingleton().getHeight();
	ViewportDescription viewports[] = { ViewportDescription(0, 0, width, height / 2), ViewportDescription(0, height / 2, width, height / 2) };
	for(int32 i = 1; i >= 0; --i)
	{
		mPlayer.getCamera().yaw(Math::PI);
		graphicsManager.setViewports(&viewports[i], 1);

		Matrix4x4 viewProjection = mPlayer.getCamera().getViewMatrix() * mPlayer.getCamera().getProjectionMatrix();
		mScene.render(viewProjection);
	}
	Color textColor(1.0f, 1.0f, 1.0f, 1.0f);
	fontManager.getFont(0).renderText(mFrameStatistics.getStatistics(), 50, 100, textColor);

	graphicsManager.presentBackBuffer();
}

void GraphicsDemo::update(Real deltaTime)
{
	if (InputManager::getSingleton().getKeyboard().isKeyPressed(KEY_ESCAPE))
		mRun = false;

	//if (!Window::getSingleton().isActive())
	//	return;

	AudioManager::getSingleton().update();
	mPlayer.update(deltaTime);
	updateSmallScene();
	mFrameStatistics.update();

	// update application volume
	Real mouseZMotion = InputManager::getSingleton().getMouse().getRelativeZMotion();
	if (mouseZMotion != 0.0f)
	{
		Real applicationVolume = AudioManager::getSingleton().getApplicationVolume();
		applicationVolume += mouseZMotion * 0.1f;
		AudioManager::getSingleton().setApplicationVolume(Math::clamp(applicationVolume, 1.0f, 0.0f));
	}

	// update this abstract thing in the scene
	SceneNode &node = mScene.getRootSceneNode().getChild(0);
	node.setOrientation(node.getOrientation() * Quaternion(Vector3(0.0f, 1.0f, 0.0f), 0.2f * Math::PI * deltaTime));
	mTest->rotate(Quaternion(Vector3(0.0f, 0.0f, 1.0f), Math::PI * deltaTime), SceneNode::PARENT);

	Real movement = deltaTime;	// update the position of the "pyramid arm" of this abstract thing
	if (up)
	{
		if (mDistance + movement > 2.0f)
		{
			up = false;
			movement = 2.0f - mDistance;
		}
	}
	else
	{
		movement *= -1.0f;
		if (mDistance + movement < 0.0f)
		{
			up = true;
			movement = 0.0f - mDistance;
		}
	}
	mDistance += movement;
	mTest->translate(Vector3(0.0f, movement, 0.0f));
}

void GraphicsDemo::updateSmallScene()
{
	if (mSmallSceneYTimer.hasExpired())
	{
		Real randomAngle = (static_cast<Real>(rand()) / RAND_MAX) * Math::TWO_PI;
		mSmallSceneYTimer.reset();
		mSmallSceneYRotation[0] = mSmallSceneYRotation[1];
		mSmallSceneYRotation[1] = Quaternion(Vector3(0.0f, 1.0f, 0.0f), randomAngle);
	}

	Real yFactor = Math::smoothCosFCurve(mSmallSceneYTimer.getElapsedTime() / mSmallSceneYTimer.getLength());
	Quaternion interpolation = Quaternion::sLerp(mSmallSceneYRotation[0], mSmallSceneYRotation[1], yFactor);
	mSmallScene->setOrientation(interpolation);
}