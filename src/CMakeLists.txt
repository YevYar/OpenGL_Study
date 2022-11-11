set(SOURCES main.cpp
	windowHandlers.cpp
	shaderHelpers.cpp
	debugHelpers.cpp)

target_sources(OpenGL_Study_Exe PRIVATE ${SOURCES})

source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}"
	PREFIX "Source Files"
	FILES ${SOURCES})
