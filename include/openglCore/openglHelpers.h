#ifndef OPENGL_HELPERS_H
#define OPENGL_HELPERS_H

#include <functional>

#include "helpers.h"

namespace helpers
{
    /**
     * \brief Gets the integer value of the specified parameter from the OpenGL state machine.
     *
     * The list of available parameters see in description of
     * [glGet()](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGet.xhtml).
     *
     * \return the integer value of the specified parameter.
     */
    GLint getOpenGLIntegerValue(GLenum parameterName);
    
    template<typename Type>
    class OpenGLBindableObject
    {
        public:
            static constexpr bool accepted = requires(Type openglObject)
            {
                Type::getTargetAssociatedGetParameter(decltype(openglObject.m_target){});
                Type::bindToTarget(decltype(openglObject.m_target){}, GLuint{});
                openglObject.bind();
            };

            OpenGLBindableObject(const Type& obj) : m_obj{ obj },
                getParameter{ helpers::toUType(obj.getTargetAssociatedGetParameter(obj.m_target)) },
                objectId{ obj.m_rendererId }
            {
            }

            GLenum getParameter;
            GLuint objectId;

            void bindToTarget(GLuint objId) const
            {
                Type::bindToTarget(m_obj.m_target, objId);
            }

            void bind() const
            {
                m_obj.bind();
            }

        private:
            const Type& m_obj;

    };

    template<typename Type>
    requires OpenGLBindableObject<Type>::accepted
    void bindForAMomentAndExecute(const OpenGLBindableObject<Type>& openglObject,
        const std::function<void()>& funcToExecute = []() {})
    {
        GLuint boundObj = getOpenGLIntegerValue(openglObject.getParameter);

        if (boundObj == openglObject.objectId)
        {
            funcToExecute();
        }
        else
        {
            openglObject.bind();
            funcToExecute();
            openglObject.bindToTarget(boundObj);
        }
    }
}

#endif
