#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# CMake file to include automated Git data generation

if (BASE_GIT)

	# write current working branch to gitBranch
	execute_process(
		COMMAND git rev-parse --abbrev-ref HEAD
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE gitBranch
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	# write latest commit hash of working branch to gitCommitHash
	execute_process(
		COMMAND git log -1 --format=%H
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE gitCommitHash
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	# generate source file containing git data
	message(STATUS "Generating git source file in ${FILE_GENERATION_OUTPUT_DIR}")
	configure_file(${FILE_GENERATION_INPUT_DIR}/Git.cpp.in ${FILE_GENERATION_OUTPUT_DIR}/Git.cpp @ONLY)
	set(generatedSourceFiles ${generatedSourceFiles} ${FILE_GENERATION_OUTPUT_DIR}/Git.cpp)

endif (BASE_GIT)