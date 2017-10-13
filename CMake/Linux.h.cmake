#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# OpenGL specific cmake functionality

set(LINUX  ${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (${LINUX})
	# set Linux preprocessor flags
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_LINUX")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX")
  
	# enable c++11
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	find_package(Threads REQUIRED) # add lib for multithreading c++11 support
endif (${LINUX})



# CMake macro for adding linux specific libs
macro(addLinuxLibs requiredLibs)
	if (${LINUX})
		set	(${requiredLibs}
				${${requiredLibs}}		
				X11
				pthread		
			)
	endif (${LINUX})
endmacro(addLinuxLibs)
