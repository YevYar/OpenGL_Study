#include "openglLimits.h"

#include <format>
#include <stdexcept>

#include "helpers/helpers.h"
#include "helpers/openglHelpers.h"

namespace ogls::oglCore
{
namespace
{
    auto isInitialised = false;
    auto limits        = std::map<LimitName, GLint>{};

}  // namespace

GLint getOpenglLimit(LimitName limitName)
{
    if (!limits.contains(limitName))
    {
        const auto errMes =
          std::format("The limit {} is not presented. Check, if initOpenglLimits() was called before.",
                      static_cast<GLenum>(limitName));
        throw std::logic_error{errMes};
    }

    return limits.find(limitName)->second;
}

const std::map<LimitName, GLint>& getOpenglLimits() noexcept
{
    return limits;
}

void initOpenglLimits()
{
    using namespace helpers;


    if (isInitialised)
    {
        return;
    }

    auto limitValue = GLint{0};

    limitValue = getOpenGLIntegerValue(toUType(LimitName::MaxCombinedTextureImageUnits));
    limits.insert({LimitName::MaxCombinedTextureImageUnits, limitValue});

    limitValue = getOpenGLIntegerValue(toUType(LimitName::MaxVertexAttribs));
    limits.insert({LimitName::MaxVertexAttribs, limitValue});

    isInitialised = true;
}

}  // namespace ogls::oglCore
