add_library(OpenGL_Study_Renderer)

set(PUBLIC_HEADERS ${PATH_TO_PUBLIC_INCLUDE}/renderer/material.h
    ${PATH_TO_PUBLIC_INCLUDE}/renderer/mesh.h 
    ${PATH_TO_PUBLIC_INCLUDE}/renderer/renderer.h
	${PATH_TO_PUBLIC_INCLUDE}/renderer/sceneObject.h)
	
set(PRIVATE_HEADERS )
	
set(SOURCES material.cpp
    mesh.cpp
    renderer.cpp
	sceneObject.cpp)


target_sources(OpenGL_Study_Renderer PRIVATE ${SOURCES} ${PUBLIC_HEADERS} ${PRIVATE_HEADERS})
target_include_directories(OpenGL_Study_Renderer PUBLIC ${PATH_TO_PUBLIC_INCLUDE}/renderer)
target_include_directories(OpenGL_Study_Renderer PRIVATE include)
target_link_libraries(OpenGL_Study_Renderer PRIVATE glad
	${GLFW3}
	OpenGL_Study_compiler_flags
	OpenGL_Study_General
	OpenGL_Study_Helpers
    OpenGL_Study_Math_Core
	OpenGL_Study_OpenGL_Core)


source_group(
	TREE "${PATH_TO_PUBLIC_INCLUDE}/renderer"
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
