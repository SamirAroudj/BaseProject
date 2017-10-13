/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _MULTITHREADING_TASK_
#define _MULTITHREADING_TASK_

#include <cassert>
#include <mutex>

namespace Platform
{
	namespace Multithreading
	{
		class Task
		{
		public:
			Task();

			virtual void function() = 0;

			inline bool hasFinished() const { return mFinished; }

			inline void redo() { mFinished = false; }

			void skip();

			void solve();

			void waitUntilFinished();

        private:
            /** Copy constructor is forbidden.
            @param copy Copy constructor is forbidden. */
            Task(Task &copy) { assert(false); }

		private:
			static std::mutex	msMutex;
			bool				mFinished;
		};
	}
}

#endif // _MULTITHREADING_TASK_
