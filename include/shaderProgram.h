#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>

namespace uniforms
{
	class BaseUniform;
}

namespace shader
{
	enum class ShaderType : unsigned int
	{
		VERTEX_SHADER = 0x8B31, GEOMETRY_SHADER = 0x8DD9, FRAGMENT_SHADER = 0x8B30
	};

	class Shader
	{
		public:
			Shader(ShaderType type, std::string shaderSource);
			~Shader();

			inline bool isValid() const noexcept { return m_rendererId != 0; }

		private:
			unsigned int m_rendererId = 0;
			ShaderType m_type = ShaderType::VERTEX_SHADER;

			friend class ShaderProgram;

	};

	class ShaderProgram
	{
		public:
			ShaderProgram(Shader vertexShader, Shader fragmentShader);
			~ShaderProgram();

			void use() const noexcept;
			inline bool isValid() const noexcept { return m_rendererId != 0; }

			bool attachUniform(uniforms::BaseUniform& uniform) const noexcept;

		private:
			unsigned int m_rendererId = 0;

	};

	std::string getShaderNameByType(ShaderType type);

}

#endif