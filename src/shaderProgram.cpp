#include "shaderProgram.h"

#include <iostream>
#include <vector>

#include "helpers/helpers.h"

using namespace shader;

Shader::Shader(ShaderType type, const std::string& shaderSource) :
    m_type{ type }
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

Shader::~Shader()
{
    GLCall(glDeleteShader(m_rendererId));
}

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader)
{
    GLCall(m_rendererId = glCreateProgram());
    if (m_rendererId == 0)
    {
        throw exceptions::GLRecAcquisitionException("Shader program cannot be created.");
    }

	GLCall(glAttachShader(m_rendererId, vertexShader.m_rendererId));
	GLCall(glAttachShader(m_rendererId, fragmentShader.m_rendererId));
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

	GLCall(glDetachShader(m_rendererId, vertexShader.m_rendererId));
	GLCall(glDetachShader(m_rendererId, fragmentShader.m_rendererId));
}

ShaderProgram::~ShaderProgram()
{
    GLCall(glDeleteProgram(m_rendererId));
}

void ShaderProgram::use() const noexcept
{
    GLCall(glUseProgram(m_rendererId));
}

BaseUniform& ShaderProgram::getUniform(const std::string& name) const
{
    return *(m_uniforms.at(name).get());
}

std::string shader::getShaderNameByType(ShaderType type)
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
