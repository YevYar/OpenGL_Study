add_library(OpenGL_Study_OpenGL_Core)

set(PUBLIC_HEADERS ${PATH_TO_PUBLIC_INCLUDE}/openglCore/buffer.h
	${PATH_TO_PUBLIC_INCLUDE}/openglCore/openglLimits.h
	${PATH_TO_PUBLIC_INCLUDE}/openglCore/shaderProgram.h
	${PATH_TO_PUBLIC_INCLUDE}/openglCore/texture.h
	${PATH_TO_PUBLIC_INCLUDE}/openglCore/textureTypes.h
	${PATH_TO_PUBLIC_INCLUDE}/openglCore/textureUnit.h
	${PATH_TO_PUBLIC_INCLUDE}/openglCore/uniforms.h
	${PATH_TO_PUBLIC_INCLUDE}/openglCore/vertexArray.h
	${PATH_TO_PUBLIC_INCLUDE}/openglCore/vertexBufferLayout.h
    ${PATH_TO_PUBLIC_INCLUDE}/openglCore/vertexTypes.h)
	
set(PRIVATE_HEADERS bufferImpl.h
	openglHelpers.h
    shaderProgramImpl.h
    include/textureImpl.h)
	
set(SOURCES buffer.cpp
	openglHelpers.cpp
	openglLimits.cpp
	shaderProgram.cpp
	texture.cpp
	textureTypes.cpp
	textureUnit.cpp
	uniforms.cpp
	vertexArray.cpp
	vertexBufferLayout.cpp)


target_sources(OpenGL_Study_OpenGL_Core PRIVATE ${SOURCES} ${PUBLIC_HEADERS} ${PRIVATE_HEADERS})
target_include_directories(OpenGL_Study_OpenGL_Core PUBLIC ${PATH_TO_PUBLIC_INCLUDE}/openglCore)
target_include_directories(OpenGL_Study_OpenGL_Core PRIVATE include)
target_link_libraries(OpenGL_Study_OpenGL_Core PRIVATE OpenGL_Study_compiler_flags
	OpenGL_Study_general_external_libs
	OpenGL_Study_General
	OpenGL_Study_Helpers)


source_group(
	TREE "${PATH_TO_PUBLIC_INCLUDE}/openglCore"
	PREFIX "Public Header Files"
	FILES ${PUBLIC_HEADERS})
	
source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}"
	PREFIX "Private Header Files"
	FILES ${PRIVATE_HEADERS})
	
source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}"
	PREFIX "Source Files"
	FILES ${SOURCES})
