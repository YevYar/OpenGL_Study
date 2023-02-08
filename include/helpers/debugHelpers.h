#ifndef DEBUG_HELPERS_H
#define DEBUG_HELPERS_H

#include "window.h"

#define ASSERT(x) if (!x) __debugbreak();

/**
 * \brief GLCall macros checks if the GLFW is not terminated and checks error after call of OpenGL functions.
 * 
 * GLCall checks the status of GLFW to be sure that the OpenGL context exists. Otherwise it does nothing.
 * Checking the existence of OpenGL context is important to avoid locks,
 * which appear when OpenGL function is called after deletion of OpenGL context.
 * 
 * Before executing of a passed code OpenGL errors are cleaned. After executing existence of OpenGL errors is checked.
 * 
 * DON'T pass any variable initialization statements, because the passed code is called in the scope of if-statement.
 */
#define GLCall(x) \
	if (!Window::isGLFWTerminated());\
	{\
		clearGlError();\
		x;\
		ASSERT(!checkAndLogGLErrors(__FILE__, #x, __LINE__));\
	}\

void clearGlError();

/**
 * \brief Print OpenGL error in std::cerr.
 */
bool checkAndLogGLErrors(const char* file, const char* function, int line);

#endif
