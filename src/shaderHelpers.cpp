#include <glad/glad.h>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "shaderHelpers.h"

std::string readShaderFromFile(std::string pathToShader)
{
    if (!std::filesystem::exists(pathToShader))
    {
        std::cerr << "Shader file does not exist at path " << pathToShader << std::endl;
        return "";
    }

    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::stringstream shaderSourceStream;
    try
    {
        file.open(pathToShader, std::ios_base::in);
        shaderSourceStream << file.rdbuf();
        file.close();
    }
    catch (std::exception exc)
    {
        file.close();
        std::cerr << "Shader loading error (path: " << pathToShader << "): " << exc.what() << std::endl;
        return "";
    }

    return shaderSourceStream.str();
}

GLuint createShader(GLenum shaderType, std::string shaderSource)
{
    const char* c_shaderSource = shaderSource.c_str();

    GLuint shader = glCreateShader(shaderType);
    if (shader == 0)
    {
        std::cerr << getShaderNameByType(shaderType) << " shader cannot be created" << std::endl;
        return 0;
    }

    glShaderSource(shader, 1, &c_shaderSource, nullptr);
    glCompileShader(shader);

    int compileResult = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

    if (compileResult == GL_FALSE)
    {
        int logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        char* errorLog = new char[logLength];
        glGetShaderInfoLog(shader, logLength, &logLength, errorLog);
        std::cerr << getShaderNameByType(shaderType) << " shader compilation error: " << errorLog << std::endl;
        delete[] errorLog;
        return 0;
    }

    return shader;
}

std::string getShaderNameByType(GLenum shaderType)
{
    switch (shaderType)
    {
    case GL_VERTEX_SHADER:
        return "VERTEX";
    case GL_FRAGMENT_SHADER:
        return "FRAGMENT";
    default:
        return "UNDEFINED";
    };
}

GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint program = glCreateProgram();
    if (program == 0)
    {
        std::cerr << "Shader program cannot be created" << std::endl;
        return 0;
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);

    int validationResult = 0;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &validationResult);
    int linkingResult = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkingResult);

    if (linkingResult == GL_FALSE || validationResult == GL_FALSE)
    {
        int errorLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &errorLength);
        char* errorLog = new char[errorLength];
        glGetProgramInfoLog(program, errorLength, &errorLength, errorLog);
        std::cerr << "Shader program creation error: " << errorLog << std::endl;
        delete[] errorLog;
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}
