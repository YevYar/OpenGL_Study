#include "shaderProgram.h"
#include "shaderProgramImpl.h"

#include <format>
#include <vector>

#include "exceptions.h"
#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"

namespace ogls::oglCore::shader
{
namespace
{
    std::string getShaderNameByType(ShaderType type);

}  // namespace

Shader::Shader(ShaderType type, const std::string& shaderSource) : m_impl{std::make_unique<Impl>(type, shaderSource)}
{
}

Shader::~Shader() noexcept = default;

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) :
    m_impl{std::make_unique<Impl>(vertexShader, fragmentShader)}
{
}

ShaderProgram::~ShaderProgram() noexcept = default;

template<size_t N, size_t M>
MatrixUniform<N, M>& ShaderProgram::getMatrixUniform(const std::string& name) const
{
    return m_impl->getUniform<MatrixUniform<N, M>>(name);
}

BaseUniform& ShaderProgram::getUniform(const std::string& name) const
{
    return m_impl->getUniform(name);
}

template<typename Type, size_t Count>
VectorUniform<Type, Count>& ShaderProgram::getVectorUniform(const std::string& name) const
{
    return m_impl->getUniform<VectorUniform<Type, Count>>(name);
}

void ShaderProgram::use() const
{
    OGLS_GLCall(glUseProgram(m_impl->rendererId));
}

std::unique_ptr<ShaderProgram> makeShaderProgram(std::string_view pathToVertexShader,
                                                 std::string_view pathToFragmentShader)
{
    const auto vShaderSource = helpers::readTextFromFile(pathToVertexShader),
               fShaderSource = helpers::readTextFromFile(pathToFragmentShader);
    if (vShaderSource.empty() || fShaderSource.empty())
    {
        throw std::runtime_error{"Vertex or fragment shader source is empty."};
    }

    const auto vShader = Shader{ShaderType::VertexShader, vShaderSource},
               fShader = Shader{ShaderType::FragmentShader, fShaderSource};

    return std::make_unique<ShaderProgram>(vShader, fShader);
}

//------ IMPLEMENTATION

Shader::Impl::Impl(ShaderType t, const std::string& shaderSource) : type{t}
{
    auto c_shaderSource = shaderSource.c_str();

    OGLS_GLCall(rendererId = {glCreateShader(helpers::toUType(type))});
    if (rendererId == 0)
    {
        const auto excMes = std::format("{} shader cannot be created.", getShaderNameByType(type));
        throw exceptions::GLRecAcquisitionException{excMes};
    }

    OGLS_GLCall(glShaderSource(rendererId, 1, &c_shaderSource, nullptr));
    OGLS_GLCall(glCompileShader(rendererId));

    auto compileResult = GLint{0};
    OGLS_GLCall(glGetShaderiv(rendererId, GL_COMPILE_STATUS, &compileResult));

    if (compileResult == GL_FALSE)
    {
        auto logLength = GLint{0};
        OGLS_GLCall(glGetShaderiv(rendererId, GL_INFO_LOG_LENGTH, &logLength));
        auto errorLog = std::vector<GLchar>(logLength);
        OGLS_GLCall(glGetShaderInfoLog(rendererId, logLength, &logLength, &errorLog[0]));

        OGLS_GLCall(glDeleteShader(rendererId));

        const auto excMes = std::format("{} shader compilation error: {}", getShaderNameByType(type), errorLog.data());
        throw exceptions::GLRecAcquisitionException{excMes};
    }
}

Shader::Impl::~Impl() noexcept
{
    try
    {
        OGLS_GLCall(glDeleteShader(rendererId));
    }
    catch (...)
    {
    }
}

ShaderProgram::Impl::Impl(const Shader& vertexShader, const Shader& fragmentShader)
{
    OGLS_GLCall(rendererId = {glCreateProgram()});
    if (rendererId == 0)
    {
        throw exceptions::GLRecAcquisitionException{"Shader program cannot be created."};
    }

    OGLS_GLCall(glAttachShader(rendererId, vertexShader.m_impl->rendererId));
    OGLS_GLCall(glAttachShader(rendererId, fragmentShader.m_impl->rendererId));
    OGLS_GLCall(glLinkProgram(rendererId));
    OGLS_GLCall(glValidateProgram(rendererId));

    auto validationResult = GLint{0};
    OGLS_GLCall(glGetProgramiv(rendererId, GL_VALIDATE_STATUS, &validationResult));
    auto linkingResult = GLint{0};
    OGLS_GLCall(glGetProgramiv(rendererId, GL_LINK_STATUS, &linkingResult));

    if (linkingResult == GL_FALSE || validationResult == GL_FALSE)
    {
        auto errorLength = GLint{0};
        OGLS_GLCall(glGetProgramiv(rendererId, GL_INFO_LOG_LENGTH, &errorLength));
        auto errorLog = std::vector<GLchar>(errorLength);
        OGLS_GLCall(glGetProgramInfoLog(rendererId, errorLength, &errorLength, &errorLog[0]));

        OGLS_GLCall(glDeleteProgram(rendererId));

        const auto excMes = std::format("Shader program creation error: {}", errorLog.data());
        throw exceptions::GLRecAcquisitionException{excMes};
    }

    OGLS_GLCall(glDetachShader(rendererId, vertexShader.m_impl->rendererId));
    OGLS_GLCall(glDetachShader(rendererId, fragmentShader.m_impl->rendererId));
}

