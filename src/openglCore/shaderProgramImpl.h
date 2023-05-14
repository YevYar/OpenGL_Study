#ifndef OGLS_OGLCORE_SHADER_SHADER_PROGRAM_IMPL_H
#define OGLS_OGLCORE_SHADER_SHADER_PROGRAM_IMPL_H

#include "shaderProgram.h"

namespace ogls::oglCore::shader
{
/**
 * \brief Impl contains private data and methods of Shader.
 */
struct Shader::Impl
{
    public:
        /**
         * \brief Constructs new Shader object and generates new 1 shader in OpenGL state machine.
         *
         * Wraps [glCreateShader()](https://docs.gl/gl4/glCreateShader),
         * [glShaderSource()](https://docs.gl/gl4/glShaderSource),
         * [glCompileShader()](https://docs.gl/gl4/glCompileShader), [glGetShaderiv()](https://docs.gl/gl4/glGetShader),
         * [glGetShaderInfoLog()](https://docs.gl/gl4/glGetShaderInfoLog).
         *
         * \param type         - the type of created shader.
         * \param shaderSource - a source code of the shader.
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        Impl(ShaderType type, const std::string& shaderSource);
        OGLS_NOT_COPYABLE_MOVABLE(Impl)
        /**
         * \brief Deletes shader in OpenGL state machine.
         *
         * Wraps [glDeleteShader()](https://docs.gl/gl4/glDeleteShader).
         */
        ~Impl() noexcept;

    public:
        /**
         * \brief ID of referenced OpenGL shader object.
         */
        GLuint           rendererId = {0};
        /**
         * \brief The type of created shader.
         */
        const ShaderType type       = ShaderType::VertexShader;

};  // struct Shader::Impl

/**
 * \brief Impl contains private data and methods of ShaderProgram.
 */
struct ShaderProgram::Impl
{
    public:
        /**
         * \brief Constructs new ShaderProgram object, generates and compiles new 1 shader program
         * in OpenGL state machine.
         *
         * Wraps [glCreateProgram()](https://docs.gl/gl4/glCreateProgram),
         * [glAttachShader()](https://docs.gl/gl4/glAttachShader), [glLinkProgram()](https://docs.gl/gl4/glLinkProgram),
         * [glValidateProgram()](https://docs.gl/gl4/glValidateProgram),
         * [glGetProgramiv()](https://docs.gl/gl4/glGetProgram),
         * [glGetProgramInfoLog()](https://docs.gl/gl4/glGetProgramInfoLog),
         * [glDetachShader()](https://docs.gl/gl4/glDetachShader).
         *
         * \param vertexShader   - an object of Shader class with the type ShaderType::VERTEX_SHADER.
         * \param fragmentShader - an object of Shader class with the type ShaderType::FRAGMENT_SHADER.
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        Impl(const Shader& vertexShader, const Shader& fragmentShader);
        OGLS_NOT_COPYABLE_MOVABLE(Impl)
        /**
         * \brief Deletes shader program in OpenGL state machine.
         *
         * Wraps [glDeleteProgram()](https://docs.gl/gl4/glDeleteProgram).
         */
        ~Impl() noexcept;

        /**
         * \brief Finds uniform location of uniform variable with specified name.
         *
         * Wraps [glGetUniformLocation()](https://docs.gl/gl4/glGetUniformLocation).
         *
         * \param uniformName - a name of uniform variable, which is used in OpenGL shader program.
         * \return location of uniform variable with specified name or throws an exception if nothing is found.
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        GLint getUniformLocation(const std::string& uniformName) const;

    public:
        /**
         * \brief ID of referenced OpenGL shader program.
         */
        GLuint                                              rendererId = {0};
        /**
         * \brief Found uniform variables, which were found by ShaderProgram::findUniform().
         */
        std::map<std::string, std::unique_ptr<BaseUniform>> uniforms;

};  // ShaderProgram::Impl

}  // namespace ogls::oglCore::shader

#endif
