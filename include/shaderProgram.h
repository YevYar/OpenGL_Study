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
	/**
	 * \brief Enum to represent type of the shader.
	 */
	enum class ShaderType : unsigned int
	{
		VERTEX_SHADER = 0x8B31, GEOMETRY_SHADER = 0x8DD9, FRAGMENT_SHADER = 0x8B30
	};

	/**
	 * \brief Shader is a wrapper over OpenGL shader.
	 */
	class Shader
	{
		public:
			/**
			 * \brief Constructs new Shader object and generates new 1 shader in OpenGL state machine.
			 * 
			 * \param type specifies the type of created shader
			 * \param shaderSource is a source code of the shader
			 * \throw exceptions::GLRecAcquisitionException
			 */
			Shader(ShaderType type, const std::string& shaderSource);

			/**
			 * \brief Destructor, which deletes shader in OpenGL state machine.
			 */
			~Shader();

			NOT_COPYABLE_MOVABLE(Shader);

		private:
			unsigned int m_rendererId = 0;
			const ShaderType m_type = ShaderType::VERTEX_SHADER;

		friend class ShaderProgram;

	};

	/**
	 * \brief ShaderProgram is a wrapper over OpenGL shader program.
	 */
	class ShaderProgram
	{
		public:
			/**
			 * \brief Constructs new ShaderProgram object, generates and compiles new 1 shader program in OpenGL state machine.
			 *
			 * \param vertexShader is an object of Shader class with the type ShaderType::VERTEX_SHADER
			 * \param fragmentShader is an object of Shader class with the type ShaderType::FRAGMENT_SHADER
			 * \throw exceptions::GLRecAcquisitionException
			 */
			ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);

			/**
			 * \brief Destructor, which deletes shader program in OpenGL state machine.
			 */
			~ShaderProgram();

			NOT_COPYABLE_MOVABLE(ShaderProgram)

			/**
			 * \brief Wrapper over [glUseProgram](https://docs.gl/gl3/glUseProgram).
			 */
			void use() const noexcept;
			
			/**
			 * \brief Find uniform location and creates BaseUniform object,
			 * which wraps the OpenGL uniform variable with specified name.
			 * 
			 * \param name is a name of uniform variable, which is used in OpenGL shader program
			 * \throw exceptions::GLRecAcquisitionException
			 */
			template<typename T, unsigned int Count>
			BaseUniform& findUniform(std::string name)
			{
				int location = 0;
				GLCall(location = glGetUniformLocation(m_rendererId, name.c_str()));
				if (location < 0)
				{
					auto excMes = std::format(
						"Cannot find location of uniform variable '{}'."
						" Check the name and is this uniform used in the shader.",
						name
					);
					throw exceptions::GLRecAcquisitionException(excMes);
				}

				auto uniform = new Uniform<T, Count>(location, name);
				m_uniforms.insert({ std::move(name), std::unique_ptr<BaseUniform>(uniform) });
				return *uniform;
			}

			/**
			 * \brief Returns BaseUniform, which wraps the OpenGL uniform variable with specified name.
			 * 
			 * To get BaseUniform for specified uniform variable this uniform must be previously found and created
			 * by calling findUniform().
			 * 
			 * \param name is a name of uniform variable, which is used in OpenGL shader program
			 * \throw std::out_of_range
			 */
			BaseUniform& getUniform(const std::string& name) const;

		private:
			unsigned int m_rendererId = 0;
			std::map<std::string, std::unique_ptr<BaseUniform>> m_uniforms;

	};

	/**
	 * \brief Creates object of ShaderProgram class, which uses specified shader sources.
	 * 
	 * \param pathToVertexShader relative to the root folder path to vertex shader source code
	 * \param pathToFragmentShader relative to the root folder path to fragment shader source code
	 * \throw std::runtime_error
	 * \throw exceptions, which can be thrown by constructors of Shader and ShaderProgram classes 
	 */
	std::unique_ptr<shader::ShaderProgram> makeShaderProgram(const std::string& pathToVertexShader,
		const std::string& pathToFragmentShader);

}

#endif
