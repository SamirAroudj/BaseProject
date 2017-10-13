#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# Patterns library
set(libName Patterns)
set(componentPath ${PROJECT_SOURCE_DIR}/${libName})

# CMake files
set(cmakeFiles
	${componentPath}/CMakeLists.txt
)


# header files
set(headerFiles 
	${componentPath}/IDeserializer.h
	${componentPath}/ISerializable.h
	${componentPath}/ISerializer.h
	${componentPath}/Patterns.h
	${componentPath}/Subject.h
	${componentPath}/Singleton.h
)

# source files
set(sourceFiles
	${componentPath}/Patterns.cpp
	${componentPath}/Subject.cpp
)

# get all file groups together
set(libFiles
	${cmakeFiles}
	${headerFiles}
	${sourceFiles}
)

add_library(${libName} ${libFiles})

# define source groups for file management within IDE
source_group("CMake Files" FILES ${cmakeFiles})
source_group("Header Files" FILES ${headerFiles})
source_group("Source Files" FILES ${sourceFiles})