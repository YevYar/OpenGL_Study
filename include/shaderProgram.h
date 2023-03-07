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

/**
 * \brief shader namespace contains types, related to OpenGL shaders, shader programs, uniforms etc.
 */
namespace shader
{
	/**
	 * \brief ShaderType represents 'shaderType' parameter of [glCreateShader()](https://docs.gl/gl4/glCreateShader).
	 */
	enum class ShaderType : unsigned int
	{
        COMPUTE_SHADER = 0x91B9, VERTEX_SHADER = 0x8B31, TESS_CONTROL_SHADER = 0x8E88, TESS_EVALUATION_SHADER = 0x8E87,
        GEOMETRY_SHADER = 0x8DD9, FRAGMENT_SHADER = 0x8B30
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
			 * Wraps [glCreateShader()](https://docs.gl/gl4/glCreateShader), [glShaderSource()](https://docs.gl/gl4/glShaderSource), 
			 * [glCompileShader()](https://docs.gl/gl4/glCompileShader), [glGetShaderiv()](https://docs.gl/gl4/glGetShader), 
			 * [glGetShaderInfoLog()](https://docs.gl/gl4/glGetShaderInfoLog).
			 * 
			 * \param type - the type of created shader.
			 * \param shaderSource - a source code of the shader.
			 * \throw exceptions::GLRecAcquisitionException().
			 */
			Shader(ShaderType type, const std::string& shaderSource);

            NOT_COPYABLE_MOVABLE(Shader)

			/**
			 * \brief Deletes shader in OpenGL state machine.
			 * 
			 * Wraps [glDeleteShader()](https://docs.gl/gl4/glDeleteShader).
			 */
			~Shader();			

		private:
            /**
             * \brief Id of referenced OpenGL shader object.
             */
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
			 * \brief Constructs new ShaderProgram object, generates and compiles new 1 shader program
			 * in OpenGL state machine.
			 * 
			 * Wraps [glCreateProgram()](https://docs.gl/gl4/glCreateProgram), [glAttachShader()](https://docs.gl/gl4/glAttachShader), 
			 * [glLinkProgram()](https://docs.gl/gl4/glLinkProgram), [glValidateProgram()](https://docs.gl/gl4/glValidateProgram), 
			 * [glGetProgramiv()](https://docs.gl/gl4/glGetProgram), [glGetProgramInfoLog()](https://docs.gl/gl4/glGetProgramInfoLog).
			 *
			 * \param vertexShader - an object of Shader class with the type ShaderType::VERTEX_SHADER.
			 * \param fragmentShader - an object of Shader class with the type ShaderType::FRAGMENT_SHADER.
			 * \throw exceptions::GLRecAcquisitionException().
			 */
			ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);

            NOT_COPYABLE_MOVABLE(ShaderProgram)

			/**
			 * \brief Deletes shader program in OpenGL state machine.
			 * 
			 * Wraps [glDeleteProgram()](https://docs.gl/gl4/glDeleteProgram).
			 */
			~ShaderProgram();

			/**
			 * \brief Wraps [glUseProgram()](https://docs.gl/gl4/glUseProgram).
			 */
			void use() const noexcept;
			
			/**
			 * \brief Finds uniform location and creates BaseUniform object,
			 * which wraps the OpenGL uniform variable with specified name.
			 * 
			 * Uses [glGetUniformLocation()](https://docs.gl/gl4/glGetUniformLocation).
			 * 
			 * \param Type - one of the list: float, double, int, unsigned int.
			 * \param Count - the integer value in the range [1, 4].
			 * \param name - a name of uniform variable, which is used in OpenGL shader program.
			 * \return created BaseUniform object or throws an exception if nothing is found.
			 * \throw exceptions::GLRecAcquisitionException().
			 */
			template<typename Type, unsigned int Count>
			BaseUniform& findUniform(std::string name)
			{
                if (m_uniforms.contains(name))
                {
                    return getUniform(name);
                }
                
				int location = 0;
				GLCall(location = glGetUniformLocation(m_rendererId, name.c_str()));
				if (location < 0)
				{
					const auto excMes = std::format(
						"Cannot find location of uniform variable '{}'."
						" Check the name and is this uniform used in the shader.",
						name
					);
					throw exceptions::GLRecAcquisitionException(excMes);
				}

				auto uniform = new Uniform<Type, Count>(m_rendererId, location, name);
				m_uniforms.insert({ std::move(name), std::unique_ptr<BaseUniform>(uniform) });
				return *uniform;
			}

			/**
			 * \brief Returns BaseUniform, which wraps the OpenGL uniform variable with specified name.
			 * 
			 * To get BaseUniform for specified uniform variable this uniform must be previously found and created
			 * by calling findUniform().
			 * 
			 * \param name - a name of uniform variable, which is used in OpenGL shader program.
			 * \return found BaseUniform object or throws an exception if nothing is found.
			 * \throw std::out_of_range.
			 */
			BaseUniform& getUniform(const std::string& name) const;

		private:
            /**
             * \brief Id of referenced OpenGL shader program.
             */
			unsigned int m_rendererId = 0;
			std::map<std::string, std::unique_ptr<BaseUniform>> m_uniforms;

	};

	/**
	 * \brief Creates object of ShaderProgram class, which uses specified shader sources.
	 * 
	 * \param pathToVertexShader - relative to the root folder path to vertex shader source code.
	 * \param pathToFragmentShader - relative to the root folder path to fragment shader source code.
	 * \return created ShaderProgram object.
	 * \throw std::runtime_error, 
	 * exceptions, which can be thrown by constructors of Shader and ShaderProgram classes. 
	 */
	std::unique_ptr<shader::ShaderProgram> makeShaderProgram(const std::string& pathToVertexShader,
		const std::string& pathToFragmentShader);

}

#endif
