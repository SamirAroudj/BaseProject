#find tinyxml2 header file, debug and release library
find_path(TINYXML2_INCLUDE_DIR NAMES tinyxml2.h)

find_library(TINYXML2_DEBUG_LIBRARY NAMES tinyxml2d)
find_library(TINYXML2_RELEASE_LIBRARY NAMES tinyxml2)
set(TINYXML2_LIBRARIES TINYXML2_DEBUG_LIBRARY TINYXML2_RELEASE_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TINYXML2 DEFAULT_MSG TINYXML2_LIBRARIES TINYXML2_INCLUDE_DIR)

mark_as_advanced(TINYXML2_INCLUDE_DIR ${TINYXML2_LIBRARIES})
