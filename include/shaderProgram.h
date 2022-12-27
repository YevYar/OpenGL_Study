#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include <format>
#include <map>
#include <string>

#include "exceptions.h"
#include "helpers/debugHelpers.h"
#include "helpers/macros.h"
#include "uniforms.h"

namespace shader
{
	enum class ShaderType : unsigned int
	{
		VERTEX_SHADER = 0x8B31, GEOMETRY_SHADER = 0x8DD9, FRAGMENT_SHADER = 0x8B30
	};

	class Shader
	{
		public:
			Shader(ShaderType type, const std::string& shaderSource);
			~Shader();

			NOT_COPYABLE_MOVABLE(Shader);

			bool isValid() const noexcept { return m_rendererId != 0; }

		private:
			unsigned int m_rendererId = 0;
			const ShaderType m_type = ShaderType::VERTEX_SHADER;

			friend class ShaderProgram;

	};

	class ShaderProgram
	{
		public:
			ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
			~ShaderProgram();

			NOT_COPYABLE_MOVABLE(ShaderProgram)

			void use() const noexcept;
			bool isValid() const noexcept { return m_rendererId != 0; }

			template<typename T, unsigned int Count>
			BaseUniform& findUniform(std::string name)
			{
				GLCall(auto location = glGetUniformLocation(m_rendererId, name.c_str()));
				if (location < 0)
				{
					auto excMes = std::format(
						"Cannot find location of uniform variable '{}'."
						" Check the name and is this uniform used in the shader",
						name
					);
					throw exceptions::GLRecAcquisitionException(excMes);
				}

				auto uniform = new Uniform<T, Count>(location, name);
				m_uniforms.insert({ std::move(name), std::unique_ptr<BaseUniform>(uniform) });
				return *uniform;
			}
			BaseUniform& getUniform(const std::string& name) const;

		private:
			unsigned int m_rendererId = 0;
			std::map<std::string, std::unique_ptr<BaseUniform>> m_uniforms;

	};

	std::string getShaderNameByType(ShaderType type);

}

#endif
