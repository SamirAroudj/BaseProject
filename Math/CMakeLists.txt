#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# Math library
set(componentPath ${PROJECT_SOURCE_DIR}/Math)
# mathLibName is set by parent CMakeLists.txt

# CMake files
set(cmakeFiles
	${componentPath}/CMakeLists.txt
)

# header files
set(headerFiles 
	${componentPath}/BezierCurve.h
	${componentPath}/KernelsInterpolationsWeightings.h
	${componentPath}/LinearSolver.h
	${componentPath}/MathCore.h
	${componentPath}/MathHelper.h
	${componentPath}/Matrix2x2.h
	${componentPath}/Matrix3x3.h
	${componentPath}/Matrix4x4.h
	${componentPath}/Polybezier.h
	${componentPath}/Quaternion.h
	${componentPath}/Statistics.h
	${componentPath}/Vector2.h
	${componentPath}/Vector3.h
	${componentPath}/Vector4.h
)

# source files
set(sourceFiles
	${componentPath}/KernelsInterpolationsWeightings.cpp
	${componentPath}/LinearSolver.cpp
	${componentPath}/MathHelper.cpp
	${componentPath}/Matrix2x2.cpp
	${componentPath}/Matrix3x3.cpp
	${componentPath}/Matrix4x4.cpp
	${componentPath}/Polybezier.cpp
	${componentPath}/Quaternion.cpp
	${componentPath}/Statistics.cpp
	${componentPath}/Vector2.cpp
	${componentPath}/Vector3.cpp
	${componentPath}/Vector4.cpp
)

# get all file groups together
set(libFiles
	${cmakeFiles}
	${headerFiles}
	${sourceFiles}
)

add_library(${mathLibName} ${libFiles})

# define source groups for file management within IDE
source_group("CMake Files" FILES ${cmakeFiles})
source_group("Header Files" FILES ${headerFiles})
source_group("Source Files" FILES ${sourceFiles})