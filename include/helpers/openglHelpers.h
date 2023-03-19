#ifndef OPENGL_HELPERS_H
#define OPENGL_HELPERS_H

#include <glad/glad.h>

namespace helpers
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

    bool isGLfloat(const char* typeName) noexcept;
    bool isGLdouble(const char* typeName) noexcept;
    bool isGLint(const char* typeName) noexcept;
    bool isGLunsignedInt(const char* typeName) noexcept;

}

#endif
