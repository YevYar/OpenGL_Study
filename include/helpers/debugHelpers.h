#ifndef DEBUG_HELPERS_H
#define DEBUG_HELPERS_H

#include <debugbreak.h>

#include "window.h"

/**
 * \brief Checks passed statement and cause debug break if the statement is false.
 * 
 * \param x - statement to check, must be convertible into bool type.
 */
#define ASSERT(x) if (!(x)) debug_break();

/**
 * \brief Checks if the GLFW is not terminated and checks error after call of OpenGL functions.
 * 
 * GLCall checks the status of GLFW to be sure that the OpenGL context exists. Otherwise it does nothing.
 * Checking the existence of OpenGL context is important to avoid locks,
 * which appear when OpenGL function is called after deletion of OpenGL context.
 * 
 * Before executing of a passed code OpenGL errors are cleaned. After executing existence of OpenGL errors is checked.
 * 
 * DON'T pass any variable initialization statements, because the passed code is called in the scope of if-statement.
 * 
 * \param x - statement to execute.
 */
#define GLCall(x) \
	if (!Window::isGLFWTerminated());\
	{\
		clearGlError();\
		x;\
		ASSERT(!checkAndLogGLErrors(__FILE__, #x, __LINE__));\
	}\

/**
 * \brief Invalidates all current OpenGL errors.
 */
void clearGlError();

/**
 * \brief Checks OpenGL error and prints OpenGL error in std::cerr.
 * 
 * \param file - the file name, from which the function is called.
 * \param function - the name of the function, from which the function is called.
 * \param line - the line, from which the function is called.
 * \return false if is no error and true otherwise.
 */
bool checkAndLogGLErrors(const char* file, const char* function, int line);

#endif
