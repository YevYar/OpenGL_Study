# Find GLFW lib
find_library(GLFW3 glfw3 ${CMAKE_CURRENT_SOURCE_DIR}/)

# Build GLAD
add_library(glad include/glad/glad.h glad.c)
target_include_directories(glad PUBLIC include)
