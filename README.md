# BaseProject
Provides basic features, such as window creation, memory management, math stuff, user input handling etc.

# Required Libraries
BaseProjects requires the following libraries for building:

- CMake (for easier building under windows and linux): https://cmake.org/
- glew (Modified BSD License, the Mesa 3-D License (MIT) and the Khronos License (MIT)): http://glew.sourceforge.net/
- glut (e.g.: freeglut 3.0.0, X-Consortium license): http://freeglut.sourceforge.net/
- libpng (http://www.libpng.org/pub/png/src/libpng-LICENSE.txt): http://www.libpng.org/pub/png/libpng.html
- OpenGL libraries
- tinyxml2 (ZLib license): https://github.com/leethomason/tinyxml2
- ZLib (license: https://zlib.net/zlib_license.html): https://zlib.net/

# Building for Windows
- download the source code with "git clone https://github.com/SamirAroudj/BaseProject.git" into some root folder path/BaseProject
- create a build folder within the BaseProject root directory: path/BaseProject/Build
- run CMake configure within path/BaseProject/Build as Build or binaries directory and path/BaseProject as source directory
- set GLUT_INCLUDE_DIR (e.g.: C:/Dev/3rdParty/freeglut/include)
- set GLUT_glut_LIBRARY (e.g.: C:/Dev/3rdParty/freeglut/lib/x64/freeglut.lib)
- set TINYXML2_DEBUG_LIBRARY (e.g.: C:/Dev/3rdParty/tinyxml2/Build/VS2013/Debug/tinyxml2.lib)
- set TINYXML2_RELEASE_LIBRARY (e.g.: C:/Dev/3rdParty/tinyxml2/Build/VS2013/Release/tinyxml2.lib)
- set TINYXML2_INCLUDE_DIR (e.g.: C:/Dev/3rdParty/tinyxml2/)
- run configure
- set GLEW_INCLUDE_DIR (e.g.: C:/Dev/3rdParty/glew-1.13.0/include)
- set GLEW_LIBRARY (e.g.: C:/Dev/3rdParty/glew-1.13.0/lib/Release/x64/glew32.lib)
- run configure
- set ZLIB_INCLUDE_DIR (e.g. C:/Dev/3rdParty/zlib-1.2.8)
- set ZLIB_LIBRARY_DEBUG (e.g. C:/Dev/3rdParty/zlib-1.2.8/Build/VS2013/Lib/Debug/zlibd.lib)
- set ZLIB_LIBRARY_RELEASE (e.g. C:/Dev/3rdParty/zlib-1.2.8/Build/VS2013/Lib/Release/zlib.lib)
- run configure
- set PNG_LIBRARY_DEBUG (e.g. C:/Dev/3rdParty/libpng-1.6.24/Build/VS2013/Debug/libpng16d.lib)
- set PNG_LIBRARY_RELEASE (e.g. C:/Dev/3rdParty/libpng-1.6.24/Build/VS2013/Release/libpng16.lib)
- set PNG_PNG_INCLUDE_DIR (e.g. C:/Dev/3rdParty/libpng-1.6.24/)
- run configure
- run generate
- have fun!
  
# Building for Linux
