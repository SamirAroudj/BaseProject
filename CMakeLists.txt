#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# project BaseProject
# main CMakeLists.txt file
cmake_minimum_required(VERSION 2.8)

# Project definition
project(BaseProject)
set(BASE_PROJECT_DIR ${PROJECT_SOURCE_DIR} CACHE PATH "Path to BaseProject root directory.")

# extra CMake functionality
include(${PROJECT_SOURCE_DIR}/CMake/CommonProject.h.cmake)
include(${PROJECT_SOURCE_DIR}/CMake/PostBuild.h.cmake)

# CMake modules
set(cmakeModules
	${PROJECT_SOURCE_DIR}/CMake/CommonProject.h.cmake
	${PROJECT_SOURCE_DIR}/CMake/CUDA.h.cmake
	${PROJECT_SOURCE_DIR}/CMake/Git.h.cmake
	${PROJECT_SOURCE_DIR}/CMake/LibPNG.h.cmake
	${PROJECT_SOURCE_DIR}/CMake/Linux.h.cmake
	${PROJECT_SOURCE_DIR}/CMake/Math.h.cmake
	${PROJECT_SOURCE_DIR}/CMake/OpenGL.h.cmake
	${PROJECT_SOURCE_DIR}/CMake/PostBuild.h.cmake
	${PROJECT_SOURCE_DIR}/CMake/TinyXML2.h.cmake
	${PROJECT_SOURCE_DIR}/CMake/Windows.h.cmake
	${PROJECT_SOURCE_DIR}/CMakeLists.txt
)
source_group("CMake modules" FILES ${cmakeModules})

subdirs(
	#Audio
	#AudioTest
	CollisionDetection
	#GameClient
	Graphics
	#Graphics3D
	#Graphics3DTest
	GraphicsTest
	Math
	MathTest
	#Network
	Patterns
	Platform
	PlatformTest
	#RazzleDazzle
	#ServerTest
	Utilities
	UtilitiesTest
)