#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# automatic documentation generation by means of the tool Doxygen

# code documentation: extra target to generate API documentation with Doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
	message(STATUS "Generating file in ${FILE_GENERATION_OUTPUT_DIR} for documentation using doxygen.")
	configure_file("${FILE_GENERATION_INPUT_DIR}/Doxyfile.in" "${FILE_GENERATION_OUTPUT_DIR}/Doxyfile" @ONLY)

	add_custom_target(
		Documentation
		${DOXYGEN_EXECUTABLE} ${FILE_GENERATION_OUTPUT_DIR}/Doxyfile
		WORKING_DIRECTORY ${FILE_GENERATION_INPUT_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
	)
endif (DOXYGEN_FOUND)