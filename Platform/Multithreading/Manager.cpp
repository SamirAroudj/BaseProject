/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Manager.h"

using namespace Platform::Multithreading;
using namespace std;

Manager::Manager() :
	mWorkers(NULL), mThreadCount(0), mRunning(false)
{
}

Manager::~Manager()
{
	stopWork();
}

void Manager::enqueue(Task *task)
{
	// queue task
	{
		unique_lock<mutex> uniqueLock(mQueueMutex);
		mTasks.push(task);
	}

	mWorkersCondition.notify_one();
}

void Manager::runWork(uint32 threadCount)
{
	mThreadCount	= threadCount;
	mRunning		= true;
	mWorkers		= new thread[mThreadCount];

	for (uint32 i = 0; i < mThreadCount; ++i)
		mWorkers[i] = thread(&Manager::workerFunction);
}

void Manager::stopWork()
{
	// clear tasks
	unique_lock<mutex> uniqueLock(mQueueMutex);
		while(!mTasks.empty())
		{
			mTasks.front()->skip();
			mTasks.pop();
		}

		// free memory
		queue<Task *> emptyQueue;
		mTasks.swap(emptyQueue);
	uniqueLock.unlock();

	// stop workers
	mRunning = false;
	mWorkersCondition.notify_all();

	for (uint32 i = 0; i < mThreadCount; ++i)
		mWorkers[i].join();

	// stop waiting for tasks
	mTasksCondition.notify_all();

	// free workers so threads join this one
	delete [] mWorkers;
	mWorkers = NULL;
	mThreadCount = 0;
}

void Manager::workerFunction()
{
	Task *task = NULL;
	unique_lock<mutex> uniqueLock(Manager::getSingleton().mQueueMutex);
	uniqueLock.unlock();

	while (Manager::getSingleton().mRunning)
	{
		// get some or wait for work
		uniqueLock.lock();

		// wait as long as there are no tasks
		while (Manager::getSingleton().mRunning && Manager::getSingleton().mTasks.empty())
			Manager::getSingleton().mWorkersCondition.wait(uniqueLock);

		// stop working if requested
		if (!Manager::getSingleton().mRunning)
			break;

		// extract task to do
		Task *task = Manager::getSingleton().mTasks.front();
		Manager::getSingleton().mTasks.pop();

		uniqueLock.unlock();

		// execute task
		task->solve();
	}
}
