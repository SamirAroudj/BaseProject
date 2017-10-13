#
# Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD 3-Clause license. See the License.txt file for details.
#

# graphics library
set(libName Graphics)
set(componentPath ${PROJECT_SOURCE_DIR}/${libName})

# CMake files
set(cmakeFiles
	${componentPath}/CMakeLists.txt
)
source_group("CMake Files" FILES ${cmakeFiles})

# header files
set(headerFiles
	${componentPath}/Camera.h
	${componentPath}/Color.h
	${componentPath}/ColorMaterial.h
	${componentPath}/DiffTexMaterial.h
	${componentPath}/DirectionalLight.h
	${componentPath}/Font.h
	${componentPath}/FontManager.h
	${componentPath}/FrameStatistics.h
	${componentPath}/GraphicsConfigurationLoader.h
	${componentPath}/GraphicsManager.h
	${componentPath}/IRenderable.h
	${componentPath}/Light.h
	${componentPath}/LightTexMaterial.h
	${componentPath}/LightTexShader.h
	${componentPath}/MagicGraphicsConstants.h
	${componentPath}/Material.h
	${componentPath}/PointLight.h
	${componentPath}/Scene.h
	${componentPath}/SceneNode.h
	${componentPath}/Shader.h
	${componentPath}/SpotLight.h
	${componentPath}/StaticMesh.h
	${componentPath}/StaticObject.h
	${componentPath}/Texture.h
	${componentPath}/TextureShader.h
	${componentPath}/Vertex.h
	${componentPath}/VertexLayout.h
	${componentPath}/ViewportDescription.h
)
source_group("Header Files" FILES ${headerFiles})

# source files
set(sourceFiles
	${componentPath}/Camera.cpp
	${componentPath}/Color.cpp
	${componentPath}/ColorMaterial.cpp
	${componentPath}/DiffTexMaterial.cpp
	${componentPath}/DirectionalLight.cpp
	${componentPath}/Font.cpp
	${componentPath}/FontManager.cpp
	${componentPath}/FrameStatistics.cpp
	${componentPath}/GraphicsConfigurationLoader.cpp
	${componentPath}/GraphicsManager.cpp
	${componentPath}/Light.cpp
	${componentPath}/LightTexMaterial.cpp
	${componentPath}/LightTexShader.cpp
	${componentPath}/MagicGraphicsConstants.cpp
	${componentPath}/Material.cpp
	${componentPath}/PointLight.cpp
	${componentPath}/Scene.cpp
	${componentPath}/SceneNode.cpp
	${componentPath}/Shader.cpp
	${componentPath}/SpotLight.cpp
	${componentPath}/StaticMesh.cpp
	${componentPath}/StaticObject.cpp
	${componentPath}/Texture.cpp
	${componentPath}/TextureShader.cpp
	${componentPath}/VertexLayout.cpp
	${componentPath}/ViewportDescription.cpp
)
source_group("Source Files" FILES ${sourceFiles})

# get all file groups together
set(libFiles
	${cmakeFiles}
	${headerFiles}
	${sourceFiles}
)

add_library(${libName} ${libFiles})