#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# TinyXML2 specific cmake functionality

# cached path to external TinyXML2 project include directory
find_package(TINYXML2 REQUIRED)
include_directories(${TINYXML2_INCLUDE_DIR})
