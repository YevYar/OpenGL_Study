#include "uniforms.h"
#include "uniformsImpl.h"

#include <array>

#include "exceptions.h"
#include "helpers/openglHelpers.h"

namespace ogls::oglCore::shader
{
namespace
{
    /**
     * \brief BaseUniformGetter is a pointer to an OpenGL function to get value of uniform variable.
     */
    using BaseUniformGetter = void (*)(GLuint, GLint, void*);

    /**
     * \brief BaseUniformSetter is a pointer to an OpenGL function to set value of uniform variable.
     */
    using BaseUniformSetter = void (*)(GLint, GLsizei, const void*);

    /**
     * \brief MatrixUniformSetter is a pointer to an OpenGL function to set value of matrix uniform variable.
     */
    using MatrixUniformSetter = void (*)(GLint, GLsizei, GLboolean, const GLfloat*);


    /**
     * \brief Returns a pointer to an OpenGL setter-function to set matrix uniform variable.
     *
     * \param N - a number of rows in the Matrix in range [2, 4].
     * \param M - a number of columns in the Matrix in range [2, 4].
     */
    MatrixUniformSetter getMatrixUniformSetter(size_t N, size_t M) noexcept;
    /**
     * \brief Returns a pointer to an OpenGL getter-function to get a data of the uniform variable.
     *
     * \param typeName - a result of call typeid(Type).name() on variable of Type GLfloat, GLdouble, GLint or GLuint.
     * \return pointer to the OpenGL getter-function or nullptr.
     */
    BaseUniformGetter   getUniformGetter(const char* typeName) noexcept;
    /**
     * \brief Returns a pointer to an OpenGL setter-function to set a data of the uniform variable.
     *
     * \param typeName - a result of call typeid(Type).name() on variable of Type GLfloat, GLdouble, GLint or GLuint.
     * \param count    - the value in the range [1, 4].
     * \return pointer to the OpenGL setter-function or nullptr.
     */
    BaseUniformSetter   getUniformSetter(const char* typeName, unsigned int count) noexcept;

}  // namespace

BaseUniform::BaseUniform(std::unique_ptr<BaseImpl> impl) noexcept : m_impl{std::move(impl)}
{
}

BaseUniform::~BaseUniform() noexcept = default;

template<size_t N, size_t M>
MatrixUniform<N, M>::MatrixUniform(GLuint shaderProgram, GLint location, std::string name) :
    BaseUniform{std::make_unique<Impl>(shaderProgram, location, std::move(name))}
{
}

template<size_t N, size_t M>
MatrixUniform<N, M>::operator DataType() const
{
    return getData();
}

template<size_t N, size_t M>
auto MatrixUniform<N, M>::getData() const -> DataType
{
    return impl()->getData();
}

template<size_t N, size_t M>
void MatrixUniform<N, M>::setData(const DataType& data)
{
    impl()->setData(data);
}

template<size_t N, size_t M>
auto MatrixUniform<N, M>::impl() const noexcept -> Impl*
{
    return static_cast<MatrixUniform<N, M>::Impl*>(m_impl.get());
}

template<typename Type, size_t Count>
Uniform<Type, Count>::Uniform(GLuint shaderProgram, GLint location, std::string name) :
    BaseUniform{std::make_unique<Impl>(shaderProgram, location, std::move(name))}
{
}

template<typename Type, size_t Count>
Uniform<Type, Count>::operator DataType() const
{
    return getData();
}

template<typename Type, size_t Count>
auto Uniform<Type, Count>::getData() const -> DataType
{
    return impl()->getData();
}

template<typename Type, size_t Count>
void Uniform<Type, Count>::setData(const DataType& data)
{
    impl()->setData(data);
}

template<typename Type, size_t Count>
typename auto Uniform<Type, Count>::impl() const noexcept -> Impl*
{
    return static_cast<Uniform<Type, Count>::Impl*>(m_impl.get());
}

//------ IMPLEMENTATION

BaseUniform::BaseImpl::BaseImpl(GLuint sProgram, GLint loc, std::string n) :
    location{loc}, name{std::move(n)}, shaderProgram{sProgram}
{
    if (shaderProgram == 0 || location < 0)
    {
        throw exceptions::GLRecAcquisitionException{"Uniform is not attached to a shader program."};
    }
}

template<size_t N, size_t M>
MatrixUniform<N, M>::Impl::Impl(GLuint shaderProgram, GLint location, std::string name) :
    BaseImpl{shaderProgram, location, std::move(name)}, setter{getMatrixUniformSetter(N, M)}
{
    if (setter == nullptr)
    {
        throw exceptions::GLRecAcquisitionException{"No uniform setter function for specified template arguments."};
    }
}

template<size_t N, size_t M>
auto MatrixUniform<N, M>::Impl::getData() const -> DataType
{
    GLfloat data[N * M];
    OGLS_GLCall(glGetUniformfv(shaderProgram, location, data));

    // From https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetUniform.xhtml:
    // The values for uniform variables declared as a matrix will be returned in column major order.
    auto result = DataType{};

    auto i = size_t{0};
    for (auto it = result.beginColumn(); it < result.endColumn(); ++it, ++i)
    {
        (*it).setValue(data[i]);
    }

    return result;
}

template<size_t N, size_t M>
void MatrixUniform<N, M>::Impl::setData(const DataType& data)
{
    // From https://docs.gl/gl4/glUniform:
    // A count of 1 should be used if modifying the value of a single uniform variable,
    // and a count of 1 or greater can be used to modify an entire array or part of an array.
    // This call modifies the uniform variable of type vec, so 1 is passed.
    //
    // If transpose is GL_FALSE, each matrix is assumed to be supplied in column major order.
    // If transpose is GL_TRUE, each matrix is assumed to be supplied in row major order.
    OGLS_GLCall(setter(location, 1, GL_FALSE, data.getPointerToData()));
}

template<typename Type, size_t Count>
Uniform<Type, Count>::Impl::Impl(GLuint shaderProgram, GLint location, std::string name) :
    BaseImpl{shaderProgram, location, std::move(name)},
    getter{reinterpret_cast<ConcreteUniformGetter>(getUniformGetter(typeid(Type).name()))},
    setter{reinterpret_cast<ConcreteUniformSetter>(getUniformSetter(typeid(Type).name(), Count))}
{
    if (getter == nullptr)
    {
        throw exceptions::GLRecAcquisitionException{"No uniform getter function for specified template arguments."};
    }
    if (setter == nullptr)
    {
        throw exceptions::GLRecAcquisitionException{"No uniform setter function for specified template arguments."};
    }
}

template<typename Type, size_t Count>
auto Uniform<Type, Count>::Impl::getData() const
{
    if constexpr (Count == 1)
    {
        auto result = Type{0};
        OGLS_GLCall(getter(shaderProgram, location, &result));
        return result;
    }
    else
    {
        Type result[Count];
        OGLS_GLCall(getter(shaderProgram, location, result));
        return helpers::makeStdArray(result);
    }
}

template<typename Type, size_t Count>
void Uniform<Type, Count>::Impl::setData(const DataType& data)
{
    if constexpr (Count == 1)
    {
        OGLS_GLCall(setter(location, 1, &data));
    }
    else
    {
        // From https://docs.gl/gl4/glUniform:
        // A count of 1 should be used if modifying the value of a single uniform variable,
        // and a count of 1 or greater can be used to modify an entire array or part of an array.
        //
        // This call modifies the uniform variable of type vec, so 1 is passed.
        OGLS_GLCall(setter(location, 1, data.data()));
    }
}

namespace
{
    MatrixUniformSetter getMatrixUniformSetter(size_t N, size_t M) noexcept
    {
        // From https://docs.gl/gl4/glUniform:
        // The commands glUniformMatrix{2|3|4|2x3|3x2|2x4|4x2|3x4|4x3}fv are used to modify a matrix or an array
        // of matrices. The numbers in the command name are interpreted as the dimensionality of the matrix.
        // The number 2 indicates a 2 x 2 matrix (i.e., 4 values), the number 3 indicates a 3 x 3 matrix
        // (i.e., 9 values), and the number 4 indicates a 4 x 4 matrix (i.e., 16 values).
        // Non-square matrix dimensionality is explicit, with the first number representing the number of columns
        // and the second number representing the number of rows.
        // For example, 2x4 indicates a 2 x 4 matrix with 2 columns and 4 rows (i.e., 8 values).

        if (N == 2 && M == 2)
        {
            return glUniformMatrix2fv;
        }
        if (N == 3 && M == 3)
        {
            return glUniformMatrix3fv;
        }
        if (N == 4 && M == 4)
        {
            return glUniformMatrix4fv;
        }
        if (N == 3 && M == 2)
        {
            return glUniformMatrix2x3fv;
        }
        if (N == 2 && M == 3)
        {
            return glUniformMatrix3x2fv;
        }
        if (N == 4 && M == 2)
        {
            return glUniformMatrix2x4fv;
        }
        if (N == 2 && M == 4)
        {
            return glUniformMatrix4x2fv;
        }
        if (N == 4 && M == 3)
        {
            return glUniformMatrix3x4fv;
        }
        if (N == 3 && M == 4)
        {
            return glUniformMatrix4x3fv;
        }

        OGLS_ASSERT(false);
        return nullptr;
    }

