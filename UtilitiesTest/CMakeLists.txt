#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# test executable for utilities library
set(componentName UtilitiesTest)
set(appName ${componentName}.exe)
set(componentPath ${PROJECT_SOURCE_DIR}/${componentName})

# define executable
add_executable(${appName} main.cpp)

# required internal libs
set(requiredLibs
	Utilities
	Platform
	${mathLibName}
)

# dependencies
add_dependencies(${appName}
	${requiredLibs}
)

# required external libs
set(requiredLibs
	${requiredLibs}
	debug ${TINYXML2_DEBUG_LIBRARY}
	optimized ${TINYXML2_RELEASE_LIBRARY}
)

# do platform specific things (link required libs, enable windows window support etc.)
if (WIN32)
	#addWindowsLibs(requiredLibs)
	#enableWindowsSubsystem(${appName})
elseif (${LINUX})
	addLinuxLibs(requiredLibs)
endif (WIN32)

# link libraries
target_link_libraries(${appName}
	${requiredLibs}
)
