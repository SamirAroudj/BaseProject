# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

find_path(TINYXML2_INCLUDE_DIR NAMES tinyxml2.h)
find_library(TINYXML2_LIBRARY_RELEASE NAMES tinyxml2)
find_library(TINYXML2_LIBRARY_DEBUG NAMES tinyxml2d)

set(TINYXML2_LIBRARY_RELEASE "${TINYXML2_LIBRARY_RELEASE}" CACHE FILEPATH "Full name of tinyxml2 release library, e.g., /usr/lib/tinyxml2.so")
set(TINYXML2_LIBRARY_DEBUG "${TINYXML2_LIBRARY_DEBUG}" CACHE FILEPATH "Full name of tinyxml2 debug library, e.g., /usr/lib/tinyxml2d.so")

mark_as_advanced(TINYXML2_INCLUDE_DIR) 
mark_as_advanced(TINYXML2_LIBRARY_DEBUG)
mark_as_advanced(TINYXML2_LIBRARY_RELEASE)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TINYXML2 DEFAULT_MSG TINYXML2_LIBRARY_RELEASE TINYXML2_INCLUDE_DIR)
unset(TINYXML2_DIR CACHE)