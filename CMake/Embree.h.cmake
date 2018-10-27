# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# add stuff required for using Intel's ray tracing framework  embree

find_package(embree 3.0.0 REQUIRED)

include_directories(${EMBREE_INCLUDE_DIR})

set(EMBREE_INCLUDE_DIR "${EMBREE_INCLUDE_DIRS}" CACHE PATH "Path to the embree include directory.")
set(EMBREE_LIBRARY_DEBUG "${EMBREE_LIBRARY}" CACHE FILEPATH "Path to the embree debug library.")
set(EMBREE_LIBRARY_RELEASE "${EMBREE_LIBRARY}" CACHE FILEPATH "Path to the embree release library.")

mark_as_advanced(EMBREE_INCLUDE_DIR EMBREE_LIBRARY_DEBUG EMBREE_LIBRARY_RELEASE)

macro(addEmbreeLibs libs)
	set(${libs}
		${${libs}}
		"debug ${EMBREE_LIBRARY_DEBUG}"
		"optimized ${EMBREE_LIBRARY_RELEASE}"
	)
endmacro(addEmbreeLibs)

