#include "uniforms.h"
#include "uniformsImpl.h"

#include "exceptions.h"
#include "helpers/debugHelpers.h"
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
     * \brief Returns a pointer to an OpenGL getter-function, which satisfies to passed type name.
     *
     * \param typeName - a result of call typeid(Type).name() on variable of Type GLfloat, GLdouble, GLint or GLuint.
     * \return pointer to the OpenGL getter-function or nullptr.
     */
    BaseUniformGetter getUniformGetter(const char* typeName) noexcept;
    /**
     * \brief Returns a pointer to an OpenGL setter-function, which satisfies to passed type name and count.
     *
     * \param typeName - a result of call typeid(Type).name() on variable of Type GLfloat, GLdouble, GLint or GLuint.
     * \param count    - the value in the range [1, 4].
     * \return pointer to the OpenGL setter-function or nullptr.
     */
    BaseUniformSetter getUniformSetter(const char* typeName, unsigned int count) noexcept;

}  // namespace

BaseUniform::BaseUniform(std::unique_ptr<BaseImpl> impl) : m_impl{std::move(impl)}
{
}

BaseUniform::~BaseUniform() = default;

template<typename Type, unsigned int Count>
Uniform<Type, Count>::Uniform(GLuint shaderProgram, GLint location, std::string name) :
    BaseUniform(std::make_unique<Impl>(shaderProgram, location, std::move(name)))
{
}

template<typename Type, unsigned int Count>
Uniform<Type, Count>::operator Type() noexcept
{
    return getValue();
}

template<typename Type, unsigned int Count>
Type Uniform<Type, Count>::getValue()
{
    return impl()->getValue();
}

template<typename Type, unsigned int Count>
void Uniform<Type, Count>::setData(const void* data)
{
    impl()->setData(data);
}

template<typename Type, unsigned int Count>
typename Uniform<Type, Count>::Impl* Uniform<Type, Count>::impl() const noexcept
{
    return static_cast<Uniform<Type, Count>::Impl*>(m_impl.get());
}

//------ IMPLEMENTATION

BaseUniform::BaseImpl::BaseImpl(GLuint sProgram, GLint loc, std::string n) :
    location{loc}, name{std::move(n)}, shaderProgram{sProgram}
{
    if (shaderProgram == 0 || location < 0)
    {
        throw exceptions::GLRecAcquisitionException("Uniform is not attached to a shader program.");
    }
}

template<typename Type, unsigned int Count>
Uniform<Type, Count>::Impl::Impl(GLuint shaderProgram, GLint location, std::string name) :
    BaseImpl(shaderProgram, location, std::move(name)),
    getter{reinterpret_cast<ConcreteUniformGetter>(getUniformGetter(typeid(Type).name()))},
    setter{reinterpret_cast<ConcreteUniformSetter>(getUniformSetter(typeid(Type).name(), Count))}
{
    if (getter == nullptr)
    {
        throw exceptions::GLRecAcquisitionException("No uniform getter function for specified template arguments.");
    }
    if (setter == nullptr)
    {
        throw exceptions::GLRecAcquisitionException("No uniform setter function for specified template arguments.");
    }
}

template<typename Type, unsigned int Count>
Type Uniform<Type, Count>::Impl::getValue()
{
    auto value = Type{0};
    OGLS_GLCall(getter(shaderProgram, location, &value));
    return value;
}

template<typename Type, unsigned int Count>
void Uniform<Type, Count>::Impl::setData(const void* data)
{
    OGLS_GLCall(setter(location, Count, reinterpret_cast<const Type*>(data)));
}

namespace
{
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

#define INSTANTIATE_UNIFORM(Type)    \
    template class Uniform<Type, 1>; \
    template class Uniform<Type, 2>; \
    template class Uniform<Type, 3>; \
    template class Uniform<Type, 4>;

INSTANTIATE_UNIFORM(GLdouble);
INSTANTIATE_UNIFORM(GLfloat);
INSTANTIATE_UNIFORM(GLint);
INSTANTIATE_UNIFORM(GLuint);

#undef INSTANTIATE_UNIFORM

}  // namespace ogls::oglCore::shader
