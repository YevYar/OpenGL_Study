#include "helpers/debugHelpers.h"

#include <glad/glad.h>
#include <iostream>

namespace ogls::helpers
{
bool checkAndLogGLErrors(std::string_view file, std::string_view function, int line)
{
    if (Window::isGLFWTerminated())
    {
        return false;
    }

    auto isErrorRaised = false;

    while (auto errorCode = glGetError())
    {
        std::cerr << "[OpenGL error]: code 0x" << std::hex << errorCode << std::dec << " in function " << function
                  << ", in file " << file << ", at line " << line << std::endl;

        isErrorRaised = true;
    }

    return isErrorRaised;
}

void clearGlError() noexcept
{
    if (Window::isGLFWTerminated())
    {
        return;
    }

    // clang-format off
    while (glGetError());
    // clang-format on
}

}  // namespace ogls::helpers