    BaseUniformGetter getUniformGetter(const char* typeName) noexcept
    {
        using namespace helpers;


        if (isGLfloat(typeName))
        {
            return reinterpret_cast<BaseUniformGetter>(glGetUniformfv);
        }
        if (isGLdouble(typeName))
        {
            return reinterpret_cast<BaseUniformGetter>(glGetUniformdv);
        }
        if (isGLint(typeName))
        {
            return reinterpret_cast<BaseUniformGetter>(glGetUniformiv);
        }
        if (isGLuint(typeName))
        {
            return reinterpret_cast<BaseUniformGetter>(glGetUniformuiv);
        }

        OGLS_ASSERT(false);
        return nullptr;
    }

    BaseUniformSetter getUniformSetter(const char* typeName, unsigned int count) noexcept
    {
        using namespace helpers;


        if (isGLfloat(typeName))
        {
            switch (count)
            {
                case 1:
                    return reinterpret_cast<BaseUniformSetter>(glUniform1fv);
                case 2:
                    return reinterpret_cast<BaseUniformSetter>(glUniform2fv);
                case 3:
                    return reinterpret_cast<BaseUniformSetter>(glUniform3fv);
                case 4:
                    return reinterpret_cast<BaseUniformSetter>(glUniform4fv);
                default:
                {
                    OGLS_ASSERT(false);
                    return nullptr;
                }
            }
        }
        if (isGLdouble(typeName))
        {
            switch (count)
            {
                case 1:
                    return reinterpret_cast<BaseUniformSetter>(glUniform1dv);
                case 2:
                    return reinterpret_cast<BaseUniformSetter>(glUniform2dv);
                case 3:
                    return reinterpret_cast<BaseUniformSetter>(glUniform3dv);
                case 4:
                    return reinterpret_cast<BaseUniformSetter>(glUniform4dv);
                default:
                {
                    OGLS_ASSERT(false);
                    return nullptr;
                }
            }
        }
        if (isGLint(typeName))
        {
            switch (count)
            {
                case 1:
                    return reinterpret_cast<BaseUniformSetter>(glUniform1iv);
                case 2:
                    return reinterpret_cast<BaseUniformSetter>(glUniform2iv);
                case 3:
                    return reinterpret_cast<BaseUniformSetter>(glUniform3iv);
                case 4:
                    return reinterpret_cast<BaseUniformSetter>(glUniform4iv);
                default:
                {
                    OGLS_ASSERT(false);
                    return nullptr;
                }
            }
        }
        if (isGLuint(typeName))
        {
            switch (count)
            {
                case 1:
                    return reinterpret_cast<BaseUniformSetter>(glUniform1uiv);
                case 2:
                    return reinterpret_cast<BaseUniformSetter>(glUniform2uiv);
                case 3:
                    return reinterpret_cast<BaseUniformSetter>(glUniform3uiv);
                case 4:
                    return reinterpret_cast<BaseUniformSetter>(glUniform4uiv);
                default:
                {
                    OGLS_ASSERT(false);
                    return nullptr;
                }
            }
        }

        OGLS_ASSERT(false);
        return nullptr;
    }

}  // namespace

#define INSTANTIATE_UNIFORM(Type)       \
    template class MatrixUniform<2, 2>; \
    template class MatrixUniform<2, 3>; \
    template class MatrixUniform<2, 4>; \
    template class MatrixUniform<3, 2>; \
    template class MatrixUniform<3, 3>; \
    template class MatrixUniform<3, 4>; \
    template class MatrixUniform<4, 2>; \
    template class MatrixUniform<4, 3>; \
    template class MatrixUniform<4, 4>; \
    template class Uniform<Type, 1>;    \
    template class Uniform<Type, 2>;    \
    template class Uniform<Type, 3>;    \
    template class Uniform<Type, 4>;

INSTANTIATE_UNIFORM(GLdouble);
INSTANTIATE_UNIFORM(GLfloat);
INSTANTIATE_UNIFORM(GLint);
INSTANTIATE_UNIFORM(GLuint);

#undef INSTANTIATE_UNIFORM

}  // namespace ogls::oglCore::shader
