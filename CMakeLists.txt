cmake_minimum_required(VERSION 3.20)

project(OpenGL_Study
	VERSION 0.1
	DESCRIPTION "Study OpenGL project."
	HOMEPAGE_URL "https://github.com/YevYarWolf/OpenGL_Study"
	LANGUAGES CXX C)

add_executable(OpenGL_Study_Exe)

set(HEADERS include/windowHandlers.h)

target_sources(OpenGL_Study_Exe PRIVATE ${HEADERS})
target_include_directories(OpenGL_Study_Exe PRIVATE include/ libs/include)

# Find OpenGL
find_package(OpenGL REQUIRED)

add_subdirectory(src)
add_subdirectory(libs)

source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}/include"
	PREFIX "Header Files"
	FILES ${HEADERS})

target_link_libraries(OpenGL_Study_Exe PRIVATE OpenGL::GL ${GLFW3} glad)
