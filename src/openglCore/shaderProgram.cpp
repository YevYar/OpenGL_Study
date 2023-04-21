#include "shaderProgram.h"
#include "shaderProgramImpl.h"

#include <format>
#include <iostream>
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

Shader::~Shader() = default;

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) :
    m_impl{std::make_unique<Impl>(vertexShader, fragmentShader)}
{
}

ShaderProgram::~ShaderProgram() = default;

template<typename Type, unsigned int Count>
BaseUniform& ShaderProgram::findUniform(std::string name)
{
    if (m_impl->uniforms.contains(name))
    {
        return getUniform(name);
    }

    const auto location = m_impl->getUniformLocation(name);
    auto       uniform  = new Uniform<Type, Count>{m_impl->rendererId, location, name};
    m_impl->uniforms.insert({std::move(name), std::unique_ptr<BaseUniform>(uniform)});
    return *uniform;
}

BaseUniform& ShaderProgram::getUniform(const std::string& name) const
{
    return *(m_impl->uniforms.at(name).get());
}

void ShaderProgram::use() const noexcept
{
    OGLS_GLCall(glUseProgram(m_impl->rendererId));
}

std::unique_ptr<ShaderProgram> makeShaderProgram(const std::string& pathToVertexShader,
                                                 const std::string& pathToFragmentShader)
{
    const auto vShaderSource = helpers::readStringFromFile(pathToVertexShader),
               fShaderSource = helpers::readStringFromFile(pathToFragmentShader);
    if (vShaderSource.empty() || fShaderSource.empty())
    {
        throw std::runtime_error{"Vertex or fragment shader source is empty."};
    }

    const auto vShader = Shader{ShaderType::VERTEX_SHADER, vShaderSource},
               fShader = Shader{ShaderType::FRAGMENT_SHADER, fShaderSource};

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

    auto compileResult = int{0};
    OGLS_GLCall(glGetShaderiv(rendererId, GL_COMPILE_STATUS, &compileResult));

    if (compileResult == GL_FALSE)
    {
        auto logLength = int{0};
        OGLS_GLCall(glGetShaderiv(rendererId, GL_INFO_LOG_LENGTH, &logLength));
        auto errorLog = std::vector<GLchar>(logLength);
        OGLS_GLCall(glGetShaderInfoLog(rendererId, logLength, &logLength, &errorLog[0]));

        OGLS_GLCall(glDeleteShader(rendererId));

        const auto excMes = std::format("{} shader compilation error: {}", getShaderNameByType(type), errorLog.data());
        throw exceptions::GLRecAcquisitionException{excMes};
    }
}

Shader::Impl::~Impl()
{
    OGLS_GLCall(glDeleteShader(rendererId));
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

    auto validationResult = int{0};
    OGLS_GLCall(glGetProgramiv(rendererId, GL_VALIDATE_STATUS, &validationResult));
    auto linkingResult = int{0};
    OGLS_GLCall(glGetProgramiv(rendererId, GL_LINK_STATUS, &linkingResult));

    if (linkingResult == GL_FALSE || validationResult == GL_FALSE)
    {
        auto errorLength = int{0};
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

ShaderProgram::Impl::~Impl()
{
    OGLS_GLCall(glDeleteProgram(rendererId));
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
            case ShaderType::VERTEX_SHADER:
                return "VERTEX";
            case ShaderType::FRAGMENT_SHADER:
                return "FRAGMENT";
            case ShaderType::GEOMETRY_SHADER:
                return "GEOMETRY";
            default:
            {
                OGLS_ASSERT(false);
                return "VERTEX";
            }
        };
    }
}  // namespace

#define INSTANTIATE_FIND_UNIFORM(Type)                                      \
    template BaseUniform& ShaderProgram::findUniform<Type, 1>(std::string); \
    template BaseUniform& ShaderProgram::findUniform<Type, 2>(std::string); \
    template BaseUniform& ShaderProgram::findUniform<Type, 3>(std::string); \
    template BaseUniform& ShaderProgram::findUniform<Type, 4>(std::string);

INSTANTIATE_FIND_UNIFORM(GLdouble);
INSTANTIATE_FIND_UNIFORM(GLfloat);
INSTANTIATE_FIND_UNIFORM(GLint);
INSTANTIATE_FIND_UNIFORM(GLuint);

#undef INSTANTIATE_FIND_UNIFORM

}  // namespace ogls::oglCore::shader
