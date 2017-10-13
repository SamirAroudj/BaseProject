#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# test executable for graphics 2D library
set(componentName GraphicsTest)
set(appName ${componentName}.exe)
set(componentPath ${PROJECT_SOURCE_DIR}/${componentName})

#include OpenGL, glut and glew headers
include(${PROJECT_SOURCE_DIR}/CMake/LibPNG.h.cmake)
include(${PROJECT_SOURCE_DIR}/CMake/OpenGL.h.cmake)

# CMake files
set(cmakeFiles
	${componentPath}/CMakeLists.txt
)
source_group("CMake Files" FILES ${cmakeFiles})

# header files
set(headerFiles 
	${componentPath}/MyApp.h
)
source_group("Header Files" FILES ${headerFiles})

# source files
set(sourceFiles
	${componentPath}/MyApp.cpp
	${componentPath}/main.cpp
)
source_group("Source Files" FILES ${sourceFiles})

# get all file groups together
set(sourceCode
	${cmakeFiles}
	${headerFiles}
	${sourceFiles}
)

# define executable
add_executable(${appName} ${sourceCode})

# required internal libs
set(requiredLibs ${requiredLibs}
	Graphics
	Platform
	Utilities
	${mathLibName}
)

# add corresponding dependencies for internal libs
add_dependencies(${appName} ${requiredLibs})

# required external libs
addPNGLibs(requiredLibs)
addOpenGLLibs(requiredLibs)


# do platform specific things (link required libs, enable windows window support etc.)
if (WIN32)
	addWindowsLibs(requiredLibs)
	enableWindowsSubsystem(${appName})
elseif (${LINUX})
	addLinuxLibs(requiredLibs)
endif (WIN32)

target_link_libraries(${appName} ${requiredLibs})