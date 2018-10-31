# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# add stuff required for using Intel's ray tracing framework  embree

find_package(EMBREE 2.17.6 REQUIRED)

set(EMBREE_INCLUDE_DIR "${EMBREE_INCLUDE_DIRS}" CACHE PATH "Path to the embree include directory.")
set(EMBREE_LIBRARY_DEBUG "${EMBREE_LIBRARY}" CACHE FILEPATH "Full name of the embree debug library, e.g., /usr/lib/embreed.so")
set(EMBREE_LIBRARY_RELEASE "${EMBREE_LIBRARY}" CACHE FILEPATH "Full name of the embree release library, e.g., /usr/lib/embree.so")
mark_as_advanced(EMBREE_INCLUDE_DIR EMBREE_LIBRARY_DEBUG EMBREE_LIBRARY_RELEASE)
mark_as_advanced(EMBREE_DIR)

include_directories(${EMBREE_INCLUDE_DIR})

macro(addEmbreeLibs libs)
	set(${libs}
		${${libs}}
		"debug ${EMBREE_LIBRARY_DEBUG}"
		"optimized ${EMBREE_LIBRARY_RELEASE}"
	)
endmacro(addEmbreeLibs)

