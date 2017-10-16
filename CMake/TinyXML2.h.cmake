#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# TinyXML2 specific cmake functionality

# cached path to external TinyXML2 project include directory
set(TINYXML2_INCLUDE_DIR "../tinyxml2" CACHE PATH "Path to TinyXML2 root directory containing tinyxml2.h.")
set(TINYXML2_DEBUG_LIBRARY "../tinyxml2/Build/Debug/libtinyxml2.so" CACHE FILEPATH "Complete path and name of TinyXML2 library file.")
set(TINYXML2_RELEASE_LIBRARY "../tinyxml2/Build/Release/libtinyxml2.so" CACHE FILEPATH "Complete path and name of TinyXML2 library file.")

# make include directory available
include_directories(${TINYXML2_INCLUDE_DIR})

