/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _SUB_GUI_H_
#define _SUB_GUI_H_

#include <cassert>
#include <vector>

#include "Platform/DataTypes.h"
#include "Graphics/RenderGroup.h"

namespace Graphics
{
	namespace GUI
	{
		/// Represents one part of the GUI, e.g. the GUI which is specific for a Player object
		class SubGUI
		{
		public:

			/** Creates a SubGUI object with a unique identifier and a specific maximum depth.
			@param identifier Sets the ID of this new SubGUI object.
			@param numOfLayers Defines how many layers the GUI creates. */
			SubGUI(int32 identifier, uint32 numOfLayers);

			/** Destroys the SubGUI object and all of its layers which should be empty. */
			~SubGUI();

			/** Adds an IRenderable object to a specific layer.
			@param renderable This object is rendered each time the layer it belongs to is rendered.
			@param layer Identifiers the layer the renderable is added to whereas 0 is the lowest layer which is occluded by all other layers. */
			void add(const IRenderable &renderable, uint32 layer);

			/** Provides access to the ID of the SubGUI.
			@return The identifier that was used for creation is returned. */
			int32 getID() const { return mIdentifier; }

			/** Removes an IRenderable object from the layer it belongs to if it is found.
			@param renderable It is removed from the SubGUI if it is found otherwise an assertion fails.
			@param layer This is the layer the renderable was added to previously. */
			void remove(const IRenderable &renderable, uint32 layer);

			/** Renders all layers of the SubGUI object whereas layer 0 is rendered first and the layer with the highest number is the last one to be rendered.
				The layer number corresponds to the height of the layer whereas 0 means it is the lowest layer which is occluded by all other layers.
				Higher layer numbers mean that the GUI elements of the layer are closer to the viewer. */
			void render() const;

		protected:
			std::vector<RenderGroup *>	mRenderGroups;			/// each render group is one layer of the SubGUI object
			int32						mIdentifier;			/// identifies the SubGUI object

		private:
			/** Copy constructor is forbidden. */
			SubGUI(const SubGUI &rhs) { assert(false); }

			/** Assignment operator is forbidden. */
			SubGUI &operator =(const SubGUI &rhs) { assert(false); return *this; }
		};
	}
}

#endif // _SUB_GUI_H_

