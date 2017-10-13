/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include "Manager.h"
#include "Task.h"

using namespace Platform::Multithreading;
using namespace std;

mutex Task::msMutex;

Task::Task() : mFinished(false)
{

}

void Task::skip()
{
	mFinished = true;
}

void Task::solve()
{
	// solve task and inform waiters
	function();

	unique_lock<mutex> uniqueLock(msMutex);
	mFinished = true;
	uniqueLock.unlock();

	Manager::getSingleton().mTasksCondition.notify_all();
}

void Task::waitUntilFinished()
{
	// wait until task has been finished
	unique_lock<mutex> uniqueLock(msMutex);

	while (!mFinished)
		Manager::getSingleton().mTasksCondition.wait(uniqueLock);
}