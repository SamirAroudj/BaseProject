#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# Platform library
set(componentName Platform)

# set paths to source folders
set(componentPath ${PROJECT_SOURCE_DIR}/${componentName})
set(failureHandlingPath ${componentPath}/FailureHandling)
set(inputPath ${componentPath}/Input)
set(loggingPath ${componentPath}/Logging)
set(multithreadingPath ${componentPath}/Multithreading)
set(resourceManagementPath ${componentPath}/ResourceManagement)
set(storagePath ${componentPath}/Storage)

# CMake files
set(cmakeFiles
	${componentPath}/CMakeLists.txt
)

# header files
set(headerFiles 
	${componentPath}/Application.h
	${componentPath}/ApplicationTimer.h
	${componentPath}/DataTypes.h
	${componentPath}/FrameRateCalculator.h
	${componentPath}/IWindowObserver.h
	${componentPath}/MagicConstants.h
	${componentPath}/ParametersManager.h
	${componentPath}/Platform.h
	${componentPath}/TimePeriod.h
	${componentPath}/Window.h
)

# source files
set(sourceFiles
	${componentPath}/Application.cpp
	${componentPath}/ApplicationTimer.cpp
	${componentPath}/DataTypes.cpp
	${componentPath}/FrameRateCalculator.cpp
	${componentPath}/MagicConstants.cpp
	${componentPath}/ParametersManager.cpp
	${componentPath}/TimePeriod.cpp
	${componentPath}/Window.cpp
)

# failure handling header files
set(failureHandlingHeaderFiles
	${failureHandlingPath}/AudioException.h
	${failureHandlingPath}/ClosedConnectionException.h
	${failureHandlingPath}/ConnectionCreationException.h
	${failureHandlingPath}/DirectoryAccessException.h
	${failureHandlingPath}/Exception.h
	${failureHandlingPath}/FailureHandling.h
	${failureHandlingPath}/FileAccessException.h	
	${failureHandlingPath}/FileCorruptionException.h
	${failureHandlingPath}/FileException.h
	${failureHandlingPath}/FileVersionException.h
	${failureHandlingPath}/GraphicsException.h
	${failureHandlingPath}/HostNameResolutionException.h
	${failureHandlingPath}/InputException.h
	${failureHandlingPath}/InvalidSocketException.h
	${failureHandlingPath}/NetworkException.h
	${failureHandlingPath}/NetworkExceptionFactory.h
	${failureHandlingPath}/SocketBindException.h
	${failureHandlingPath}/WindowException.h
)

# failure handling source files
set(failureHandlingSourceFiles
	${failureHandlingPath}/AudioException.cpp
	${failureHandlingPath}/ClosedConnectionException.cpp
	${failureHandlingPath}/ConnectionCreationException.cpp
	${failureHandlingPath}/DirectoryAccessException.cpp
	${failureHandlingPath}/Exception.cpp
	${failureHandlingPath}/FailureHandling.cpp
	${failureHandlingPath}/FileAccessException.cpp
	${failureHandlingPath}/FileCorruptionException.cpp	
	${failureHandlingPath}/FileException.cpp
	${failureHandlingPath}/FileVersionException.cpp
	${failureHandlingPath}/GraphicsException.cpp
	${failureHandlingPath}/HostNameResolutionException.cpp
	${failureHandlingPath}/InputException.cpp
	${failureHandlingPath}/InvalidSocketException.cpp
	${failureHandlingPath}/NetworkException.cpp
	${failureHandlingPath}/NetworkExceptionFactory.cpp
	${failureHandlingPath}/SocketBindException.cpp
	${failureHandlingPath}/WindowException.cpp
)

if (BASE_CUDA)
	set(failureHandlingSourceFiles
		${failureHandlingSourceFiles}
		${failureHandlingPath}/FailureHandling.cu
	)
endif (BASE_CUDA)

# input header files
set(inputHeaderFiles
	${inputPath}/GameController.h
	${inputPath}/InputManager.h
	${inputPath}/Keyboard.h
	${inputPath}/KeyboardMapping.h
	${inputPath}/MagicConstants.h
	${inputPath}/Mouse.h
	${inputPath}/SomeController.h
	${inputPath}/TextInput.h
	${inputPath}/Xbox360Controller.h
)

