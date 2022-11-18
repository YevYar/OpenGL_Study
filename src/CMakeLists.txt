set(SOURCES main.cpp
	window.cpp
	shaderHelpers.cpp
	debugHelpers.cpp
	vertexBuffer.cpp
	vertexArray.cpp
	vertexBufferLayout.cpp
	vertexHelpers.cpp)

target_sources(OpenGL_Study_Exe PRIVATE ${SOURCES})

source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}"
	PREFIX "Source Files"
	FILES ${SOURCES})
