#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# LibPNG specific cmake functionality

# cached path to external libPNG project include directory
#set(LIB_PNG_INCLUDE_DIR ${PNG_INCLUDE_DIR} CACHE PATH "Path to lib PNG root directory containing png.h.")
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

macro(addPNGLibs libs)
	set(${libs} ${${libs}} ${PNG_LIBRARY}	)
endmacro(addPNGLibs)
