#ifndef OPENGL_LIMITS_H
#define OPENGL_LIMITS_H

#include <glad/glad.h>
#include <map>

namespace openglCore
{
    enum class LimitName : GLenum
    {
        MAX_VERTEX_ATTRIBS = 0x8869, MAX_COMBINED_TEXTURE_IMAGE_UNITS = 0x8B4D
    };

    void initOpenglLimits();

    const std::map<LimitName, GLint>& getOpenglLimits() noexcept;

    GLint getOpenglLimit(LimitName limitName) noexcept;
}

#endif
