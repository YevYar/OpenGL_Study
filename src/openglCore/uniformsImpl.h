#ifndef OGLS_OGLCORE_SHADER_UNIFORMS_IMPL_H
#define OGLS_OGLCORE_SHADER_UNIFORMS_IMPL_H

#include "uniforms.h"

namespace ogls::oglCore::shader
{
struct BaseUniform::BaseImpl
{
    public:
        /**
         * \brief Constructs new object.
         *
         * It is also checked that the uniform is attached to a shader program.
         *
         * \param shaderProgram - an ID of parent shader program.
         * \param location      - a location of the uniform in a shader program.
         * \param name          - a name of the uniform variable.
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        BaseImpl(GLuint shaderProgram, GLint location, std::string name);
        OGLS_NOT_COPYABLE_MOVABLE(BaseImpl)

        /**
         * \brief Sets the data of OpenGL uniform variable.
         *
         * \param data - pointer to a data, which must be set in the uniform.
         */
        virtual void setData(const void* data) = 0;

    public:
        /**
         * \brief Location (ID) of the referenced OpenGL uniform variable in a shader program.
         */
        const GLint       location = {-1};
        /**
         * \brief Name of the referenced OpenGL uniform variable, which is used in a code of a shader.
         */
        const std::string name;
        /*
         * \brief ID of parent shader program.
         */
        const GLuint      shaderProgram = {0};

};  // struct BaseUniform::BaseImpl

template<typename Type, unsigned int Count>
struct Uniform<Type, Count>::Impl : public BaseUniform::BaseImpl
{
    public:
        /**
         * \brief ConcreteUniformGetter is a signature of OpenGL function, which is used by getValue() to get the data
         * of this uniform variable.
         */
        using ConcreteUniformGetter = void (*)(GLuint, GLint, Type*);
        /**
         * \brief ConcreteUniformSetter is a signature of OpenGL function, which is used by setData() to set the data
         * of this uniform variable.
         */
        using ConcreteUniformSetter = void (*)(GLint, GLsizei, const Type*);

    public:
        /**
         * \brief Constructs new object.
         *
         * \param shaderProgram - an ID of parent shader program.
         * \param location      - a location of the uniform in a shader program.
         * \param name          - a name of the uniform variable.
         * \see BaseImpl().
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        Impl(GLuint shaderProgram, GLint location, std::string name);
        OGLS_NOT_COPYABLE_MOVABLE(Impl)

        /**
         * \brief Returns current value, which is stored in OpenGL uniform variable.
         */
        Type getValue();

        void setData(const void* data) override;

    public:
        /**
         * \brief The number of elements in the uniform variable.
         */
        const unsigned int          count  = {Count};
        /**
         * \brief The pointer to OpenGL function to get value of this uniform in OpenGL state machine.
         */
        const ConcreteUniformGetter getter = nullptr;
        /**
         * \brief The pointer to OpenGL function to set value of this uniform in OpenGL state machine.
         */
        const ConcreteUniformSetter setter = nullptr;

};  // struct Uniform::Impl

}  // namespace ogls::oglCore::shader

#endif
