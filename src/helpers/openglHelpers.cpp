#include "helpers/openglHelpers.h"

#include <cstring>
#include <typeinfo>

#include "helpers/debugHelpers.h"

GLint helpers::getOpenGLIntegerValue(GLenum parameterName)
{
    GLint result = 0;
    GLCall(glGetIntegerv(parameterName, &result));
    return result;
}

bool helpers::isGLfloat(const char* typeName) noexcept
{
    GLfloat floatIndicator = 0;
    return strcmp(typeName, typeid(floatIndicator).name()) == 0;
}

bool helpers::isGLdouble(const char* typeName) noexcept
{
    GLdouble doubleIndicator = 0;
    return strcmp(typeName, typeid(doubleIndicator).name()) == 0;
}

bool helpers::isGLint(const char* typeName) noexcept
{
    GLint intIndicator = 0;
    return strcmp(typeName, typeid(intIndicator).name()) == 0;
}

bool helpers::isGLunsignedInt(const char* typeName) noexcept
{
    GLuint unsignedIndicator = 0;
    return strcmp(typeName, typeid(unsignedIndicator).name()) == 0;
}
