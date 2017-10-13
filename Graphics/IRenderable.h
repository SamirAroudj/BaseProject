/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _I_RENDERABLE_H_
#define _I_RENDERABLE_H_

namespace Graphics
{
	/// Interface which should be implmented by classes that provide a render function
	class IRenderable
	{
	public:
		/** Virtual destructor ensures proper destruction of objects of a implementing class. */
		virtual ~IRenderable() { }

		/** Draw this. */
		virtual void render() const = 0;
	};
}

#endif // _I_RENDERARB_H_