#include "helpers/debugHelpers.h"

#include <glad/glad.h>
#include <iostream>

void helpers::clearGlError()
{
    if (Window::isGLFWTerminated())
    {
        return;
    }

    while (glGetError());
}

bool helpers::checkAndLogGLErrors(const char* file, const char* function, int line)
{
    if (Window::isGLFWTerminated())
    {
        return false;
    }

    bool isErrorRaised = false;

    while (auto errorCode = glGetError())
    {
        std::cerr << "[OpenGL error]: code 0x" << std::hex << errorCode << std::dec
            << " in function " << function << ", in file " << file << ", at line " << line << std::endl;

        isErrorRaised = true;
    }

    return isErrorRaised;
}
