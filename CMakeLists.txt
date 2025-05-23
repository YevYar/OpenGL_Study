cmake_minimum_required(VERSION 3.20)

project(OpenGL_Study
	VERSION 0.1
	DESCRIPTION "Study OpenGL project."
	HOMEPAGE_URL "https://github.com/YevYarWolf/OpenGL_Study"
	LANGUAGES CXX C)


# === SET OPTIONS
option(BUILD_DOC "Build documentation" ON)

# Define an option for selecting the graphics API
option(USE_OPENGL "Use OpenGL as the graphics API" ON)
option(USE_VULKAN "Use Vulkan as the graphics API" OFF)

if(NOT USE_OPENGL AND NOT USE_VULKAN)
    message(FATAL_ERROR "Please select at least one graphics API.")
endif()

# Set the appropriate compile definitions based on user's choice
if(USE_OPENGL)
    add_definitions(-DTREAT_VECTORS_AS_COLUMNS)
elseif(USE_VULKAN)
    add_definitions(-DTREATE_VECTORS_AS_ROWS)
else()
    add_definitions(-DTREAT_VECTORS_AS_COLUMNS)
endif()


# === CREATE GENERAL INTERFACE LIBRARY TO SET NECESSARY FLAGS TO ALL TARGETS
add_library(OpenGL_Study_compiler_flags INTERFACE)
target_compile_features(OpenGL_Study_compiler_flags INTERFACE cxx_std_23)

# Adding compiler warning flags
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(OpenGL_Study_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)
set_target_properties(OpenGL_Study_compiler_flags PROPERTIES CXX_ISO_COMPLIANT ON)


# === SOME USEFUL CONFIGURATION
# Let's nicely support folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# === SET OUTPUT DIRECTORIES
# Set folder for build result files (*.exe/*.lib/*.a) 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


# === FORBID BUILD IN SOURCE DIRECTORY
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS ${LOC_PATH})
	message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()


# Find OpenGL
find_package(OpenGL REQUIRED)


# === ADD SUBDIRECTORIES
add_subdirectory(app)
add_subdirectory(libs)
add_subdirectory(src)

if(BUILD_DOC)
	add_subdirectory(docs)
endif()
