#ifndef UNIFORMS_H
#define UNIFORMS_H

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <type_traits>

#include "exceptions.h"
#include "helpers/debugHelpers.h"
#include "helpers/macros.h"

namespace shader
{
	class ShaderProgram;

    /**
     * \brief BaseUniformSetter is a pointer to an OpenGL function to set value of uniform variable.
     */
	using BaseUniformSetter = void (*)(GLint, GLsizei, const void*);

    using BaseUniformGetter = void (*)(GLuint, GLint, void*);

    /**
     * \brief Returns a pointer to an OpenGL function, which satisfies to passed type name and count.
     * 
     * \param typeName - one of the list: "float", "double", "int", "unsigned int".
     * \param count - the integer value in the range [1, 4].
     * \return pointer to the OpenGL function or nullptr.
     */
	BaseUniformSetter getUniformSetter(const char* typeName, unsigned int count) noexcept;

    BaseUniformGetter getUniformGetter(const char* typeName) noexcept;

    /**
     * \brief BaseUniform is a base class for unifrom classes.
     * 
     * By design, object of BaseUniform class can be created only by ShaderProgram class.
     * Such architecture simulates how uniforms are used in OpenGL: 
     * OpenGL shader program must be used, after that location of the specified uniform must be received
     * and in the result the value of the uniform can be set using found location. 
     */
	class BaseUniform
	{
		public:
			virtual ~BaseUniform() = default;

            /**
             * \brief Sets the data of OpenGL uniform variable.
             * 
             * \param data - pointer to a data, which must be set in the uniform.
             */
			virtual void setData(const void* data) = 0;

		protected:
            /**
             * \brief Constructs new object.
             * 
             * \param location - a location of the uniform in a shader program.
             * \param name - a name of the uniform variable. 
             * \throw exceptions::GLRecAcquisitionException(), if location is 0
             */
			BaseUniform(GLuint shaderProgram, int location, std::string name);

			NOT_COPYABLE_MOVABLE(BaseUniform)
			
		protected:
            /**
             * \brief Location (id) of the referenced OpenGL uniform variable in a shader program.
             */
			const int m_location = -1;

            /**
             * \brief Name of the referenced OpenGL uniform variable, which is used in a code of a shader.
             */
			const std::string m_name;

            const GLuint m_shaderProgram = 0;

		friend class ShaderProgram;

	};

    /**
     * \brief Uniform represents one dimensional uniform variable, which contains [1, 4] elements.
     * 
     * \param Type - one of the list: float, double, int, unsigned int.
     * \param Count - the integer value in the range [1, 4].
     */
	template<typename Type, unsigned int Count>
	class Uniform : public BaseUniform
	{
		static_assert(std::is_same_v<float, Type> || std::is_same_v<double, Type>
			|| std::is_same_v<int, Type> || std::is_same_v<unsigned, Type>,
			"A Uniform can be of the following types: float, double, int or unsigned int.");
		static_assert(Count >= 1 && Count <= 4, "Count must be in range [1, 4].");


		private:
			using ConcreteUniformSetter = void (*)(GLint, GLsizei, const Type*);
            using ConcreteUniformGetter = void (*)(GLuint, GLint, Type*);

		public:
			void setData(const void* data) override
			{
				GLCall(m_setter(m_location, Count, reinterpret_cast<const Type*>(data)));
			}

            Type getValue()
            {
                Type value = 0;
                GLCall(m_getter(m_shaderProgram, m_location, &value));
                return value;
            }
            
            explicit operator Type() noexcept
            {
                return getValue();
            }

		protected:
            /**
             * \brief Constructs new object.
             *
             * \param location - a location of the uniform in a shader program.
             * \param name - a name of the uniform variable.
             * \throw exceptions::GLRecAcquisitionException(), if location is 0
             */
			Uniform(GLuint shaderProgram, int location, std::string name) :
				BaseUniform{ shaderProgram, location, std::move(name) },
				m_setter{ reinterpret_cast<ConcreteUniformSetter>(getUniformSetter(typeid(Type).name(), Count)) },
                m_getter{ reinterpret_cast<ConcreteUniformGetter>(getUniformGetter(typeid(Type).name())) }
			{
				if (m_setter == nullptr)
				{
					throw exceptions::GLRecAcquisitionException(
						"No uniform setter function for specified template arguments.");
				}
                if (m_getter == nullptr)
                {
                    throw exceptions::GLRecAcquisitionException(
                        "No uniform getter function for specified template arguments.");
                }
			}

		private:
            /**
             * \brief The number of elements in the uniform variable.
             */
			const unsigned int m_count = Count;

            /**
             * \brief The pointer to OpenGL function to set value of this uniform in OpenGL state machine.
             */
			const ConcreteUniformSetter m_setter = nullptr;
            const ConcreteUniformGetter m_getter = nullptr;

		friend class ShaderProgram;

	};
}

#endif
