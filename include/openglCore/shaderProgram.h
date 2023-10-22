#ifndef OGLS_OGLCORE_SHADER_SHADER_PROGRAM_H
#define OGLS_OGLCORE_SHADER_SHADER_PROGRAM_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "helpers/macros.h"
#include "uniforms.h"

/**
 * \namespace ogls::oglCore::shader
 * \brief shader namespace contains types, related to OpenGL shaders, shader programs, uniforms etc.
 */
namespace ogls::oglCore::shader
{
/**
 * \brief ShaderType represents 'shaderType' parameter of [glCreateShader()](https://docs.gl/gl4/glCreateShader).
 */
enum class ShaderType : GLenum
{
    ComputeShader        = 0x91'B9,
    FragmentShader       = 0x8B'30,
    GeometryShader       = 0x8D'D9,
    TessControlShader    = 0x8E'88,
    TessEvaluationShader = 0x8E'87,
    VertexShader         = 0x8B'31
};

/**
 * \brief Shader is a wrapper over OpenGL shader.
 */
class Shader
{
    private:
        /**
         * \brief Impl contains private data and methods of Shader.
         */
        class Impl;

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
        Shader(ShaderType type, const std::string& shaderSource);
        OGLS_NOT_COPYABLE_MOVABLE(Shader)
        /**
         * \brief Deletes shader in OpenGL state machine.
         *
         * Wraps [glDeleteShader()](https://docs.gl/gl4/glDeleteShader).
         */
        ~Shader() noexcept;

    private:
        /**
         * \brief Pointer to implementation.
         */
        std::unique_ptr<Impl> m_impl;


        friend class ShaderProgram;

};  // class Shader

/**
 * \brief ShaderProgram is a wrapper over OpenGL shader program.
 */
class ShaderProgram
{
    private:
        /**
         * \brief Impl contains private data and methods of ShaderProgram.
         */
        class Impl;

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
        ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
        OGLS_NOT_COPYABLE_MOVABLE(ShaderProgram)
        /**
         * \brief Deletes shader program in OpenGL state machine.
         *
         * Wraps [glDeleteProgram()](https://docs.gl/gl4/glDeleteProgram).
         */
        ~ShaderProgram() noexcept;

        /**
         * \brief Finds matrix uniform location and creates MatrixUniform<N, M> object,
         * which wraps the OpenGL matrix uniform variable with specified name.
         *
         * Uses [glGetUniformLocation()](https://docs.gl/gl4/glGetUniformLocation).
         *
         * \param N    - a number of rows in the Matrix in range [2, 4].
         * \param M    - a number of columns in the Matrix in range [2, 4].
         * \param name - a name of matrix uniform variable, which is used in OpenGL shader program.
         * \see MatrixUniform.
         * \return created MatrixUniform<N, M> object via reference to base class BaseUniform
         * or throws an exception if nothing is found.
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        template<size_t N, size_t M>
        BaseUniform& findMatrixUniform(std::string name);
        /**
         * \brief Finds uniform location and creates Uniform<Type, Count> object,
         * which wraps the OpenGL uniform variable with specified name.
         *
         * Uses [glGetUniformLocation()](https://docs.gl/gl4/glGetUniformLocation).
         *
         * \param Type  - one of the list: GLfloat, GLdouble, GLint, GLuint.
         * \param Count - the integer value in the range [1, 4].
         * \param name  - a name of uniform variable, which is used in OpenGL shader program.
         * \see Uniform.
         * \return created Uniform<Type, Count> object via reference to base class BaseUniform
         * or throws an exception if nothing is found.
         * \throw ogls::exceptions::GLRecAcquisitionException().
         */
        template<typename Type, unsigned int Count>
        BaseUniform& findUniform(std::string name);
        /**
         * \brief Returns uniform wrapper object via reference to base class BaseUniform,
         * which wraps the OpenGL uniform variable with specified name.
         *
         * To get uniform wrapper object for specified uniform variable this uniform must be previously found
         * and created by calling findMatrixUniform() or findUniform().
         *
         * \param name - a name of uniform variable, which is used in OpenGL shader program.
         * \see findMatrixUniform(), findUniform().
         * \return found BaseUniform object or throws an exception if nothing is found.
         * \throw std::out_of_range.
         */
        BaseUniform& getUniform(const std::string& name) const;
        /**
         * \brief Wraps [glUseProgram()](https://docs.gl/gl4/glUseProgram).
         */
        void         use() const;

    private:
        /**
         * \brief Pointer to implementation.
         */
        std::unique_ptr<Impl> m_impl;

};  // class ShaderProgram

/**
 * \brief Creates object of ShaderProgram class, which uses specified shader sources.
 *
 * \param pathToVertexShader   - relative to the root folder path to vertex shader source code.
 * \param pathToFragmentShader - relative to the root folder path to fragment shader source code.
 * \return created ShaderProgram object.
 * \throw std::runtime_error,
 * exceptions, which can be thrown by constructors of Shader and ShaderProgram classes.
 */
std::unique_ptr<ShaderProgram> makeShaderProgram(std::string_view pathToVertexShader,
                                                 std::string_view pathToFragmentShader);

}  // namespace ogls::oglCore::shader

#endif