ShaderProgram::Impl::~Impl() noexcept
{
    try
    {
        OGLS_GLCall(glDeleteProgram(rendererId));
    }
    catch (...)
    {
    }
}

template<typename DerivedUniformType>
requires std::derived_from<DerivedUniformType, BaseUniform>
DerivedUniformType& ShaderProgram::Impl::getUniform(const std::string& name) const
{
    if (uniforms.contains(name))
    {
        return *(static_cast<DerivedUniformType*>(uniforms.at(name).get()));
    }

    const auto location = getUniformLocation(name);
    auto       uniform  = new DerivedUniformType{rendererId, location, name};
    uniforms.insert({name, std::unique_ptr<BaseUniform>(uniform)});
    return *uniform;
}

BaseUniform& ShaderProgram::Impl::getUniform(const std::string& name) const
{
    if (uniforms.contains(name))
    {
        return *(uniforms.at(name).get());
    }

    const auto excMes = std::format(
      "Cannot find uniform variable '{}'."
      " Check the name and is this uniform used in the shader."
      " Call ShaderProgram::getMatrixUniform() or ShaderProgram::getVectorUniform() before calling of"
      " ShaderProgram::getUniform().",
      name);
    throw exceptions::GLRecAcquisitionException{excMes};
}

GLint ShaderProgram::Impl::getUniformLocation(const std::string& uniformName) const
{
    auto location = GLint{0};
    OGLS_GLCall(location = {glGetUniformLocation(rendererId, uniformName.c_str())});

    if (location < 0)
    {
        const auto excMes = std::format(
          "Cannot find location of uniform variable '{}'."
          " Check the name and is this uniform used in the shader.",
          uniformName);
        throw exceptions::GLRecAcquisitionException{excMes};
    }

    return location;
}

namespace
{
    std::string getShaderNameByType(ShaderType type)
    {
        switch (type)
        {
            case ShaderType::VertexShader:
                return "VERTEX";
            case ShaderType::FragmentShader:
                return "FRAGMENT";
            case ShaderType::GeometryShader:
                return "GEOMETRY";
            default:
            {
                OGLS_ASSERT(false);
                return "UNSUPPORTED";
            }
        };
    }

}  // namespace

#define INSTANTIATE_FIND_UNIFORM(Type)                                                                   \
    template MatrixUniform<2, 2>&    ShaderProgram::getMatrixUniform<2, 2>(const std::string&) const;    \
    template MatrixUniform<2, 3>&    ShaderProgram::getMatrixUniform<2, 3>(const std::string&) const;    \
    template MatrixUniform<2, 4>&    ShaderProgram::getMatrixUniform<2, 4>(const std::string&) const;    \
    template MatrixUniform<3, 2>&    ShaderProgram::getMatrixUniform<3, 2>(const std::string&) const;    \
    template MatrixUniform<3, 3>&    ShaderProgram::getMatrixUniform<3, 3>(const std::string&) const;    \
    template MatrixUniform<3, 4>&    ShaderProgram::getMatrixUniform<3, 4>(const std::string&) const;    \
    template MatrixUniform<4, 2>&    ShaderProgram::getMatrixUniform<4, 2>(const std::string&) const;    \
    template MatrixUniform<4, 3>&    ShaderProgram::getMatrixUniform<4, 3>(const std::string&) const;    \
    template MatrixUniform<4, 4>&    ShaderProgram::getMatrixUniform<4, 4>(const std::string&) const;    \
    template VectorUniform<Type, 1>& ShaderProgram::getVectorUniform<Type, 1>(const std::string&) const; \
    template VectorUniform<Type, 2>& ShaderProgram::getVectorUniform<Type, 2>(const std::string&) const; \
    template VectorUniform<Type, 3>& ShaderProgram::getVectorUniform<Type, 3>(const std::string&) const; \
    template VectorUniform<Type, 4>& ShaderProgram::getVectorUniform<Type, 4>(const std::string&) const;

INSTANTIATE_FIND_UNIFORM(GLdouble);
INSTANTIATE_FIND_UNIFORM(GLfloat);
INSTANTIATE_FIND_UNIFORM(GLint);
INSTANTIATE_FIND_UNIFORM(GLuint);

#undef INSTANTIATE_FIND_UNIFORM

}  // namespace ogls::oglCore::shader
