#ifndef UNIFORMS_IMPL_H
#define UNIFORMS_IMPL_H

#include "uniforms.h"

namespace openglCore::shader
{
    struct BaseUniform::BaseImpl
    {
        // METHODS
        /**
            * \brief Constructs new object.
            *
            * \param location - a location of the uniform in a shader program.
            * \param name - a name of the uniform variable.
            * \throw exceptions::GLRecAcquisitionException(), if location is 0
            */
        BaseImpl(GLuint shaderProgram, GLint location, std::string name);

        NOT_COPYABLE_MOVABLE(BaseImpl)

        // DATA
        /**
            * \brief Location (id) of the referenced OpenGL uniform variable in a shader program.
            */
        const GLint m_location = -1;

        /**
            * \brief Name of the referenced OpenGL uniform variable, which is used in a code of a shader.
            */
        const std::string m_name;

        const GLuint m_shaderProgram = 0;

    };

    template<typename Type, unsigned int Count>
    struct Uniform<Type, Count>::Impl : public BaseUniform::BaseImpl
    {
        // TYPES
        using ConcreteUniformSetter = void (*)(GLint, GLsizei, const Type*);
        using ConcreteUniformGetter = void (*)(GLuint, GLint, Type*);

        // METHODS
        Impl(GLuint shaderProgram, GLint location, std::string name);

        NOT_COPYABLE_MOVABLE(Impl)

        void setData(const void* data);
        Type getValue();

        // DATA
        /**
            * \brief The number of elements in the uniform variable.
            */
        const unsigned int m_count = Count;

        /**
            * \brief The pointer to OpenGL function to set value of this uniform in OpenGL state machine.
            */
        const ConcreteUniformSetter m_setter = nullptr;
        const ConcreteUniformGetter m_getter = nullptr;

    };
}

#endif
