#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# Utilities library
set(libName Utilities)
set(componentPath ${PROJECT_SOURCE_DIR}/${libName})
set(profilingPath ${componentPath}/Profiling)

# CMake files
set(cmakeFiles
	${componentPath}/CMakeLists.txt
)

# header files
set(headerFiles
	${componentPath}/Conversions.h
	${componentPath}/HelperFunctions.h
	${componentPath}/Licenser.h
	${componentPath}/MagicConstants.h
	${componentPath}/PlyFile.h
	${componentPath}/Size2.h
	${componentPath}/RandomManager.h
	${componentPath}/RectanglePacker.h
	${componentPath}/RegularExpression.h
	${componentPath}/SVGLoader.h
)

# source files
set(sourceFiles
	${componentPath}/Conversions.cpp
	${componentPath}/HelperFunctions.cpp
	${componentPath}/Licenser.cpp
	${componentPath}/PlyFile.cpp
	${componentPath}/RandomManager.cpp
	${componentPath}/RectanglePacker.cpp
	${componentPath}/RegularExpression.cpp
	${componentPath}/SVGLoader.cpp
)

# profiling header files
set(profilingHeaderFiles
	${profilingPath}/Profiler.h
	${profilingPath}/TimeMeasurements.h
)

if (BASE_CUDA)
	set(profilingHeaderFiles
		${profilingHeaderFiles}
		${profilingPath}/CUDAProfiler.h
	)
endif (BASE_CUDA)

# profiling source files
set(profilingSourceFiles
	${profilingPath}/Profiler.cpp
	${profilingPath}/TimeMeasurements.cpp
)

if (BASE_CUDA)
	set(profilingSourceFiles
		${profilingSourceFiles}
		${profilingPath}/CUDAProfiler.cu
	)
endif (BASE_CUDA)


# get all file groups together
set(libFiles
	${cmakeFiles}
	${headerFiles}
	${profilingHeaderFiles}
	${profilingSourceFiles}
	${sourceFiles}
)

if (BASE_CUDA)
	cuda_add_library(${libName} ${libFiles})
else (BASE_CUDA)
	add_library(${libName} ${libFiles})
endif (BASE_CUDA)

# define source groups for file management within IDE
source_group("CMake Files" FILES ${cmakeFiles})

# header file groups
source_group("Header Files" FILES ${headerFiles})
source_group("Header Files\\Profiling" FILES ${profilingHeaderFiles})

# source file groups
source_group("Source Files" FILES ${sourceFiles})
source_group("Source Files\\Profiling" FILES ${profilingSourceFiles})
