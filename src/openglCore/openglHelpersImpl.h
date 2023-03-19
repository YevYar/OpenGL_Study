#ifndef OPENGL_HELPERS_IMPL_H
#define OPENGL_HELPERS_IMPL_H

#include <concepts>
#include <functional>

#include "helpers/helpers.h"
#include "helpers/openglHelpers.h"

namespace openglCore
{    
    template<typename Type>
    concept OpenGLBindableObject = requires(Type openglObject)
    {
        requires std::is_same_v<decltype(openglObject.m_rendererId), GLuint>;
        requires std::is_same_v < std::underlying_type_t<decltype(openglObject.m_target)>, GLenum> ;
        Type::getTargetAssociatedGetParameter(decltype(openglObject.m_target){});
        Type::bindToTarget(decltype(openglObject.m_target){}, GLuint{});
        openglObject.bind();        
    };

    template<OpenGLBindableObject Type>
    void bindForAMomentAndExecute(const Type& openglObject,
        const std::function<void()>& funcToExecute = []() {})
    {
        GLuint boundObj = helpers::getOpenGLIntegerValue(helpers::toUType(
            openglObject.getTargetAssociatedGetParameter(openglObject.m_target)));

        if (boundObj == openglObject.m_rendererId)
        {
            funcToExecute();
        }
        else
        {
            openglObject.bind();
            funcToExecute();
            openglObject.bindToTarget(openglObject.m_target, boundObj);
        }
    }
}

#endif
