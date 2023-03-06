#include "helpers/openglHelpers.h"

#include "helpers/debugHelpers.h"

GLint helpers::getOpenGLIntegerValue(GLenum parameterName)
{
    GLint result = 0;
    GLCall(glGetIntegerv(parameterName, &result));
    return result;
}
