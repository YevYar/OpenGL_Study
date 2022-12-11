#include "shaderProgram.h"

#include <glad/glad.h>
#include <iostream>

#include "debugHelpers.h"
#include "helpers.h"
#include "uniforms.h"

using namespace shader;

Shader::Shader(ShaderType type, std::string shaderSource) :
    m_type {type}
{
    const char* c_shaderSource = shaderSource.c_str();

    m_rendererId = glCreateShader(helpers::toUType(type));
    if (m_rendererId == 0)
    {
        std::cerr << getShaderNameByType(type) << " shader cannot be created" << std::endl;
    }
    else
    {
        GLCall(glShaderSource(m_rendererId, 1, &c_shaderSource, nullptr));
        GLCall(glCompileShader(m_rendererId));

        int compileResult = 0;
        GLCall(glGetShaderiv(m_rendererId, GL_COMPILE_STATUS, &compileResult));

        if (compileResult == GL_FALSE)
        {
            int logLength = 0;
            GLCall(glGetShaderiv(m_rendererId, GL_INFO_LOG_LENGTH, &logLength));
            char* errorLog = new char[logLength];
            GLCall(glGetShaderInfoLog(m_rendererId, logLength, &logLength, errorLog));
            std::cerr << getShaderNameByType(type) << " shader compilation error: " << errorLog << std::endl;
            delete[] errorLog;
            m_rendererId = 0;
        }
    }
}

Shader::~Shader()
{
    GLCall(glDeleteShader(m_rendererId));
}

ShaderProgram::ShaderProgram(Shader vertexShader, Shader fragmentShader)
{
    GLCall(m_rendererId = glCreateProgram());
    if (m_rendererId == 0)
    {
        std::cerr << "Shader program cannot be created" << std::endl;
    }
    else
    {
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
            char* errorLog = new char[errorLength];
            GLCall(glGetProgramInfoLog(m_rendererId, errorLength, &errorLength, errorLog));
            std::cerr << "Shader program creation error: " << errorLog << std::endl;
            delete[] errorLog;
            m_rendererId = 0;
        }
    }
}

ShaderProgram::~ShaderProgram()
{
    GLCall(glDeleteProgram(m_rendererId));
}

void ShaderProgram::use() const noexcept
{
    GLCall(glUseProgram(m_rendererId));
}

bool ShaderProgram::attachUniform(uniforms::BaseUniform& uniform) const noexcept
{
    GLCall(uniform.m_location = glGetUniformLocation(m_rendererId, uniform.m_name.c_str()));
    return uniform.m_location >= 0;
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
