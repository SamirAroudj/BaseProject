#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# CMake file to include CUDA

if (BASE_CUDA)
	find_package(CUDA REQUIRED)
	
	if (${LINUX})
		# enable c++11
		set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -std=c++11")
	endif (${LINUX})
	
	# enable debug symbols on device
	set(CUDA_NVCC_FLAGS_DEBUG "${CUDA_NVCC_FLAGS_DEBUG} -G")	

	#include CUDA headers
	include_directories(${CUDA_SDK_ROOT_DIR}/common/inc)
	include_directories(${CUDA_TOOLKIT_ROOT_DIR}/samples/common/inc)
	include_directories(${CUDA_TOOLKIT_ROOT_DIR}/include)
	
	#preprocessor flags
	add_definitions(-DCUDA)

endif (BASE_CUDA)