/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _WINDOWS
	#include <WinSock2.h>
#endif // _WINDOWS

#include "Network/MagicConstants.h"

#ifdef _WINDOWS
	const int32 Network::WOULD_BLOCK = WSAEWOULDBLOCK;
	const int32 Network::NO_MULTICAST_SUPPORT = WSAESOCKTNOSUPPORT;
#elif _LINUX
	const int32 Network::WOULD_BLOCK = EWOULDBLOCK;
	const int32 Network::NO_MULTICAST_SUPPORT = EFAULT;
#endif // _WINDOWS
