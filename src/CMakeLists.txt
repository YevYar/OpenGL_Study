add_library(OpenGL_Study_General)


# Define variable with path to project include folder
set(PATH_TO_PUBLIC_INCLUDE ${OpenGL_Study_SOURCE_DIR}/include)

# Define variable with path to libs include folder
set(PATH_TO_LIBS_INCLUDE ${OpenGL_Study_SOURCE_DIR}/libs/include)

set(HEADERS ${PATH_TO_PUBLIC_INCLUDE}/exceptions.h
	${PATH_TO_PUBLIC_INCLUDE}/generalTypes.h
	${PATH_TO_PUBLIC_INCLUDE}/window.h)
	
set(SOURCES generalTypes.cpp
	window.cpp)


# Add interface library to link all necessary external libs
add_library(OpenGL_Study_general_external_libs INTERFACE)
target_link_libraries(OpenGL_Study_general_external_libs INTERFACE OpenGL::GL glad)
target_include_directories(OpenGL_Study_general_external_libs INTERFACE ${PATH_TO_LIBS_INCLUDE})


target_sources(OpenGL_Study_General PRIVATE ${SOURCES} ${HEADERS})
target_include_directories(OpenGL_Study_General PUBLIC ${PATH_TO_PUBLIC_INCLUDE})
target_link_libraries(OpenGL_Study_General PRIVATE ${GLFW3}
	OpenGL_Study_compiler_flags
	OpenGL_Study_general_external_libs
	OpenGL_Study_Helpers)


source_group(
	TREE "${PATH_TO_PUBLIC_INCLUDE}"
	PREFIX "Header Files"
	FILES ${HEADERS})
	
source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}"
	PREFIX "Source Files"
	FILES ${SOURCES})


add_subdirectory(helpers)
add_subdirectory(mathCore)
add_subdirectory(openglCore)
