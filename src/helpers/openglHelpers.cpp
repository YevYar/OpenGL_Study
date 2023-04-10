#include "helpers/openglHelpers.h"

#include <cstring>
#include <typeinfo>

#include "helpers/debugHelpers.h"

namespace ogls::helpers
{
GLint getOpenGLIntegerValue(GLenum parameterName)
{
    auto result = GLint{0};
    OGLS_GLCall(glGetIntegerv(parameterName, &result));
    return result;
}

bool isGLdouble(const char* typeName) noexcept
{
    auto doubleIndicator = GLdouble{0};
    return strcmp(typeName, typeid(doubleIndicator).name()) == 0;
}

bool isGLfloat(const char* typeName) noexcept
{
    auto floatIndicator = GLfloat{0};
    return strcmp(typeName, typeid(floatIndicator).name()) == 0;
}

bool isGLint(const char* typeName) noexcept
{
    auto intIndicator = GLint{0};
    return strcmp(typeName, typeid(intIndicator).name()) == 0;
}

bool isGLuint(const char* typeName) noexcept
{
    auto unsignedIndicator = GLuint{0};
    return strcmp(typeName, typeid(unsignedIndicator).name()) == 0;
}

}  // namespace ogls::helpers
