#include "shaderProgram.h"
#include "shaderProgramImpl.h"

#include <format>
#include <iostream>
#include <vector>

#include "exceptions.h"
#include "helpers/debugHelpers.h"
#include "helpers/helpers.h"

using namespace shader;

namespace
{
    std::string getShaderNameByType(ShaderType type);
}

Shader::Shader(ShaderType type, const std::string& shaderSource) :
    m_impl{ std::make_unique<Impl>(type, shaderSource) }
{
}

Shader::~Shader() = default;

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) :
    m_impl{ std::make_unique<Impl>(vertexShader, fragmentShader) }
{
}

ShaderProgram::~ShaderProgram() = default;

void ShaderProgram::use() const noexcept
{
    GLCall(glUseProgram(m_impl->m_rendererId));
}

template<typename Type, unsigned int Count>
BaseUniform& ShaderProgram::findUniform(std::string name)
{
    if (m_impl->m_uniforms.contains(name))
    {
        return getUniform(name);
    }

    const auto location = m_impl->getUniformLocation(name);
    auto uniform = new Uniform<Type, Count>(m_impl->m_rendererId, location, name);
    m_impl->m_uniforms.insert({ std::move(name), std::unique_ptr<BaseUniform>(uniform) });
    return *uniform;
}

BaseUniform& ShaderProgram::getUniform(const std::string& name) const
{
    return *(m_impl->m_uniforms.at(name).get());
}

std::unique_ptr<shader::ShaderProgram> shader::makeShaderProgram(const std::string& pathToVertexShader,
	const std::string& pathToFragmentShader)
{
	auto vShaderSource = helpers::readStringFromFile(pathToVertexShader);
	auto fShaderSource = helpers::readStringFromFile(pathToFragmentShader);
	if (vShaderSource.empty() || fShaderSource.empty())
	{
		throw std::runtime_error("Vertex or fragment shader source is empty.");
	}

	Shader vShader(ShaderType::VERTEX_SHADER, vShaderSource),
		fShader(ShaderType::FRAGMENT_SHADER, fShaderSource);

	return std::make_unique<ShaderProgram>(vShader, fShader);
}


// IMPLEMENTATION

Shader::Impl::Impl(ShaderType type, const std::string& shaderSource) : m_type{ type }
{
    auto c_shaderSource = shaderSource.c_str();

    GLCall(m_rendererId = glCreateShader(helpers::toUType(type)));
    if (m_rendererId == 0)
    {
        auto excMes = std::format("{} shader cannot be created.", getShaderNameByType(type));
        throw exceptions::GLRecAcquisitionException(excMes);
    }

    GLCall(glShaderSource(m_rendererId, 1, &c_shaderSource, nullptr));
    GLCall(glCompileShader(m_rendererId));

    int compileResult = 0;
    GLCall(glGetShaderiv(m_rendererId, GL_COMPILE_STATUS, &compileResult));

    if (compileResult == GL_FALSE)
    {
        int logLength = 0;
        GLCall(glGetShaderiv(m_rendererId, GL_INFO_LOG_LENGTH, &logLength));
        std::vector<GLchar> errorLog(logLength);
        GLCall(glGetShaderInfoLog(m_rendererId, logLength, &logLength, &errorLog[0]));

        GLCall(glDeleteShader(m_rendererId));

        auto excMes = std::format("{} shader compilation error: {}",
            getShaderNameByType(type), errorLog.data());
        throw exceptions::GLRecAcquisitionException(excMes);
    }
}

Shader::Impl::~Impl()
{
    GLCall(glDeleteShader(m_rendererId));
}

ShaderProgram::Impl::Impl(const Shader& vertexShader, const Shader& fragmentShader)
{
    GLCall(m_rendererId = glCreateProgram());
    if (m_rendererId == 0)
    {
        throw exceptions::GLRecAcquisitionException("Shader program cannot be created.");
    }

    GLCall(glAttachShader(m_rendererId, vertexShader.m_impl->m_rendererId));
    GLCall(glAttachShader(m_rendererId, fragmentShader.m_impl->m_rendererId));
    GLCall(glLinkProgram(m_rendererId));
    GLCall(glValidateProgram(m_rendererId));

    int validationResult = 0;
    GLCall(glGetProgramiv(m_rendererId, GL_VALIDATE_STATUS, &validationResult));
    int linkingResult = 0;
    GLCall(glGetProgramiv(m_rendererId, GL_LINK_STATUS, &linkingResult));

    if (linkingResult == GL_FALSE || validationResult == GL_FALSE)
    {
        int errorLength = 0;
        GLCall(glGetProgramiv(m_rendererId, GL_INFO_LOG_LENGTH, &errorLength));
        std::vector<GLchar> errorLog(errorLength);
        GLCall(glGetProgramInfoLog(m_rendererId, errorLength, &errorLength, &errorLog[0]));

        GLCall(glDeleteProgram(m_rendererId));

        auto excMes = std::format("Shader program creation error: {}", errorLog.data());
        throw exceptions::GLRecAcquisitionException(excMes);
    }

    GLCall(glDetachShader(m_rendererId, vertexShader.m_impl->m_rendererId));
    GLCall(glDetachShader(m_rendererId, fragmentShader.m_impl->m_rendererId));
}

ShaderProgram::Impl::~Impl()
{
    GLCall(glDeleteProgram(m_rendererId));
}

GLint ShaderProgram::Impl::getUniformLocation(const std::string& uniformName) const
{
    GLint location = 0;
    GLCall(location = glGetUniformLocation(m_rendererId, uniformName.c_str()));

    if (location < 0)
    {
        const auto excMes = std::format(
            "Cannot find location of uniform variable '{}'."
            " Check the name and is this uniform used in the shader.",
            uniformName
        );
        throw exceptions::GLRecAcquisitionException(excMes);
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
        };
    }
}

#define INSTANTIATE_FIND_UNIFORM(Type) \
    template BaseUniform& ShaderProgram::findUniform<Type, 1>(std::string);\
    template BaseUniform& ShaderProgram::findUniform<Type, 2>(std::string);\
    template BaseUniform& ShaderProgram::findUniform<Type, 3>(std::string);\
    template BaseUniform& ShaderProgram::findUniform<Type, 4>(std::string);\

INSTANTIATE_FIND_UNIFORM(GLfloat)
INSTANTIATE_FIND_UNIFORM(GLdouble)
INSTANTIATE_FIND_UNIFORM(GLint)
INSTANTIATE_FIND_UNIFORM(GLuint)
