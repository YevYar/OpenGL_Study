#include "openglLimits.h"

#include "helpers/helpers.h"
#include "helpers/openglHelpers.h"

using namespace openglCore;

namespace
{
    bool isInitialised = false;
    std::map<LimitName, GLint> limits;
}

void openglCore::initOpenglLimits()
{
    if (isInitialised)
    {
        return;
    }

    using namespace helpers;
    GLint limitValue = 0;

    limitValue = getOpenGLIntegerValue(toUType(LimitName::MAX_COMBINED_TEXTURE_IMAGE_UNITS));
    limits.insert({ LimitName::MAX_COMBINED_TEXTURE_IMAGE_UNITS, limitValue });

    limitValue = getOpenGLIntegerValue(toUType(LimitName::MAX_VERTEX_ATTRIBS));
    limits.insert({ LimitName::MAX_VERTEX_ATTRIBS, limitValue });

    isInitialised = true;
}

const std::map<LimitName, GLint>& openglCore::getOpenglLimits() noexcept
{
    return limits;
}

GLint openglCore::getOpenglLimit(LimitName limitName) noexcept
{
    return limits.find(limitName)->second;
}
