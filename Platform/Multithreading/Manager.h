/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include "Platform/DataTypes.h"
#include "Patterns/Singleton.h"
#include "Task.h"

namespace Platform
{
	namespace Multithreading
	{
		/// Manages threads for task parallelization.
		/** Creates threads and provides features to work off tasks in a parallel manner. */
		class Manager : public Patterns::Singleton<Manager>
        {
        friend Task;

        public:
			Manager();
			~Manager();

			void enqueue(Task *task);

			uint32 getThreadCount() const { return mThreadCount; }

			inline bool isRunning() const { return mRunning; }
			
			void runWork(uint32 threadCount);

			void stopWork();

		private:
            /** Copy constructor is forbidden.
            @param rhs Don't call it, it fails.*/
            Manager(const Manager &rhs) { assert(false); }

            /** Assignment operator is forbidden.
            @param rhs Don't call it, it fails.
            @return Don't call it, it fails. */
            Manager &operator =(const Manager &rhs) { assert(false); return *this; }

			static void workerFunction();

        private:
			std::thread				*mWorkers;
			std::queue<Task *>		mTasks;
			std::condition_variable mTasksCondition;
			std::condition_variable	mWorkersCondition;
			std::mutex				mQueueMutex;

			uint32					mThreadCount;
            bool					mRunning;
		};
	}
}

#endif // _THREAD_POOL_H_
