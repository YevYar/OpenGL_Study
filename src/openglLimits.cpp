#include "openglLimits.h"

#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"

namespace
{
    std::map<LimitName, GLint> limits;
}

void initOpenglLimits()
{
    using namespace helpers;
    GLint limitValue = 0;

    GLCall(glGetIntegerv(toUType(LimitName::MAX_COMBINED_TEXTURE_IMAGE_UNITS), &limitValue));
    limits.insert({ LimitName::MAX_COMBINED_TEXTURE_IMAGE_UNITS, limitValue });

    GLCall(glGetIntegerv(toUType(LimitName::MAX_VERTEX_ATTRIBS), &limitValue));
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
