add_executable(OpenGL_Study_Exe)


set(HEADERS multicoloredRectangle.h
	renderer.h
	sceneObject.h)
	
set(SOURCES main.cpp
	multicoloredRectangle.cpp
	renderer.cpp
	sceneObject.cpp)
	
	
target_sources(OpenGL_Study_Exe PRIVATE ${SOURCES} ${HEADERS})
target_include_directories(OpenGL_Study_Exe PRIVATE ${OpenGL_Study_SOURCE_DIR}/libs/include/GLFW)
target_link_libraries(OpenGL_Study_Exe PRIVATE glad
	${GLFW3}
	OpenGL_Study_compiler_flags
	OpenGL_Study_General
	OpenGL_Study_Helpers
    OpenGL_Study_Math_Core
	OpenGL_Study_OpenGL_Core)


source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}"
	PREFIX "Header Files"
	FILES ${HEADERS})
	
source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}"
	PREFIX "Source Files"
	FILES ${SOURCES})
	