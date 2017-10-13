/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _GRAPHICS_DEMO_H_
#define _GRAPHICS_DEMO_H_

#include "Platform/Application.h"
#include "FrameStatistics.h"
#include "DirectionalLight.h"
#include "Event.h"
#include "Math/MathHelper.h"
#include "Player.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Scene.h"
#include "StaticObject.h"
#include "Graphics/Texture.h"
#include "Platform/TimePeriod.h"

namespace Graphics
{
	/// Class for demonstration of graphical features implemented with DirectX
	class GraphicsDemo : public Platform::Application
	{
	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Create the graphics demonstration. (load configuration data, create the window etc) </summary>
		///
		/// <remarks>	Samir, 30.11.2011. </remarks>
		///
		/// <param name="applicationHandle"> 	Should be obtained by WinMain. </param>
		/// <param name="graphicsConfigFile">	Contains data necessary to start the program. See GraphicsConfigurationLoader. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GraphicsDemo(HINSTANCE applicationHandle, const std::string &graphicsConfigFile);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	 Release requested resources. </summary>
		///
		/// <remarks>	Samir, 30.11.2011. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual ~GraphicsDemo();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	This function is called when the application's main window is activated (Observer pattern). </summary>
		///
		/// <remarks>	Samir, 19.11.2011. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void onActivation();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	This function is called when the application's main window is deactivated  (Observer pattern). </summary>
		///
		/// <remarks>	Samir, 19.11.2011. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void onDeactivation();

	protected:

		// creates a coloured and composite object which exploits scene graph functionality to test SceneNode and Scene
		void createThingToTestScene();

		// put some things togehter so that it looks a little bit interesting
		void createSmallScene();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Posts the render. </summary>
		///
		/// <remarks>	Samir, 29.11.2011. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void postRender();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Render the scene or whatever you want to show. </summary>
		///
		/// <remarks>	Samir, 29.11.2011. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void render();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Update the simulation. </summary>
		///
		/// <remarks>	Samir, 29.11.2011. </remarks>
		///
		/// <param name="deltaTime">	Contains the elapsed time in seconds between last update invocation and this update call. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void update(Real deltaTime);

		// this scene node is rotated using smooth quaternion interpolation
		void updateSmallScene();

		/// <summary> Statistics containing the frame rate and the time necessary to render a frame. See FrameStatistics. </summary>
		FrameStatistics mFrameStatistics;

		// representation of the user in the scene
		Player mPlayer;

		// test 3D sound effects
		Audio::Event *mAirBlow;

		/// test of illumination
		DirectionalLight mDirectionalLight;
		PointLight mPointLight;
		SpotLight mSpotLight;

		/// <summary> Test the static object class. </summary>
		StaticObject *mCrate;
		StaticObject *mFloor;
		StaticObject *mFloorSmall;
		StaticObject *mPyramid;
		StaticObject *mStaticObject;
		StaticObject *mTank;
		StaticObject *mWomen;

		/// <summary> Test a shader that involves lighting and textures. </summary>
		StaticObject *mTestRectangle;

		// small scene to demonstrate some features and test of smooth quaternion interpolation
		Graphics::SceneNode *mSmallScene;
		Math::Quaternion mSmallSceneYRotation[2];
		Platform::TimePeriod mSmallSceneYTimer;

		// Short test of scene graph functionality.
		Graphics::SceneNode *mTest;
		Real mDistance;
		bool up;

		/// <summary> used for scene management by means of a scene graph </summary>
		Graphics::Scene mScene;

		/// <summary> Height map which is used to display some basic terrain.</summary>
		StaticObject *mHeightMap;
	};
}

#endif // _GRAPHICS_DEMO_H_
