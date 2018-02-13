/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
#include <Windows.h>
#elif _LINUX
#endif // _WINDOWS

#include "MyApp.h"
#include "Platform/ResourceManagement/MemoryManager.h"
#include "Platform/Utilities/Conversions.h"
using namespace Platform;

#ifdef MEMORY_MANAGEMENT
	const uint32 ResourceManagement::DEFAULT_POOL_BUCKET_NUMBER = 5;
	const uint16 ResourceManagement::DEFAULT_POOL_BUCKET_CAPACITIES[DEFAULT_POOL_BUCKET_NUMBER] = { 1024, 1024, 1024, 1024, 1024 };
	const uint16 ResourceManagement::DEFAULT_POOL_BUCKET_GRANULARITIES[DEFAULT_POOL_BUCKET_NUMBER] = { 16, 32, 64, 128, 256 };
#endif // MEMORY_MANAGEMENT

#ifdef _WINDOWS
	int32 WINAPI WinMain(HINSTANCE applicationHandle, HINSTANCE unused, LPSTR commandLineString, int32 windowShowState)
#else
	int main(int argc, char *argv[])
#endif // _WINDOWS
{
	std::string test1 = Utilities::Converter::from<uint8, INTEGER_BASE_HEXADECIMAL>(12);
	std::string test2 = Utilities::Converter::from<uint8>(12);
	std::string test3 = Utilities::Converter::from<int8>(-8);

	{
		#ifdef _WINDOWS
			MyApp application(applicationHandle);
		#else
			MyApp application;
		#endif // _WINDOWS

		application.run();
	}

	#ifdef MEMORY_MANAGEMENT
		ResourceManagement::MemoryManager::shutDown();
	#endif // MEMORY_MANAGEMENT
	return 0;
}
