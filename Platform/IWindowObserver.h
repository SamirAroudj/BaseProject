/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _IWINDOW_LISTENER_H_
#define _IWINDOW_LISTENER_H_

namespace Platform
{
	class IWindowObserver
	{
	public:
		/** If the window the observer is registered for is activated then this function is called. */
		virtual void onActivation() = 0;

		/** If the window the observer is registered for is deactivated then this function is called. */
		virtual void onDeactivation() = 0;

		/** If the window the observer is registered for is stopping to be resized then this function is called. */
		/*virtual void onResize() = 0;*/
	};
}

#endif // _IWINDOW_LISTENER_H_
