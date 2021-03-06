# BaseProject
Provides basic features, such as window creation, memory management, math stuff, user input handling etc.

# Conventions
- coordinate systems are right-handed. Let b0, b1, b2 be coordinate system base vectors, then b2 = b0 x b1 holds.
- triangle normals follow the right-hand rule. For triangle t = (p0, p1, p2), its normal is parallel to o = (p1 - p0) x (p2 - p0).
- cameras look along the negative z-axis. The x-axis goes to the right-hand side and the y-axis goes up (right-handed camera systems).
- vectors are considered to be in row layout. Matrices are correspondingly build for multiplication by row vectors (DirectX convention). This is in contrast to OpenGL and university linear algebra. If you are used to v' = M * N * v, here it is: v' = v * N^t * M^t with matrices M, N, their transposes M^t, N^t and  original and result vectors v and v'.

# Required Tools and Libraries
BaseProjects requires the following libraries for building:

- CMake (for easier building under windows and linux, get also ccmake for linux): https://cmake.org/
- glew (Modified BSD License, the Mesa 3-D License (MIT) and the Khronos License (MIT)): http://glew.sourceforge.net/
- glut (e.g.: freeglut 3.0.0, X-Consortium license): http://freeglut.sourceforge.net/
- libpng (http://www.libpng.org/pub/png/src/libpng-LICENSE.txt): http://www.libpng.org/pub/png/libpng.html
- OpenGL libraries
- tinyxml2 (ZLib license): https://github.com/leethomason/tinyxml2
- ZLib (license: https://zlib.net/zlib_license.html): https://zlib.net/

# Building for Windows
- Do not use relative paths with CMake!
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
- set ZLIB_INCLUDE_DIR (e.g.: C:/Dev/3rdParty/zlib-1.2.8)
- set ZLIB_LIBRARY_DEBUG (e.g.: C:/Dev/3rdParty/zlib-1.2.8/Build/VS2013/Lib/Debug/zlibd.lib)
- set ZLIB_LIBRARY_RELEASE (e.g.: C:/Dev/3rdParty/zlib-1.2.8/Build/VS2013/Lib/Release/zlib.lib)
- run configure
- set PNG_LIBRARY_DEBUG (e.g.: C:/Dev/3rdParty/libpng-1.6.24/Build/VS2013/Debug/libpng16d.lib)
- set PNG_LIBRARY_RELEASE (e.g.: C:/Dev/3rdParty/libpng-1.6.24/Build/VS2013/Release/libpng16.lib)
- set PNG_PNG_INCLUDE_DIR (e.g.: C:/Dev/3rdParty/libpng-1.6.24/;C:/Dev/3rdParty/libpng-1.6.24/Build/VS2013)
  Note that it is important here to point to the libpng source folder AND the build folder containing the header file pnglibconf.h!
- run configure
- run generate
- have fun building the code with a toolchain of your liking! (e.g. visual studio and path/BaseProject/Build/BaseProject.sln)
  
# Building for Linux
- Do not use relative paths with CMake!
- download the source code with "git clone https://github.com/SamirAroudj/BaseProject.git" into some root folder path/BaseProject
- create a build folder within the BaseProject root directory: path/BaseProject/Build
- create a sub build folder for each target you want to build, e.g., path/BaseProject/Build/Debug or path/BaseProject/Build/Release
- within the sub folder Build/target run "ccmake ../.."
- run configure
- set the CMAKE_BUILD_TYPE to your target configuration, e.g., Debug or Release
- set TINYXML2_DEBUG_LIBRARY   (e.g.: somePath/Repos/tinyxml2/Build/Debug/libtinyxml2.so.3.0.0)
- set TINYXML2_RELEASE_LIBRARY (e.g.: somePath/Repos/tinyxml2/Build/Release/libtinyxml2.so.3.0.0)
- set TINYXML2_INCLUDE_DIR     (e.g.: somePath/Repos/tinyxml2 )
- run configure
- run generate
- have fun building the code with a toolchain of your liking! (e.g., simply "make -j" within the build target configuration folder or using QtCreator or ...)
