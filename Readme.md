# OpenGL_Study

It is a study project to know basics of OpenGL.

## Installation

- Using CMake generate project for desired build system.
- If you use Microsoft Visual Studio, use File -> Open -> Folder and select a root folder of the project.

## Building documentation
- If you generated project using CMake, build *OpenGL_Study_docs* target.
- Using the following command ```cmake --build . --target OpenGL_Study_docs``` (if MVS is used to work with CMake project without generating *.sln file, this command must be run from folder out\build\x64-Debug).
- ```doxygen Doxyfile.OpenGL_Study_docs``` (run this command from location of Doxygen config file, which is defined by *DOXYGEN_OUTPUT_DIRECTORY*).