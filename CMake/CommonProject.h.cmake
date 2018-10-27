#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# Do CMake stuff which is generally done in projects

# common user options
option(BASE_DOUBLE_PRECISION "Enables either double or single precision floating point variables." on)
option(BASE_CUDA "Enables CUDA code if set to true. (Some alternatives like OpenCL might be possible later.)" off)
option(BASE_GIT "Enables generation of git.cpp which can be used with git.h to automatically include git data in the code, e.g.,working branch, commit hash, etc" on)

option(BASE_LOGGING "Enables BASE_LOGGING" off)
mark_as_advanced(BASE_LOGGING)

option(BASE_PROFILING "Enables or disables profiling functionality." on)
mark_as_advanced(BASE_PROFILING)

# memory management user options
option(BASE_MEMORY_MANAGEMENT "Enables or disables the custom memory management of the base project." off)
option(BASE_MEMORY_MANAGEMENT_ACTIVE_MEMORY_DESTRUCTION "Enables overwriting of released memory with an uncommon pattern. Only works if MEMORY_MANAGEMENT is turned on." on)
option(BASE_MEMORY_MANAGEMENT_CORRECT_DELETE_OPERATOR_AND_BOUNDS_CHECK "Enables checking of correct usage of delete and delete [] and heap array bounds overwrite detection. Only works if MEMORY_MANAGEMENT is turned on." on)
mark_as_advanced(BASE_MEMORY_MANAGEMENT BASE_MEMORY_MANAGEMENT_ACTIVE_MEMORY_DESTRUCTION BASE_MEMORY_MANAGEMENT_CORRECT_DELETE_OPERATOR_AND_BOUNDS_CHECK)

# where to find built 3rd party dendencies
list(APPEND CMAKE_MODULE_PATH ${BASE_PROJECT_DIR}/CMake)
set(CMAKE_INSTALL_PREFIX CACHE PATH "Path to custom built dependencies.")

# common user defined build paths
if (NOT(CMAKE_PROJECT_NAME STREQUAL BaseProject))
	set(BASE_PROJECT_BUILD_DIR "${BASE_PROJECT_DIR}/Build" CACHE PATH "Path to BaseProject build directory containing build type (release, debug) folders with libraries.")
endif (NOT(CMAKE_PROJECT_NAME STREQUAL BaseProject))
set(BASE_BUILD_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/Build" CACHE FILEPATH "Path to the folder defining where to put Release, Debug, etc, folders with binaries.")

# inferred build path settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_BUILD_OUTPUT_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BASE_BUILD_OUTPUT_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BASE_BUILD_OUTPUT_DIR}/${CMAKE_BUILD_TYPE})

# common preprocessor flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

# common CMake files
set(cmakeFiles
	${cmakeFiles}
	${PROJECT_SOURCE_DIR}/CMakeLists.txt
)
source_group("CMake Files" FILES ${cmakeFiles})

include(${BASE_PROJECT_DIR}/CMake/Linux.h.cmake)
include(${BASE_PROJECT_DIR}/CMake/Windows.h.cmake)
include(${BASE_PROJECT_DIR}/CMake/FileGeneration/FileGeneration.h.cmake)
include(${BASE_PROJECT_DIR}/CMake/Math.h.cmake)
include(${BASE_PROJECT_DIR}/CMake/TinyXML2.h.cmake)
include(${BASE_PROJECT_DIR}/CMake/Documentation.h.cmake)
include(${BASE_PROJECT_DIR}/CMake/Git.h.cmake) #must be after FileGeneration.h.cmake
include(${BASE_PROJECT_DIR}/CMake/CUDA.h.cmake) #must be after Linux.h.cmake and Windows.h.cmake

# extra common files
set(extraFiles
	todo.txt
)
source_group("Extra Files" FILES ${extraFiles})

if (NOT(CMAKE_PROJECT_NAME STREQUAL BaseProject))
	include_directories(${BASE_PROJECT_DIR})
	link_directories(${BASE_PROJECT_BUILD_DIR}/${CMAKE_BUILD_TYPE})
endif (NOT(CMAKE_PROJECT_NAME STREQUAL BaseProject))

# include internal project source files and libraries
include_directories(${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/Build/${CMAKE_BUILD_TYPE})

# common preprocessor flags
add_definitions(-DUNICODE)

# process common user options
if (BASE_PROFILING)
	add_definitions(-DPROFILING)
endif (BASE_PROFILING)

if (BASE_MEMORY_MANAGEMENT)
	add_definitions(-DMEMORY_MANAGEMENT)
endif (BASE_MEMORY_MANAGEMENT)

if (BASE_MEMORY_MANAGEMENT_ACTIVE_MEMORY_DESTRUCTION)
	add_definitions(-DMEMORY_MANAGEMENT_ACTIVE_MEMORY_DESTRUCTION)
endif (BASE_MEMORY_MANAGEMENT_ACTIVE_MEMORY_DESTRUCTION)

if (BASE_MEMORY_MANAGEMENT_CORRECT_DELETE_OPERATOR_AND_BOUNDS_CHECK)
	add_definitions(-DCORRECT_DELETE_OPERATOR_AND_BOUNDS_CHECK)
endif (BASE_MEMORY_MANAGEMENT_CORRECT_DELETE_OPERATOR_AND_BOUNDS_CHECK)

if (BASE_LOGGING)
	add_definitions(-DBASE_LOGGING)
endif (BASE_LOGGING)
