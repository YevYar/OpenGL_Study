#ifndef UNIFORMS_H
#define UNIFORMS_H

#include <glad/glad.h>
#include <memory>
#include <string>
#include <type_traits>

#include "helpers/macros.h"

namespace openglCore::shader
{
	class ShaderProgram;

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
        // TYPES
        protected:
            struct BaseImpl;

        // METHODS
		public:
            BaseUniform() = delete;

            NOT_COPYABLE_MOVABLE(BaseUniform)

			virtual ~BaseUniform();

            /**
             * \brief Sets the data of OpenGL uniform variable.
             * 
             * \param data - pointer to a data, which must be set in the uniform.
             */
			virtual void setData(const void* data) = 0;

        protected:
            explicit BaseUniform(std::unique_ptr<BaseImpl> impl);

        // DATA
        protected:
            std::unique_ptr<BaseImpl> m_impl;

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
		static_assert(std::is_same_v<GLfloat, Type> || std::is_same_v<GLdouble, Type>
			|| std::is_same_v<GLint, Type> || std::is_same_v<GLuint, Type>,
			"A Uniform can be of the following types: GLfloat, GLdouble, GLint or GLuint.");
		static_assert(Count >= 1 && Count <= 4, "Count must be in range [1, 4].");


        // TYPES
        private:
            struct Impl;

        // METHODS
		public:
            Uniform() = delete;

            void setData(const void* data) override;

            Type getValue();
            
            explicit operator Type() noexcept;

		protected:
            /**
             * \brief Constructs new object.
             *
             * \param location - a location of the uniform in a shader program.
             * \param name - a name of the uniform variable.
             * \throw exceptions::GLRecAcquisitionException(), if location is 0
             */
            Uniform(GLuint shaderProgram, GLint location, std::string name);

        private:
            Impl* impl() const noexcept;
		
		friend class ShaderProgram;

	};
}

#endif
