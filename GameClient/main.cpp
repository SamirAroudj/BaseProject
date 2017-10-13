/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <Windows.h>
#include "Game.h"
#include "Light.h"
#include "Platform/ResourceManagement/ResourceManagement.h"

using namespace GamePlay;
using namespace Graphics;
using namespace ResourceManagement;
using namespace Platform;
using namespace std;

// Memory management
const uint32 ResourceManagement::DEFAULT_POOL_BUCKET_NUMBER = 5;
const uint16 ResourceManagement::DEFAULT_POOL_BUCKET_CAPACITIES[DEFAULT_POOL_BUCKET_NUMBER] = { 1024, 1024, 1024, 1024, 1024 };
const uint16 ResourceManagement::DEFAULT_POOL_BUCKET_GRANULARITIES[DEFAULT_POOL_BUCKET_NUMBER] = { 16, 32, 64, 128, 256 };

int32 WINAPI WinMain(HINSTANCE applicationHandle, HINSTANCE unused, LPSTR commandLineString, int32 windowShowState)
{
	int32 result = 0;

	{
		#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
			Game game(applicationHandle, "DebugGraphics.cfg");
		#else
			Game game(applicationHandle, "ReleaseGraphics.cfg");
		#endif // _DEBUG
		result = game.run();
	}

	Light::freeMemory();
	MemoryManager::shutDown();
	return result;
}