#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# windows specific cmake functionality

set(WINDOWS ${WIN32})

# set windows preprocessor flags
if (${WINDOWS})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_WINDOWS")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WINDOWS")
endif (${WINDOWS})

# macro for defining a windows executable supporting window / graphics stuff
macro(enableWindowsSubsystem target)
	if (${WINDOWS})
		if(MSVC)
			set_target_properties(${target} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
		endif()
	endif (${WINDOWS})
endmacro(enableWindowsSubsystem)

# CMake macro for adding windows specific libs
macro(addWindowsLibs requiredLibs)
	if (${WINDOWS})
		set(${requiredLibs}
			${${requiredLibs}}
			dinput8.lib
			dxguid.lib
			Winmm.lib
			xinput.lib)
	endif (${WINDOWS})
endmacro(addWindowsLibs)