set(SOURCES main.cpp
	window.cpp
	buffer.cpp
	vertexArray.cpp
	vertexBufferLayout.cpp
	shaderProgram.cpp
	uniforms.cpp
	helpers/helpers.cpp
	helpers/debugHelpers.cpp
	sceneObject.cpp
	generalTypes.cpp
	multicoloredRectangle.cpp
    texture.cpp
    textureTypes.cpp
    openglLimits.cpp
	textureUnit.cpp)

target_sources(OpenGL_Study_Exe PRIVATE ${SOURCES})

source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}"
	PREFIX "Source Files"
	FILES ${SOURCES})
