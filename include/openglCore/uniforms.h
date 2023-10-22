#ifndef OGLS_OGLCORE_SHADER_UNIFORMS_H
#define OGLS_OGLCORE_SHADER_UNIFORMS_H

#include <memory>
#include <string>
#include <type_traits>

#include <glad/glad.h>

#include "helpers/macros.h"
#include "mathCore/matrix.h"

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
        class BaseImpl;

    public:
        BaseUniform() = delete;
        OGLS_NOT_COPYABLE_MOVABLE(BaseUniform)
        virtual ~BaseUniform() noexcept;

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
 * \brief MatrixUniform represents a matrix uniform variable, which contains N*M elements.
 *
 * \param N - a number of rows in the Matrix in range [2, 4].
 * \param M - a number of columns in the Matrix in range [2, 4].
 */
template<size_t N, size_t M>
class MatrixUniform : public BaseUniform
{
        static_assert((N > 1 && N <= 4) && (M > 1 && M <= 4), "N and M must be in range [2, 4].");


    private:
        /**
         * \brief Impl contains private data and methods of MatrixUniform.
         */
        class Impl;

    public:
        /**
         * \brief DataType is a type to represent the data of the uniform variable inside the OpenGL state machine.
         *
         * mathCore::Matrix elements must be ordered in a column-row order.
         */
        using DataType = mathCore::Matrix<N, M>;

    public:
        MatrixUniform() = delete;

        /**
         * \brief Returns a MatrixUniform::DataType representation of the MatrixUniform object.
         *
         * \see getData().
         */
        explicit operator DataType() const;

        /**
         * \brief Returns current data, which is stored in OpenGL uniform variable inside the OpenGL state machine.
         *
         * mathCore::Matrix elements are ordered in a column-row order.
         */
        DataType getData() const;
        /**
         * \brief Updates the data, which is stored in OpenGL uniform variable inside the OpenGL state machine.
         *
         * \param data - the data, which must be set in the OpenGL uniform variable.
         * mathCore::Matrix elements must be ordered in a column-row order.
         */
        void     setData(DataType data);

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
        MatrixUniform(GLuint shaderProgram, GLint location, std::string name);

    private:
        /**
         * \brief Returns a pointer to an object of implementation class, casted from BaseUniform::BaseImpl to Impl.
         */
        Impl* impl() const noexcept;


        friend class ShaderProgram;

};  // class MatrixUniform

/**
 * \brief Uniform represents one dimensional uniform variable, which contains [1, 4] elements.
 *
 * \param Type  - one of the list: GLfloat, GLdouble, GLint, GLuint.
 * \param Count - the integer value in the range [1, 4].
 */
template<typename Type, size_t Count>
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
        class Impl;

    public:
        /**
         * \brief DataType is a type to represent the data of the uniform variable inside the OpenGL state machine.
         */
        using DataType = std::conditional_t<Count == 1, Type, std::array<Type, Count>>;

    public:
        Uniform() = delete;

        /**
         * \brief Returns a Uniform::DataType representation of the Uniform object.
         *
         * \see getData().
         */
        explicit operator DataType() const;

        /**
         * \brief Returns current data, which is stored in OpenGL uniform variable inside the OpenGL state machine.
         */
        DataType getData() const;
        /**
         * \brief Updates the data, which is stored in OpenGL uniform variable inside the OpenGL state machine.
         *
         * \param data - the data, which must be set in the OpenGL uniform variable.
         */
        void     setData(DataType data);

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
