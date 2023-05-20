#ifndef OGLS_OGLCORE_SHADER_UNIFORMS_H
#define OGLS_OGLCORE_SHADER_UNIFORMS_H

#include <memory>
#include <string>
#include <type_traits>

#include <glad/glad.h>

#include "helpers/macros.h"

namespace ogls::oglCore::shader
{
class ShaderProgram;

/**
 * \brief BaseUniform is a base class for uniform classes.
 *
 * By design, object of BaseUniform class can be created only by ShaderProgram class.
 * Such architecture simulates how uniforms are used in OpenGL:
 * OpenGL shader program must be used, after that location of the specified uniform must be received
 * and in the result the value of the uniform can be set using found location.
 */
class BaseUniform
{
    protected:
        /**
         * \brief BaseImpl contains private data and methods of BaseUniform.
         */
        struct BaseImpl;

    public:
        BaseUniform() = delete;
        OGLS_NOT_COPYABLE_MOVABLE(BaseUniform)
        virtual ~BaseUniform() noexcept;

        /**
         * \brief Sets the data of OpenGL uniform variable.
         *
         * \param data - pointer to a data, which must be set in the uniform.
         */
        virtual void setData(const void* data) = 0;

    protected:
        /**
         * \brief Constructs new BaseUniform object using pointer to BaseImpl object or to object of the class,
         * derived from BaseImpl.
         *
         * \param impl - pointer to BaseImpl object or to object of the class, derived from BaseImpl.
         */
        explicit BaseUniform(std::unique_ptr<BaseImpl> impl) noexcept;

    protected:
        /**
         * \brief Pointer to implementation.
         */
        std::unique_ptr<BaseImpl> m_impl;


        friend class ShaderProgram;

};  // class BaseUniform

/**
 * \brief Uniform represents one dimensional uniform variable, which contains [1, 4] elements.
 *
 * \param Type  - one of the list: GLfloat, GLdouble, GLint, GLuint.
 * \param Count - the integer value in the range [1, 4].
 */
template<typename Type, unsigned int Count>
class Uniform : public BaseUniform
{
        static_assert(std::is_same_v<GLfloat, Type> || std::is_same_v<GLdouble, Type> || std::is_same_v<GLint, Type>
                        || std::is_same_v<GLuint, Type>,
                      "A Uniform can be of the following types: GLfloat, GLdouble, GLint or GLuint.");
        static_assert(Count >= 1 && Count <= 4, "Count must be in range [1, 4].");


    private:
        /**
         * \brief Impl contains private data and methods of Uniform.
         */
        struct Impl;

    public:
        Uniform() = delete;

        /**
         * \brief Returns a Type representation of Uniform object.
         *
         * \see getValue().
         */
        explicit operator Type() const;

        /**
         * \brief Returns current value, which is stored in OpenGL uniform variable inside OpenGL state machine.
         */
        Type getValue() const;

        void setData(const void* data) override;

    protected:
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
        Uniform(GLuint shaderProgram, GLint location, std::string name);

    private:
        /**
         * \brief Returns a pointer to an object of implementation class, casted from BaseUniform::BaseImpl to Impl.
         */
        Impl* impl() const noexcept;


        friend class ShaderProgram;

};  // class Uniform

}  // namespace ogls::oglCore::shader

#endif
