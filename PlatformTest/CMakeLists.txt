#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# test executable for platform library
set(componentName PlatformTest)
set(appName ${componentName}.exe)
set(componentPath ${PROJECT_SOURCE_DIR}/${componentName})

# define executable
add_executable(${appName} ${componentPath}/main.cpp)

# define required internal libraries and do linking stuff
set(requiredLibs 
	Platform
	Utilities
	${mathLibName}
)

# dependencies for internal libraries
add_dependencies(${appName} ${requiredLibs})

# required external libs
set(requiredLibs
	${requiredLibs}
)
	

# do platform specific things (link required libs, enable windows window support etc.)
if (WIN32)
	addWindowsLibs(requiredLibs)
	enableWindowsSubsystem(${appName})
elseif (${LINUX})
	addLinuxLibs(requiredLibs)
endif (WIN32)

target_link_libraries(${appName} ${requiredLibs})