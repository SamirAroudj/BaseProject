#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# post build CMake functionality

macro(slashToBackslash output input)
	string(REPLACE "/" "\\" ${output} ${input})
endmacro(slashToBackslash)

# A macro for copying a file to the binaries directory of the project
macro(copyToBinDir targetName file)
	if (WIN32)
		slashToBackslash(windowsFile ${file})
		add_custom_command(
		   TARGET ${targetName}
		   POST_BUILD
		   COMMAND xcopy /r /y \"${windowsFile}\" \"$\(TargetDir\)\"
		)                        
	endif (WIN32)
endmacro(copyToBinDir)	


# xcopy /r /y (\r means replace \y means don't ask when something is replaced, xcopy needs windows paths with backslash in "")