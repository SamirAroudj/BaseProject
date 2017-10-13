#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# TinyXML2 specific cmake functionality

# cached path to external TinyXML2 project include directory
set(TINYXML2_INCLUDE_DIR "../TinyXML2" CACHE PATH "Path to TinyXML2 root directory containing tinyxml2.h.")
set(TINYXML2_DEBUG_LIBRARY "../TinyXML2/Debug/TinyXML2Lib" CACHE FILEPATH "Complete path and name of TinyXML2 library file.")
set(TINYXML2_RELEASE_LIBRARY "../TinyXML2/Release/TinyXML2Lib" CACHE FILEPATH "Complete path and name of TinyXML2 library file.")

# make include directory available
include_directories(${TINYXML2_INCLUDE_DIR})

