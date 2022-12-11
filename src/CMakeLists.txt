set(SOURCES main.cpp
	window.cpp
	helpers.cpp
	debugHelpers.cpp
	buffer.cpp
	vertexArray.cpp
	vertexBufferLayout.cpp
	shaderProgram.cpp
	uniforms.cpp)

target_sources(OpenGL_Study_Exe PRIVATE ${SOURCES})

source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}"
	PREFIX "Source Files"
	FILES ${SOURCES})
