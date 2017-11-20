#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# 2D graphics library
set(libName Graphics)
set(componentPath ${PROJECT_SOURCE_DIR}/${libName})

#include OpenGL, glut and glew stuff
include(${PROJECT_SOURCE_DIR}/CMake/OpenGL.h.cmake)

#include lib png stuff
include(${PROJECT_SOURCE_DIR}/CMake/LibPNG.h.cmake)

# CMake files
set(cmakeFiles
	${componentPath}/CMakeLists.txt
)

# header files
set(headerFiles 
	${componentPath}/AnimatedSprite.h
	${componentPath}/BaseCamera3D.h
	${componentPath}/Camera2D.h
	${componentPath}/Camera3D.h
	${componentPath}/Color.h
	${componentPath}/ColorMap.h
	${componentPath}/ElementsDescription.h
	${componentPath}/FacesDescription.h
	${componentPath}/GraphicsManager.h
	${componentPath}/Helper.h
	${componentPath}/ImageManager.h
	${componentPath}/IRenderable.h
	${componentPath}/MagicConstants.h
	${componentPath}/RenderGroup.h
	${componentPath}/PinholeCamera.h
	${componentPath}/Sprite.h
	${componentPath}/Texture.h
	${componentPath}/TextureHeader.h
	${componentPath}/VerticesDescription.h
	${componentPath}/Viewport.h
)

# source files
set(sourceFiles
	${componentPath}/AnimatedSprite.cpp
	${componentPath}/BaseCamera3D.cpp
	${componentPath}/Camera2D.cpp
	${componentPath}/Camera3D.cpp
	${componentPath}/Color.cpp
	${componentPath}/ColorMap.cpp
	${componentPath}/ElementsDescription.cpp
	${componentPath}/FacesDescription.cpp
	${componentPath}/GraphicsManager.cpp
	${componentPath}/Helper.cpp
	${componentPath}/ImageManager.cpp
	${componentPath}/MagicConstants.cpp
	${componentPath}/RenderGroup.cpp
	${componentPath}/PinholeCamera.cpp
	${componentPath}/Sprite.cpp
	${componentPath}/Texture.cpp
	${componentPath}/VerticesDescription.cpp
	${componentPath}/Viewport.cpp
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