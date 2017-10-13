#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# OpenGL specific cmake functionality

# OpenGL			
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIR})

# glut 				 
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

# glew                             
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

## inform preprocessor that glew is linked statically
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DGLEW_STATIC")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DGLEW_STATIC")

macro(addOpenGLLibs libs)
	set(${libs}
		${${libs}}
		${GLEW_LIBRARIES}
		${GLUT_LIBRARIES}
		${OPENGL_LIBRARIES}
	)
endmacro(addOpenGLLibs)
