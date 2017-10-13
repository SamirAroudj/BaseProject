/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _GUI_Manager_H_
#define _GUI_MANAGER_H_

#include "Platform/DataTypes.h"
#include "Patterns/Singleton.h"
#include "../Sprite.h"
#include "SubGUI.h"
#include "../Viewport.h"

namespace Graphics
{
	/// Contains functionality to render GUI stuff, such as text, buttons, text fields, etc.
	namespace GUI
	{
		/// Main manager of GUI functionality.
		/** It's responsible for management of the general sub GUI and each sub GUI that is created for each local user. */
		class GUIManager : public Patterns::Singleton<GUIManager>
		{
		public:
			/** Creates the manager and the main SubGUI object. */
			GUIManager();

			/** Destroys the manager and its main SubGUI object. */
			virtual ~GUIManager();

			/** Creates a new SubGUI that is taken care of by this manager.
			@param identifier This is the ID of the object to be created so that it can be found later. It must not already be used.
			@param numOfLayers Defines the maximum number of layers that are used for this SubGUI object. */
			SubGUI &createSubGUI(int32 indentifier, uint32 numOfLayers);

			/** Destroys a SubGUI object.
			@param identifier The SubGUI that is destroyed has the ID identifier. */
			void deleteSubGUI(int32 identifier);

			/** Provides access to all created SubGUI instances.
			@param The returned object is found by means of its ID which should be equal to identifier.
				   An assertion fails if there is no SubGUI object with the entered identifier. */
			SubGUI &getSubGUI(int32 identifier);

			/** Renders the main SubGUI object which is the common / general GUI that occupies the whole backbuffer. */
			void renderMainSubGUI();

			/** Updates the whole GUI. */
			void update();

		private:
			std::vector<SubGUI *>	mSubGUIs;				/// the GUI consists of several SubGUI objects, a main sub GUI and a Sub GUI for each Player object
			Sprite					*mTestSprite;			/// for the purpose of debugging there is always some GUI
			Viewport				mMainSubGUIViewport;	/// this viewport is as big as the back buffer and used by the main SubGUI object
		};
	}
}

#endif // _GUI_MANAGER_H_
