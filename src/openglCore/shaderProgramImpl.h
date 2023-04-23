#ifndef OGLS_OGLCORE_SHADER_SHADER_PROGRAM_IMPL_H
#define OGLS_OGLCORE_SHADER_SHADER_PROGRAM_IMPL_H

#include "shaderProgram.h"

namespace ogls::oglCore::shader
{
struct Shader::Impl
{
    public:
        Impl(ShaderType type, const std::string& shaderSource);
        OGLS_NOT_COPYABLE_MOVABLE(Impl)
        ~Impl();

    public:
        /**
         * \brief Id of referenced OpenGL shader object.
         */
        GLuint           rendererId = {0};
        const ShaderType type       = ShaderType::VertexShader;

};  // struct Shader::Impl

struct ShaderProgram::Impl
{
    public:
        Impl(const Shader& vertexShader, const Shader& fragmentShader);
        OGLS_NOT_COPYABLE_MOVABLE(Impl)
        ~Impl();

        GLint getUniformLocation(const std::string& uniformName) const;

    public:
        /**
         * \brief Id of referenced OpenGL shader program.
         */
        GLuint                                              rendererId = {0};
        std::map<std::string, std::unique_ptr<BaseUniform>> uniforms;

};  // ShaderProgram::Impl

}  // namespace ogls::oglCore::shader

#endif
