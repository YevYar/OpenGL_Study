add_library(OpenGL_Study_Math_Core)

set(PUBLIC_HEADERS ${PATH_TO_PUBLIC_INCLUDE}/mathCore/base.h
    ${PATH_TO_PUBLIC_INCLUDE}/mathCore/point.h)
	
set(PRIVATE_HEADERS "")
	
set(SOURCES base.cpp)


target_sources(OpenGL_Study_Math_Core PRIVATE ${SOURCES} ${PUBLIC_HEADERS} ${PRIVATE_HEADERS})
target_include_directories(OpenGL_Study_Math_Core PUBLIC ${PATH_TO_PUBLIC_INCLUDE}/mathCore)
target_include_directories(OpenGL_Study_Math_Core PRIVATE include)
target_link_libraries(OpenGL_Study_Math_Core PRIVATE OpenGL_Study_compiler_flags
	OpenGL_Study_General
	OpenGL_Study_Helpers)


source_group(
	TREE "${PATH_TO_PUBLIC_INCLUDE}/mathCore"
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
