add_library(OpenGL_Study_Helpers)


set(HEADERS ${PATH_TO_PUBLIC_INCLUDE}/helpers/debugHelpers.h
    ${PATH_TO_PUBLIC_INCLUDE}/helpers/floats.h
	${PATH_TO_PUBLIC_INCLUDE}/helpers/helpers.h
	${PATH_TO_PUBLIC_INCLUDE}/helpers/macros.h
    ${PATH_TO_PUBLIC_INCLUDE}/helpers/openglHelpers.h)
	
set(SOURCES debugHelpers.cpp
	helpers.cpp
    openglHelpers.cpp)


target_sources(OpenGL_Study_Helpers PRIVATE ${SOURCES} ${HEADERS})
target_include_directories(OpenGL_Study_Helpers PUBLIC ${PATH_TO_PUBLIC_INCLUDE}/helpers ${PATH_TO_LIBS_INCLUDE}/debugbreak)
target_link_libraries(OpenGL_Study_Helpers PRIVATE OpenGL_Study_compiler_flags
	OpenGL_Study_general_external_libs
	OpenGL_Study_General
	OpenGL_Study_OpenGL_Core)
	

source_group(
	TREE "${PATH_TO_PUBLIC_INCLUDE}/helpers"
	PREFIX "Header Files"
	FILES ${HEADERS})
	
source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}"
	PREFIX "Source Files"
	FILES ${SOURCES})
