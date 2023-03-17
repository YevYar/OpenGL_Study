#ifndef SHADER_PROGRAM_IMPL_H
#define SHADER_PROGRAM_IMPL_H

#include "shaderProgram.h"

namespace shader
{
    struct Shader::Impl
    {
        public:
            Impl(ShaderType type, const std::string& shaderSource);

            NOT_COPYABLE_MOVABLE(Impl)

            ~Impl();

        public:
            /**
             * \brief Id of referenced OpenGL shader object.
             */
            GLuint m_rendererId = 0;
            const ShaderType m_type = ShaderType::VERTEX_SHADER;

    };

    struct ShaderProgram::Impl
    {
        public:
            Impl(const Shader& vertexShader, const Shader& fragmentShader);

            NOT_COPYABLE_MOVABLE(Impl)

            ~Impl();

            GLint getUniformLocation(const std::string& uniformName) const;

        public:
            /**
             * \brief Id of referenced OpenGL shader program.
             */
            GLuint m_rendererId = 0;
            std::map<std::string, std::unique_ptr<BaseUniform>> m_uniforms;

    };
}

#endif
