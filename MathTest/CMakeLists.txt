#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# test executable for math library
set(componentName MathTest)
set(appName ${componentName}.exe)
set(componentPath ${PROJECT_SOURCE_DIR}/${componentName})

# define executable
add_executable(${appName} main.cpp)

# link libraries
target_link_libraries(${appName}
	${mathLibName}
)

# dependencies
add_dependencies(${appName} ${mathLibName})