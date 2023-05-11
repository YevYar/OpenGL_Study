#ifndef OGLS_HELPERS_DEBUG_HELPERS_H
#define OGLS_HELPERS_DEBUG_HELPERS_H

#include <string_view>

#include <debugbreak.h>

#include "window.h"

namespace ogls::helpers
{
/**
 * \brief Checks passed statement and cause debug break if the statement is false.
 *
 * \param x - statement to check, must be convertible into bool type.
 */
#define OGLS_ASSERT(x) \
    if (!(x))          \
    {                  \
        debug_break(); \
    }

/**
 * \brief Checks if the GLFW is not terminated and checks error after call of OpenGL functions.
 *
 * OGLS_GLCall checks the status of GLFW to be sure that the OpenGL context exists. Otherwise it does nothing.
 * Checking the existence of OpenGL context is important to avoid locks,
 * which appear when OpenGL function is called after deletion of OpenGL context.
 *
 * Before executing of a passed code OpenGL errors are cleaned. After executing existence of OpenGL errors is checked.
 *
 * DON'T pass any variable initialization statements, because the passed code is called in the scope of if-statement.
 *
 * \param x - statement to execute.
 */
#define OGLS_GLCall(x)                                                            \
    if (!ogls::Window::isGLFWTerminated())                                        \
    {                                                                             \
        ogls::helpers::clearGlError();                                            \
        x;                                                                        \
        OGLS_ASSERT(!ogls::helpers::checkAndLogGLErrors(__FILE__, #x, __LINE__)); \
    }

/**
 * \brief Checks OpenGL error and prints OpenGL error in std::cerr.
 *
 * \param file     - the file name, from which the function is called.
 * \param function - the name of the function, from which the function is called.
 * \param line     - the line, from which the function is called.
 * \return false if is no error and true otherwise.
 */
bool checkAndLogGLErrors(std::string_view file, std::string_view function, int line);

/**
 * \brief Invalidates all current OpenGL errors.
 */
void clearGlError() noexcept;

}  // namespace ogls::helpers

#endif
