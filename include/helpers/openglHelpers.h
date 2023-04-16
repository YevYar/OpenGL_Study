#ifndef OGLS_HELPERS_OPENGL_HELPERS_H
#define OGLS_HELPERS_OPENGL_HELPERS_H

#include <glad/glad.h>

namespace ogls::helpers
{
/**
 * \brief Gets the integer value of the specified parameter from the OpenGL state machine.
 *
 * The list of available parameters see in description of
 * [glGet()](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGet.xhtml).
 *
 * \return the integer value of the specified parameter.
 */
GLint getOpenGLIntegerValue(GLenum parameterName);

/**
 * \brief Checks if the provided typeName corresponds to a type of GLdouble.
 *
 * \param typeName - a result of calling typeid(some_type).name().
 * \return true if typeName corresponds to GLdouble, false otherwise.
 */
bool isGLdouble(const char* typeName) noexcept;

/**
 * \brief Checks if the provided typeName corresponds to a type of GLfloat.
 *
 * \param typeName - a result of calling typeid(some_type).name().
 * \return true if typeName corresponds to GLfloat, false otherwise.
 */
bool isGLfloat(const char* typeName) noexcept;

/**
 * \brief Checks if the provided typeName corresponds to a type of GLint.
 *
 * \param typeName - a result of calling typeid(some_type).name().
 * \return true if typeName corresponds to GLint, false otherwise.
 */
bool isGLint(const char* typeName) noexcept;

/**
 * \brief Checks if the provided typeName corresponds to a type of GLuint.
 *
 * \param typeName - a result of calling typeid(some_type).name().
 * \return true if typeName corresponds to GLuint, false otherwise.
 */
bool isGLuint(const char* typeName) noexcept;

}  // namespace ogls::helpers

#endif
