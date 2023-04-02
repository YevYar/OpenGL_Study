#include "uniforms.h"
#include "uniformsImpl.h"

#include "exceptions.h"
#include "helpers/debugHelpers.h"
#include "helpers/openglHelpers.h"

using namespace openglCore::shader;

namespace
{
    /**
     * \brief BaseUniformSetter is a pointer to an OpenGL function to set value of uniform variable.
     */
    using BaseUniformSetter = void (*)(GLint, GLsizei, const void*);

    using BaseUniformGetter = void (*)(GLuint, GLint, void*);

    /**
     * \brief Returns a pointer to an OpenGL function, which satisfies to passed type name and count.
     *
     * \param typeName - one of the list: "float", "double", "int", "unsigned int".
     * \param count - the integer value in the range [1, 4].
     * \return pointer to the OpenGL function or nullptr.
     */
    BaseUniformSetter getUniformSetter(const char* typeName, unsigned int count) noexcept;

    BaseUniformGetter getUniformGetter(const char* typeName) noexcept;
}

BaseUniform::BaseUniform(std::unique_ptr<BaseImpl> impl) : m_impl{ std::move(impl) }
{
    if (m_impl->m_shaderProgram == 0 || m_impl->m_location < 0)
    {
        throw exceptions::GLRecAcquisitionException("Uniform is not attached to a shader program.");
    }
}

BaseUniform::~BaseUniform() = default;

template<typename Type, unsigned int Count>
Uniform<Type, Count>::Uniform(GLuint shaderProgram, GLint location, std::string name) :
    BaseUniform(std::make_unique<Impl>(shaderProgram, location, std::move(name)))
{
}

template<typename Type, unsigned int Count>
void Uniform<Type, Count>::setData(const void* data)
{
    impl()->setData(data);
}

template<typename Type, unsigned int Count>
Type Uniform<Type, Count>::getValue()
{
    return impl()->getValue();
}

template<typename Type, unsigned int Count>
Uniform<Type, Count>::operator Type() noexcept
{
    return getValue();
}

template<typename Type, unsigned int Count>
typename Uniform<Type, Count>::Impl* Uniform<Type, Count>::impl() const noexcept
{
    return static_cast<Uniform<Type, Count>::Impl*>(m_impl.get());
}


// IMPLEMENTATION

BaseUniform::BaseImpl::BaseImpl(GLuint shaderProgram, GLint location, std::string name) :
    m_shaderProgram{ shaderProgram }, m_location{ location }, m_name{ name }
{
}

template<typename Type, unsigned int Count>
Uniform<Type, Count>::Impl::Impl(GLuint shaderProgram, GLint location, std::string name) :
    BaseImpl(shaderProgram, location, std::move(name)),
    m_setter{ reinterpret_cast<ConcreteUniformSetter>(getUniformSetter(typeid(Type).name(), Count)) },
    m_getter{ reinterpret_cast<ConcreteUniformGetter>(getUniformGetter(typeid(Type).name())) }
{
    if (m_setter == nullptr)
    {
        throw exceptions::GLRecAcquisitionException(
            "No uniform setter function for specified template arguments.");
    }
    if (m_getter == nullptr)
    {
        throw exceptions::GLRecAcquisitionException(
            "No uniform getter function for specified template arguments.");
    }
}

template<typename Type, unsigned int Count>
void Uniform<Type, Count>::Impl::setData(const void* data)
{
    GLCall(m_setter(m_location, Count, reinterpret_cast<const Type*>(data)));
}

template<typename Type, unsigned int Count>
Type Uniform<Type, Count>::Impl::getValue()
{
    Type value = 0;
    GLCall(m_getter(m_shaderProgram, m_location, &value));
    return value;
}

namespace
{
    BaseUniformSetter getUniformSetter(const char* typeName, unsigned int count) noexcept
    {
        using namespace helpers;

        if (isGLfloat(typeName))
        {
            switch (count)
            {
            case 1: return reinterpret_cast<BaseUniformSetter>(glUniform1fv);
            case 2: return reinterpret_cast<BaseUniformSetter>(glUniform2fv);
            case 3: return reinterpret_cast<BaseUniformSetter>(glUniform3fv);
            case 4: return reinterpret_cast<BaseUniformSetter>(glUniform4fv);
            }
        }
        if (isGLdouble(typeName))
        {
            switch (count)
            {
            case 1: return reinterpret_cast<BaseUniformSetter>(glUniform1dv);
            case 2: return reinterpret_cast<BaseUniformSetter>(glUniform2dv);
            case 3: return reinterpret_cast<BaseUniformSetter>(glUniform3dv);
            case 4: return reinterpret_cast<BaseUniformSetter>(glUniform4dv);
            }
        }
        if (isGLint(typeName))
        {
            switch (count)
            {
            case 1: return reinterpret_cast<BaseUniformSetter>(glUniform1iv);
            case 2: return reinterpret_cast<BaseUniformSetter>(glUniform2iv);
            case 3: return reinterpret_cast<BaseUniformSetter>(glUniform3iv);
            case 4: return reinterpret_cast<BaseUniformSetter>(glUniform4iv);
            }
        }
        if (isGLunsignedInt(typeName))
        {
            switch (count)
            {
            case 1: return reinterpret_cast<BaseUniformSetter>(glUniform1uiv);
            case 2: return reinterpret_cast<BaseUniformSetter>(glUniform2uiv);
            case 3: return reinterpret_cast<BaseUniformSetter>(glUniform3uiv);
            case 4: return reinterpret_cast<BaseUniformSetter>(glUniform4uiv);
            }
        }

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
        if (isGLunsignedInt(typeName))
        {
            return reinterpret_cast<BaseUniformGetter>(glGetUniformuiv);
        }

        return nullptr;
    }
}

#define INSTANTIATE_UNIFORM(Type) \
    template class Uniform<Type, 1>;\
    template class Uniform<Type, 2>;\
    template class Uniform<Type, 3>;\
    template class Uniform<Type, 4>;\

INSTANTIATE_UNIFORM(GLfloat)
INSTANTIATE_UNIFORM(GLdouble)
INSTANTIATE_UNIFORM(GLint)
INSTANTIATE_UNIFORM(GLuint)
