#include "openglLimits.h"

#include "openglHelpers.h"

namespace
{
    std::map<LimitName, GLint> limits;
}

void initOpenglLimits()
{
    using namespace helpers;
    GLint limitValue = 0;

    limitValue = getOpenGLIntegerValue(toUType(LimitName::MAX_COMBINED_TEXTURE_IMAGE_UNITS));
    limits.insert({ LimitName::MAX_COMBINED_TEXTURE_IMAGE_UNITS, limitValue });

    limitValue = getOpenGLIntegerValue(toUType(LimitName::MAX_VERTEX_ATTRIBS));
    limits.insert({ LimitName::MAX_VERTEX_ATTRIBS, limitValue });
}

const std::map<LimitName, GLint>& getOpenglLimits() noexcept
{
    return limits;
}

GLint getOpenglLimit(LimitName limitName) noexcept
{
    return limits.find(limitName)->second;
}
