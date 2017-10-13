#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# CollisionDetection library
set(componentName CollisionDetection)

# set paths to source folders
set(componentPath ${PROJECT_SOURCE_DIR}/${componentName})

# CMake files
set(cmakeFiles
	${componentPath}/CMakeLists.txt
)

# header files
set(headerFiles 
	${componentPath}/CollisionDetection.h
	${componentPath}/ObliqueCircularCone.h
	${componentPath}/Plane.h
)

# source files
set(sourceFiles
	${componentPath}/CollisionDetection.cpp
	${componentPath}/ObliqueCircularCone.cpp
)

# get all file groups together
set(libFiles
	${cmakeFiles}
	${headerFiles}
	${sourceFiles}
)

add_library(${componentName} ${libFiles})

# define source groups for file management within IDE
source_group("CMake Files" FILES ${cmakeFiles})
source_group("Header Files" FILES ${headerFiles})
source_group("Source Files" FILES ${sourceFiles})