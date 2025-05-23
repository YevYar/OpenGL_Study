#ifndef OGLS_OGLCORE_OPENGL_HELPERS_IMPL_H
#define OGLS_OGLCORE_OPENGL_HELPERS_IMPL_H

#include <concepts>
#include <functional>

#include "helpers/helpers.h"
#include "helpers/openglHelpers.h"

namespace ogls::oglCore
{
/**
 * \brief OpenGLBindableObject is a concept, which specifies the OpenGL object, which can be bound to some target
 * in the context of this project.
 *
 * \param Type - a type to check constraints.
 */
template<typename Type>
concept OpenGLBindableObject = requires(Type openglObject) {
    requires std::is_same_v<decltype(openglObject.rendererId), GLuint>;
    requires std::is_same_v<std::underlying_type_t<decltype(openglObject.target)>, GLenum>;
    Type::getTargetAssociatedGetParameter(decltype(openglObject.target){});
    Type::bindToTarget(decltype(openglObject.target){}, GLuint{});
    openglObject.bind();
};

/**
 * \brief Binds a given object to its target and executes passed function.
 *
 * If openglObject has been already bound, no re-bounding occurs.
 *
 * \param openglObject  - an object to be bound.
 * \param funcToExecute - a function, which is executed after bounding of openglObject.
 */
template<OpenGLBindableObject Type>
void bindForAMomentAndExecute(
  const Type& openglObject, const std::function<void()>& funcToExecute = []() {})
{
    auto boundObj = helpers::getOpenGLIntegerValue(
      helpers::toUType(openglObject.getTargetAssociatedGetParameter(openglObject.target)));

    if (boundObj == openglObject.rendererId)
    {
        funcToExecute();
    }
    else
    {
        openglObject.bind();
        funcToExecute();
        openglObject.bindToTarget(openglObject.target, boundObj);
    }
}

}  // namespace ogls::oglCore

#endif