# input source files
set(inputSourceFiles
	${inputPath}/GameController.cpp
	${inputPath}/InputManager.cpp
	${inputPath}/Keyboard.cpp
	${inputPath}/Mouse.cpp
	${inputPath}/SomeController.cpp
	${inputPath}/TextInput.cpp
	${inputPath}/Xbox360Controller.cpp
)

# logging header files
set(loggingHeaderFiles
	${loggingPath}/ConsoleLogOutput.h
	${loggingPath}/FileLogOutput.h
	${loggingPath}/LogManager.h
	${loggingPath}/LogOutput.h
)

# logging source files
set(loggingSourceFiles
	${loggingPath}/ConsoleLogOutput.h
	${loggingPath}/FileLogOutput.h
	${loggingPath}/LogManager.h
	${loggingPath}/LogOutput.h
)

# multithreading header files
set(multithreadingHeaderFiles
	${multithreadingPath}/Manager.h
	${multithreadingPath}/Task.h
)

# multithreading source files
set(multithreadingSourceFiles
	${multithreadingPath}/Manager.cpp
	${multithreadingPath}/Task.cpp
)

# resource management header files
set(resourceManagementHeaderFiles
	${resourceManagementPath}/Bucket.h
	${resourceManagementPath}/MemoryManager.h
	${resourceManagementPath}/MemoryPool.h
	${resourceManagementPath}/Resource.h
	${resourceManagementPath}/MagicConstants.h
	${resourceManagementPath}/UserResource.h
	${resourceManagementPath}/VolatileResource.h
)

# resource management source files
set(resourceManagementSourceFiles
	${resourceManagementPath}/Bucket.cpp
	${resourceManagementPath}/MemoryManager.cpp
	${resourceManagementPath}/MemoryPool.cpp
)

# storage header files
set(storageHeaderFiles
	${storagePath}/Directory.h
	${storagePath}/File.h
	${storagePath}/Path.h
)

# storage source files
set(storageSourceFiles
	${storagePath}/Directory.cpp
	${storagePath}/File.cpp
	${storagePath}/Path.cpp
)

# get all file groups together
set(sourceCode
	${cmakeFiles}
	${failureHandlingHeaderFiles}
	${failureHandlingSourceFiles}
	${headerFiles}
	${inputHeaderFiles}
	${inputSourceFiles}
	${loggingHeaderFiles}
	${loggingSourceFiles}
	${multithreadingHeaderFiles}
	${multithreadingSourceFiles}
	${resourceManagementHeaderFiles}
	${resourceManagementSourceFiles}
	${sourceFiles}
	${storageHeaderFiles}
	${storageSourceFiles}
)

if (BASE_CUDA)
	cuda_add_library(${componentName} ${sourceCode})
else (BASE_CUDA)
	add_library(${componentName} ${sourceCode})
endif (BASE_CUDA)


# define source groups for file management within IDE
source_group("CMake Files" FILES ${cmakeFiles})

# header file groups
source_group("Header Files" FILES ${headerFiles})
source_group("Header Files\\Failure Handling" FILES ${failureHandlingHeaderFiles})
source_group("Header Files\\Input" FILES ${inputHeaderFiles})
source_group("Header Files\\Logging" FILES ${loggingHeaderFiles})
source_group("Header Files\\Multithreading" FILES ${multithreadingHeaderFiles})
source_group("Header Files\\Resource Management" FILES ${resourceManagementHeaderFiles})
source_group("Header Files\\Storage" FILES ${storageHeaderFiles})

# source file groups
source_group("Source Files" FILES ${sourceFiles})
source_group("Source Files\\Failure Handling" FILES ${failureHandlingSourceFiles})
source_group("Source Files\\Input" FILES ${inputSourceFiles})
source_group("Source Files\\Logging" FILES ${loggingSourceFiles})
source_group("Source Files\\Multithreading" FILES ${multithreadingSourceFiles})
source_group("Source Files\\Resource Management" FILES ${resourceManagementSourceFiles})
source_group("SOurce Files\\Storage" FILES ${storageSourceFiles})
