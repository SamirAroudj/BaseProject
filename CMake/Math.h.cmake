#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# extra CMake functionality concerning math component

macro(getMathLibName name doublePrecision)
	# target lib file name depends on double/single precision setting
	if (${doublePrecision})
		set(${name} MathDoublePrecision)
	else (${doublePrecision})
		set(${name} MathSinglePrecision)
	endif (${doublePrecision})
	#message(${name} ": " ${${name}})
endmacro(getMathLibName)

getMathLibName(mathLibName BASE_DOUBLE_PRECISION)
if (BASE_DOUBLE_PRECISION)
	add_definitions(-DDOUBLE_PRECISION)	
else (BASE_DOUBLE_PRECISION)
	add_definitions(-DSINGLE_PRECISION)
endif (BASE_DOUBLE_PRECISION)